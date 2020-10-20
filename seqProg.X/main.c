/*
 * File:   main.c
 * Author: josewrpereira
 *
 * Created on 15 de Agosto de 2020, 16:03:21
 *
 * -------------------------------------------------
 *          MAPA DE ENTRADAS E SAIDAS
 * -------------------------------------------------
 *  Pinos   |nº     | Conexão
 * ---------|-------|-------------------------------
 *  RD4     |21     | LCD_RS
 *  RD5     |22     | LCD_EN
 *  RD0     |27     | LCD_D4
 *  RD1     |28     | LCD_D5
 *  RD2     |29     | LCD_D6
 *  RD3     |30     | LCD_D7
 * -------------------------------------------------
 * -------------------------------------------------
 */
#include <xc.h>
#include "config.h"
#include "delay.h"
#include "dispLCD4vias.h"
#include "teclado.h"
#include "fifo.h"
#include "serialIO.h"
#include "atuadores.h"
#include "timer1.h"
#include "estados.h"

void __interrupt() irq(void)
{
    INTCONbits.GIE = 0;
    if(PIR1bits.TMR1IF && PIE1bits.TMR1IE)
    {
        PIR1bits.TMR1IF = 0;
        //INT_T1;
        intt1_1ms();
    }
    INTCONbits.GIE = 1;    
}


