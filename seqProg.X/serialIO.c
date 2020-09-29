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

unsigned char * ptrSerialIn;
unsigned char * ptrSerialOut;
unsigned char   lenSerialIO;

// ************************** HAL
#define SERIALIO_ADDRS 0x009

typedef union
{
    struct
    {
        unsigned char SHLD:1;
        unsigned char CLK :1;
        unsigned char OUT :1;
        unsigned char IN  :1;
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
void initSerialIO( unsigned char * ptrIn, unsigned char * ptrOut, unsigned char length )
{
    ptrSerialIn = ptrIn;
    ptrSerialOut = ptrOut;
    lenSerialIO = length;
// ************************** Serial IO Shift/Load
    PORTEbits.RE0 = 0;
    ANSELbits.ANS5 = 0;
    TRISEbits.TRISE0 = 0;

// ************************** Serial IO Clock
    PORTEbits.RE1 = 0;
    ANSELbits.ANS6 = 0;
    TRISEbits.TRISE1 = 0;

// ************************** Serial IO Output
    PORTEbits.RE2 = 0;
    ANSELbits.ANS7 = 0;
    TRISEbits.TRISE2 = 0;

// ************************** Serial IO Input
    PORTEbits.RE3;
    TRISEbits.TRISE3 = 1;
    
//***************** Inicialização dos pinos de interface com os CIs
    SERIALIO.CLK = 0;
    SERIALIO.OUT = 0;
    SERIALIO.SHLD = 1;
}