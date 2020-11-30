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
    char atuador;
    char lin2[17] = "                ";
    char contPassos = 0;
    char vetorIn[1] = { 0 };
    char vetorOut[1] = { 0 };
    initSerialIO( vetorIn, vetorOut, 1 );
    char meAtuadores = -1;
    char auxPasso = 0; 
    char aux = 2;
    unsigned int num = 0;    
    unsigned int ciclos = 0;
    char botao = 0;
#define STOP    0
#define PLAY    1
#define PAUSE   2
#define STEP    3
    char boolPause = 0;
#define boolPlay !boolPause
    char boolStop = 0;
    char pauseLeft = 0;
    char pauseRight = 0;
    char meAtAux = 0;
    char next = 0;

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
                                    delay(10);
                                }
                            }

                            for(char i = 0; i <= 1; i++)
                            {
                                for(char j = 0; j <= 15; j++)
                                {
                                    dispLCD(i, j, " ");
                                    delay(10);
                                }
                            }
                            estado = TELA_EMPRESA;
                            break;
                    
            case TELA_EMPRESA:
                            dispLCD(0, 2, "PIController");
                            setT1(1000);                    
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
                            setT1(1000);
                            estado = TELA_AJUSTE_INICIAL_DELAY;
                            break;
                
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
                            dispLCD(0,0,"   A- B- C- D-  ");
                            estado = TELA_EDICAO_ATUADORES;
                            break;
                            
            case TELA_EDICAO_ATUADORES:
                            tecla = teclado_borda();
                            switch(tecla)
                            {
                                case 'A':   alt_atuador(tecla); dispLCD_lincol(0, 4); dispLCD_dataReg(ler_atuador('A') ? '+' : '-');     break;
                                case 'B':   alt_atuador(tecla); dispLCD_lincol(0, 7); dispLCD_dataReg(ler_atuador('B') ? '+' : '-');     break;
                                case 'C':   alt_atuador(tecla); dispLCD_lincol(0,10); dispLCD_dataReg(ler_atuador('C') ? '+' : '-');     break;
                                case 'D':   alt_atuador(tecla); dispLCD_lincol(0,13); dispLCD_dataReg(ler_atuador('D') ? '+' : '-');     break;
                                case TECLA_ENTER:   estado = TELA_INSERIR_PASSOS;                            break;                        
                            }
                            break;

                    // TELA DE EDIÇAO

            case TELA_INSERIR_PASSOS:
                            dispLCD_clr();
                            dispLCD(0, 0, "Insira os passos");
                            estado = TELA_CONTAR_PASSOS;
                            break;

             case TELA_EDITAR_PASSOS:
                            tecla = teclado_borda();
                            switch( tecla )
                            {
                                case 'A': 
                                case 'B': 
                                case 'C': 
                                case 'D': 
                                            if( fifo_disponivel() )
                                            {
                                                alt_atuador(tecla);  
                                                fifo_add( ler_atuador(tecla) ? tecla : tecla|0x20 );
                                                estado = TELA_PRINTFILA;
                                            }
                                            break;
                                case TECLA_RIGHT:
                                            fifo_indicePrint_inc();
                                            estado = TELA_PRINTFILA;
                                            break;
                                case TECLA_LEFT:
                                            fifo_indicePrint_dec();
                                            estado = TELA_PRINTFILA;                        
                                            break;

                                case TECLA_DELETE:   
                                            fifo_delete();
                                            estado = TELA_PRINTFILA;
                                            break;    

                                case TECLA_ENTER:   estado = TELA_EDITA_REPETICAO;        break; 

                                case TECLA_T:       estado = TELA_EDITATEMPO;               break;
                            }
                            break; 

             case TELA_PRINTFILA:
                            fifo_print();
                            estado = TELA_CONTAR_PASSOS;
                            break;
                    
             case TELA_CONTAR_PASSOS:
                            dispLCD(1,0,"<  /  >");
                            dispLCD_num(1, 1,( fifo_indice()-2 ), 2);
                            dispLCD_num(1, 4,( fifo_tam()-2 ), 2);
                            estado = TELA_EDITAR_PASSOS;
                            break;
                    
             case TELA_EXECUTANDO_PASSOS:
                            //dispLCD_clr();                            
                            estado = ME_ESPERA_EXECUCAO;
                            meAtuadores = ME_ATUADORES_START;
                            break;
                 
             case ME_ESPERA_EXECUCAO:
                            if( meAtuadores == 3 && botao != PAUSE )
                            {                                
                                dispLCD(0, 0, "     [    ]     ");
                                printExec(1, -1);
                                printExec(7, 0);
                                printExec(12, 1);  
                                dispLCD(1, 0, "<  /  > c:      ");
                                dispLCD_num(1, 4,( fifo_indice() - 2 ), 2);
                                dispLCD_num(1, 1, ( getIndicePassos() - 1 ), 2);                                         
                                dispLCD_num(1, 11, ( getContCiclos() + 1 ), 5);                                            
                                
                            }
                            if(meAtuadores == 0)
                            {
                                estado = TELA_PRINTFILA;
                                dispLCD_clr();
                            }
                            tecla = teclado_borda();
                            switch(tecla)
                            {
                                case TECLA_PLAY:
                                    botao = PLAY;
                                    break;
                                
                                case TECLA_PAUSE:
                                    botao = PAUSE;
                                    break;
                                    
                                case TECLA_STOP:
                                    botao = STOP;
                                    break;
                                   
                                case TECLA_STEP:
                                    if(botao == PAUSE)
                                        botao = STEP;
                                    break;
                            }
                            break;
                 
             case CONFIG_TEMPO:
                            tecla = teclado_borda();
                            if(tecla >= '0' && tecla <= '9')
                            {
                                dignum_conc(tecla, &num);
                                if(num > 120)
                                    num = 120;
                                estado = TELA_EDITATEMPO;                                
                            }
                            if(tecla == TECLA_DELETE)
                            {
                                dignum_apagar(&num);
                                estado = TELA_EDITATEMPO;
                            }
                            if(tecla == TECLA_ENTER)
                                estado = INSERIR_TEMPOFILA;
                            break;
                            
             case TELA_EDITATEMPO:
                            dispLCD(1, 0, "cfg tempo:     s");
                            dispLCD_num(1, 11, num, 3);
                            estado = CONFIG_TEMPO;
                            break;
                            
             case INSERIR_TEMPOFILA:
                            fifo_add_tempo(num);
                            dispLCD_clr();
                            estado = TELA_PRINTFILA;
                            break;
                            
             case TELA_TITULO_REPETICAO:
                            dispLCD_clr();
                            dispLCD(0, 0, "    Tela de     ");
                            dispLCD(1, 0, "   Repeticao    ");
                            setT1(2000);
                            estado = TELA_DIGNUM_REPETICAO;
                            break;
                 
             case TELA_DIGNUM_REPETICAO:
                            if(!statusT1())
                            {
                                dispLCD_clr();
                                dispLCD(0, 0, "Digite o numero ");
                                dispLCD(1, 0, " de repeticoes  ");
                                setT1(2000);
                                estado = TELA_CONFIG_REPETICAO;
                            }
                            break;
                
             case TELA_DIGNUM_DELAY:
                            if(!statusT1())
                            {
                               dispLCD_clr(); 
                               estado = TELA_EDITA_REPETICAO;                    
                            }
                            break;
                 
                
             case TELA_CONFIG_REPETICAO:
                            tecla = teclado_borda();
                            if(tecla >= '0' && tecla <= '9')
                               {
                                dignum_conc(tecla, &ciclos);
                                if(ciclos > 10000)
                                    ciclos = 10000;
                                estado = TELA_EDITA_REPETICAO;
                                break;
                               }
                            switch(tecla)
                               {
                                   case 'A':
                                   case 'B':
                                   case 'C':
                                   case 'D':
                                       dispLCD(1, 0, "                ");
                                       estado = TELA_PRINTFILA;
                                       break;
                                   case '#':
                                       estado = ME_INSERIR_CICLO_FILA;
                                       break;
                                   case '*':
                                       dignum_apagar(&ciclos);
                                       estado = TELA_EDITA_REPETICAO;
                                       break;
                               }                    
                            break;                
                 
             case TELA_EDITA_REPETICAO: 
                            dispLCD(0, 0, "Ciclos:         ");
                            dispLCD_num(0, 8, ciclos, 5);
                            dispLCD(1, 0, "0-Ciclo continuo");
                            estado = TELA_CONFIG_REPETICAO;
                            break;
                    
             case ME_INSERIR_CICLO_FILA:
                            fifo_add_ciclo(ciclos);
                            estado = TELA_EXECUTANDO_PASSOS;
                            botao = PLAY;
                            break;
        }
 
        serialIOscan();     
 
        switch(meAtuadores)
        {
            case 0:         break;
            
            case 1:
                            resetIndicePassos();
                            resetContCiclos();                    
                            meAtuadores = 2;
                            break;

            case 2:
                            auxPasso = fifo_lerPos(getIndicePassos());
                           
                            if(botao == PLAY || botao == STEP || botao == STOP)
                                meAtuadores = 3;
                            break;

            case 3:
                            if(auxPasso & 0x80)
                            {
                                setT1( (unsigned long int)(auxPasso & 0x7F) * 1000 );
                                meAtuadores = 4;
                            }
                            else
                            {
                                set_passo(auxPasso, vetorOut);
                                meAtuadores = 5;
                            }
                            break;

            case 4:
                            if(!statusT1())
                                meAtuadores = 6;
                            break;

            case 5:
                            if(ler_sensor(auxPasso, vetorIn))
                                meAtuadores = 6;
                            break; 
                    
            case 6:
                            if(botao == STEP)
                            {
                                botao = PAUSE;
                            }
                            meAtuadores = 7;
                            
                            break;
                            
            case 7:
                            addIndicePassos();
                            if(getIndicePassos() < fifo_indice())
                                meAtuadores = 2;
                            else
                            {
                                addContCiclos();
                                meAtuadores = 8;
                            }
                            break;
                    
            case 8:
                            if((botao != STOP) && ((getContCiclos() < getCiclos()) || (getCiclos() == 0)))
                            {
                                resetIndicePassos();
                                meAtuadores = 2;
                            }
                            else
                            {
                                meAtuadores = 0;                        
                            }
                            break;               
        }
    }
    return;
}
