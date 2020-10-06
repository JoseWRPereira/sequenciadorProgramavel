/*
 * File:   serialIO.c
 * Author: curtaeletronica
 *
 * Created on 30 de Setembro de 2019, 19:36
 * 
 * Objetivo: 
 *      Camada de Abstração de Hardware:
 *          Interface com o CIs 74595 e 74165.
 * 
 * Pinos    |nº     |Conexão
 * RE0      |8      | SHLD : Shift / Load
 * RE1      |9      | CLK : Clock
 * RE2      |10     | OUT : Output
 * RE3      |1      | IN : Input
 */



#include <xc.h>
#include "serialIO.h"

char * ptrSerialIn;
char * ptrSerialOut;
unsigned char   lenSerialIO;

// ************************** HAL
#define SERIALIO_ADDRS 0x005

typedef union
{
    struct
    {         
        unsigned char OUT   :1;
        unsigned char IN    :1;
        unsigned char CLK   :1;
        unsigned char SHLD  :1;
        unsigned char       :4;
    };
} SERIALIObits_t;
volatile SERIALIObits_t SERIALIO __at(SERIALIO_ADDRS);



void serialIOload( void )
{
    SERIALIO.SHLD = 0;
    SERIALIO.SHLD = 1;
}
unsigned char serialIObyteShift( unsigned char dataIn )
{
    unsigned char dataBit;
    unsigned char dataOut;

    dataBit = 0x80;
    dataOut = 0;

    while( dataBit )
    {
        SERIALIO.CLK = 0;
        if( dataIn & dataBit )
            SERIALIO.OUT = 1;
        else
            SERIALIO.OUT = 0;

        if( SERIALIO.IN )
            dataOut |= dataBit;
        SERIALIO.CLK = 1;
        dataBit >>= 1;
    }
    SERIALIO.CLK = 0;
    return( dataOut );
}

//***************** Interface Serial IO Scan
void serialIOscan( void )
{
    unsigned char i,j;
    j = lenSerialIO - 1;
    serialIOload();
    for( i=0; i<lenSerialIO; i++ )
    {
        ptrSerialIn[i] = serialIObyteShift( ptrSerialOut[j-i]);
    }
    serialIOload();
}

//***************** Inicialiação de interface com SerialIO
void initSerialIO( char * ptrIn, char * ptrOut, unsigned char length )
{
    ptrSerialIn = ptrIn;
    ptrSerialOut = ptrOut;
    lenSerialIO = length;
// ************************** Serial IO Shift/Load
    PORTAbits.RA3 = 0;
    ANSELbits.ANS3 = 0;
    TRISAbits.TRISA3 = 0;

// ************************** Serial IO Clock
    PORTAbits.RA2 = 0;
    ANSELbits.ANS2 = 0;
    TRISAbits.TRISA2 = 0;

// ************************** Serial IO Output
    PORTAbits.RA0 = 0;
    ANSELbits.ANS0 = 0;
    TRISAbits.TRISA0 = 0;

// ************************** Serial IO Input
    PORTAbits.RA1;
    ANSELbits.ANS1 = 0;
    TRISAbits.TRISA1 = 1;
    
//***************** Inicialização dos pinos de interface com os CIs
    SERIALIO.CLK = 0;
    SERIALIO.OUT = 0;
    SERIALIO.SHLD = 1;
}