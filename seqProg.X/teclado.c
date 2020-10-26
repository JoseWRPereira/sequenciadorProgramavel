#include "teclado.h"
/*
 * File:   teclado.c
 * Author: josewrpereira
 *
 * Created on 18 de Agosto de 2020, 07:56
 * 
 * -------------------------------------------------
 *          MAPA DE ENTRADAS E SAIDAS
 * -------------------------------------------------
 *  Pinos   |nº     | Conexão
 * ---------|-------|-------------------------------
 *  RD0     |33     | Teclado Entrada C0
 *  RD1     |34     | Teclado Entrada C1
 *  RD2     |35     | Teclado Entrada C2
 *  RD3     |36     | Teclado Entrada C3
 *  RD4     |37     | Teclado Saída L0
 *  RD5     |38     | Teclado Saída L1
 *  RD6     |39     | Teclado Saída L2
 *  RD7     |40     | Teclado Saída L3
 * ------------------------------------------------- * 
 */


#include <xc.h>
#include "delay.h"


void teclado_init( void )
{
    PORTD = 0x00;
    TRISD = 0xF0;
    
}

#define L0          PORTDbits.RD0
#define L1          PORTDbits.RD1
#define L2          PORTDbits.RD2
#define L3          PORTDbits.RD3
#define C0          PORTDbits.RD4
#define C1          PORTDbits.RD5
#define C2          PORTDbits.RD6
#define C3          PORTDbits.RD7


#define L0_MASK         0x01
#define C0_MASK         0x10


char teclas[4][4] = {{'1','2','3','A'},
                     {'4','5','6','B'},
                     {'7','8','9','C'},
                     {'*','0','#','D'}};

unsigned char teclado_scan( void )
{
   char tecla = 0;
//    char l;
//    
//    for(l=0; l<4; l++)
//    {
//        PORTD = 0x01 << l;
//        if(L0 == 1) tecla = teclas[l][0];
//        if(L1 == 1) tecla = teclas[l][1];
//        if(L2 == 1) tecla = teclas[l][2];
//        if(L3 == 1) tecla = teclas[l][3];
//        
//    }
//    return( tecla );
//    unsigned char aux = 0;
    unsigned char l,c;
    
    for( l=0; l<4; l++ )
    {
        PORTD = (L0_MASK << l);
        //delay(100);
        for( c=0; c<4; c++ )
        {
            //delay(1000);
            if( PORTD & (C0_MASK<<c) )
            {
                tecla = teclas[l][c];
                //return( tecla );
            }
        }
    }   
    return( tecla );
    
}    

unsigned char teclaAnterior = 0;
unsigned char teclado_borda(void)
{
    char tecla = 0;
    char aux = 0;
        
    tecla = teclado_scan();
    
    if(tecla && !teclaAnterior)
        aux = tecla;
        
    teclaAnterior = tecla;
        
    return(aux);
}

char teclado_tempo(char tecla)
{
    switch(tecla)
    {
        case '0':   return( 0 );        break;
        case '1':   return( 1 );        break;
        case '2':   return( 2 );        break;
        case '3':   return( 3 );        break;
        case '4':   return( 4 );        break;
        case '5':   return( 5 );        break;
        case '6':   return( 6 );        break;
        case '7':   return( 7 );        break;
        case '8':   return( 8 );        break;
        case '9':   return( 9 );        break;
        default:    return( 0 );        break;
    }
}