void main(void)
{    
    dispLCD_init();
    teclado_init();
    timer1_init();
    
    char *texto;
    char tecla = 0;
    char estado = INICIO;
    char tela[16] = {"   A- B- C- D-  "};
    char atuador;
    char iPassos;
    char lin2[17] = "                ";
    char contPassos = 0;
    char vetorIn[1] = { 0 };
    char vetorOut[1] = { 0 };
    initSerialIO( vetorIn, vetorOut, 1 );
    char meAtuadores = -1;
    char auxPasso = 0; 
    char aux = 0;
    
    

    while(1)
    {        
         switch(estado)
        {        
            case INICIO:
                    for(char i = 0; i <= 1; i++)
                    {
                        for(char j = 0; j <= 15; j++)
                        {
                            dispLCD(i, j, "\377");
                            delay(50);
                        }
                    }

                    for(char i = 0; i <= 1; i++)
                    {
                        for(char j = 0; j <= 15; j++)
                        {
                            dispLCD(i, j, " ");
                            delay(50);
                        }
                    }
                    estado = TELA_EMPRESA;
                    break;
                    
            case TELA_EMPRESA:
                    dispLCD(0, 2, "PIController");
                    setT1(2000);                    
                    estado = TELA_EMPRESA_DELAY;
                    break;
                    
             case TELA_EMPRESA_DELAY:
                    if(!statusT1())
                        estado = TELA_AJUSTE_INICIAL;                    
                    break;
                    
             case TELA_AJUSTE_INICIAL:
                    dispLCD_clr();
                    dispLCD(0, 0, "Ajuste o inicio ");
                    dispLCD(1, 0, "   do sistema   ");
                    setT1(4000);
                    estado = TELA_AJUSTE_INICIAL_DELAY;
                
             case TELA_AJUSTE_INICIAL_DELAY:
                    if(!statusT1())
                        estado = INICIO_ATUADORES;
                    break;
                    
            case INICIO_ATUADORES:
                    init_atuadores();                    
                    estado = TELA_DISPLAY_ATUADORES;
                    break;
            
            case TELA_DISPLAY_ATUADORES:
                    dispLCD_clr();
                    exib_LCD( tela );                    
                    estado = TELA_EDICAO_ATUADORES;
                    break;
                            
            case TELA_EDICAO_ATUADORES:
                    tecla = teclado_borda();
                    if( tecla )                             // NAO ALTERAR DE POSICAO
                        estado = TELA_DISPLAY_ATUADORES;                    
                    switch(tecla)
                    {
                        case 'A':   alt_estado(tecla); tela[4]  = ler_estado('A') ? '+' : '-';     break;
                        case 'B':   alt_estado(tecla); tela[7]  = ler_estado('B') ? '+' : '-';     break;
                        case 'C':   alt_estado(tecla); tela[10] = ler_estado('C') ? '+' : '-';     break;
                        case 'D':   alt_estado(tecla); tela[13] = ler_estado('D') ? '+' : '-';     break;
                        case TECLA_ENTER:   estado = TELA_INSERIR_PASSOS;                          break;                        
                    }
                    break;

                    // TELA DE EDIÇAO

            case TELA_INSERIR_PASSOS:
                    dispLCD_clr();
                    iPassos = 0;
                    dispLCD(0, 0, "Insira os passos");
                    dispLCD(1, 0, "     00/20      ");
                    estado = TELA_EDITAR_PASSOS;
                    break;

             case TELA_EDITAR_PASSOS:
                    tecla = teclado_borda();
                    switch( tecla )
                    {
                        case 'A': 
                        case 'B': 
                        case 'C': 
                        case 'D': 
                                    atuador = tecla;     
                                    if(iPassos < pos_fila()-8)
                                        iPassos = pos_fila()-8;         //alteracao feita                                    
                                    estado = ME_ALTERA_ESTADO;
                                    break;
                        case TECLA_RIGHT:
                                    estado = ME_LER_POS_FILA;
                                    break;
                        case TECLA_LEFT:
                                    estado = ME_LER_PASSOS;                        
                                    break;
                                    
                        case '3':   
                                    retirar_fila();
                                    if( iPassos > ( pos_fila()-8 ) && iPassos )
                                        iPassos = ( pos_fila()-8 );
                                    estado = ME_ESCREVE_FILA;
                                    break;
                                    
                        case '9':    
                                    
                        case TECLA_ENTER:   estado = TELA_EXECUTANDO_PASSOS;        break; 
                    }
                    break;

             case ME_ALTERA_ESTADO:
                    alt_estado(atuador);  
                    inserir_fila( ler_estado(atuador) ? atuador : atuador|0x20 );
                    estado = ME_LER_POS_FILA;
                    break;

             case ME_ESCREVE_FILA:
                    escreve_filaLCD( ler_fila(), iPassos );
                    estado = ME_CONTAR_PASSOS;
                    break;
                    
             case ME_CONTAR_PASSOS:
                    lin2[0] = iPassos ? '<' : ' ';
                    lin2[15] = iPassos < (pos_fila()-8) ? '>' : ' ';  
                    contPassos = pos_fila();                    
                    lin2[5] = char_fila( contPassos / 10 );                    
                    lin2[6] = char_fila( contPassos % 10 );                                        
                    lin2[7] = '/';                                        
                    lin2[8] = '2';
                    lin2[9] = '0';
                    estado = TELA_CONTAGEM_PASSOS;
                    break;
                    
             case TELA_CONTAGEM_PASSOS:
                    dispLCD( 1, 0, lin2 );
                    estado = TELA_EDITAR_PASSOS;
                    break;

             case ME_LER_POS_FILA: 
                    if( pos_fila() > 8 && iPassos < pos_fila()-8 )
                        iPassos++;
                    estado = ME_ESCREVE_FILA;
                    break;
                    
             case ME_LER_PASSOS:
                    if( iPassos )
                        iPassos--;
                    estado = ME_ESCREVE_FILA;
                    break;
                    
             case TELA_EXECUTANDO_PASSOS:   
                 dispLCD_clr();
                 dispLCD(0, 0, "   Executando   ");
                 dispLCD(1, 0, "     passos     ");
                 estado = 70;
                 meAtuadores = INICIO;     
                 break; 
                    
                    
        }
        serialIOscan();        
        switch(meAtuadores)
        {
            case 0: meAtuadores = 1;     break;

            case 1:                
                auxPasso = ler_posfila(aux);
                aux = (aux+1) % 21;
                if(!auxPasso)
                {
                    reset_fila();
                    aux = 0;
                    estado = TELA_EMPRESA_DELAY;
                    meAtuadores = -1;
                    break;
                }
                meAtuadores = 2;
                break;

            case 2:
                    set_passo(auxPasso, vetorOut);                    
                    //setT1(1000);
                    meAtuadores = 3;
                    break;

            case 3:
                    if( ler_sensor(auxPasso, vetorIn) )
                        meAtuadores = 1;                                                                   
                    break;
                    /*else if( !statusT1() )
                    {
                        meAtuadores = 4;                        
                    }
                    break;
                    
            case 4:
                    meAtuadores = 1;
                    break;*/
        }                                             
    }
    return;
}