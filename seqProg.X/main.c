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
    char estado = 0;
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
    
    
    while( 1 )
    {
        switch(estado)
        {
            case 0:     
                dispLCD(0, 0, "Arre");
                estado = 1;
                break;            
                
            case 1:
                setT1(2000);
                estado = 2;
                break;
                
            case 2:
                if(!statusT1())
                    estado = 3;                
                break;
                
            case 3:
                dispLCD(0, 0, "Egua");
                setT1(2000);
                estado = 4;
                break;
                
            case 4:
                if(!statusT1())
                    estado = 0;                
                break;
                
        }
    }

    while(1)
    {        
         switch(estado)
        {        
            case 0:
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
                    estado = 1;
                    break;
                    
            case 1:
                    dispLCD(0, 2, "PIController");
                    delay(3000);
                    dispLCD_clr();
                    estado = 2;
                    break;
                    
             case 2:
                    dispLCD(0, 0, "Ajuste o inicio ");
                    dispLCD(1, 0, "   do sistema   ");
                    delay(3000);
                    dispLCD_clr();
                    estado = 3;
                
            case 3:
                    init_atuadores();                    
                    estado = 4;
                    break;
            
            case 4:
                    exib_LCD( tela );                    
                    estado = 5;
                    break;
                            
            case 5:
                    tecla = teclado_borda();
                    if( tecla )                             // NAO ALTERAR DE POSICAO
                        estado = 4;                    
                    switch(tecla)
                    {
                        case 'A':   alt_estado(tecla); tela[4]  = ler_estado('A') ? '+' : '-';     break;
                        case 'B':   alt_estado(tecla); tela[7]  = ler_estado('B') ? '+' : '-';     break;
                        case 'C':   alt_estado(tecla); tela[10] = ler_estado('C') ? '+' : '-';     break;
                        case 'D':   alt_estado(tecla); tela[13] = ler_estado('D') ? '+' : '-';     break;
                        case '#':   estado = 10;                                                   break;                        
                    }
                    break;

                    // TELA DE EDIÇAO

            case 10:
                    dispLCD_clr();
                    iPassos = 0;
                    dispLCD(0, 0, "Insira os passos");
                    dispLCD(1, 0, "     00/20      ");
                    estado = 11;
                    break;

             case 11:
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
                                    estado = 12;
                                    break;
                        case '>':
                                    estado = 20;
                                    break;
                        case '<':
                                    estado = 21;                        
                                    break;
                                    
                        case '3':   
                                    retirar_fila();
                                    if( iPassos > ( pos_fila()-8 ) && iPassos )
                                        iPassos = ( pos_fila()-8 );
                                    estado = 13;
                                    break;
                                    
                        case '#':   estado = 30;        break; 
                    }
                    break;

             case 12:
                    alt_estado(atuador);  
                    inserir_fila( ler_estado(atuador) ? atuador : atuador|0x20 );
                    estado = 20;
                    break;

             case 13:
                    escreve_filaLCD( ler_fila(), iPassos );
                    estado = 14;
                    break;
                    
             case 14:
                    lin2[0] = iPassos ? '<' : ' ';
                    lin2[15] = iPassos < (pos_fila()-8) ? '>' : ' ';  
                    contPassos = pos_fila();                    
                    lin2[5] = char_fila( contPassos / 10 );                    
                    lin2[6] = char_fila( contPassos % 10 );                                        
                    lin2[7] = '/';                                        
                    lin2[8] = '2';
                    lin2[9] = '0';
                    estado = 15;
                    break;
                    
             case 15:
                    dispLCD( 1, 0, lin2 );
                    estado = 11;
                    break;

             case 20: 
                    if( pos_fila() > 8 && iPassos < pos_fila()-8 )
                        iPassos++;
                    estado = 13;
                    break;
                    
             case 21:
                    if( iPassos )
                        iPassos--;
                    estado = 13;
                    break;
                    
             case 30:   
                 dispLCD_clr();
                 dispLCD(0, 0, "   Executando   ");
                 dispLCD(1, 0, "     passos     ");
                 estado = 70;
                 meAtuadores = 0;     
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
                    estado = 2;
                    meAtuadores = -1;
                }
                meAtuadores = 2;
                break;

            case 2:
                    set_passo(auxPasso, vetorOut);
                    meAtuadores = 3;
                    break;

            case 3:
                    if( ler_sensor(auxPasso, vetorIn))
                    {
                        meAtuadores = 1;                    
                    }
                    break;
        }                                             
    }
    return;
}