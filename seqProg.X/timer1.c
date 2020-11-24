#include <xc.h>

unsigned long int t1cont = 0;

void intt1_1ms( void )
{
    TMR1H = ((65536-625)>>8) & 0x00FF;
    TMR1L =  (65536-625)     & 0x00FF;
   
    if(t1cont)    
        --t1cont;    
    else
        T1CONbits.TMR1ON = 0;
}

void timer1_init(void)
{
    T1CONbits.TMR1CS = 0;
    T1CONbits.T1CKPS = 0b11;
    T1CONbits.nT1SYNC = 1;
    TMR1H = ((65536-625)>>8) & 0x00FF;
    TMR1L =  (65536-625)     & 0x00FF;
    T1CONbits.TMR1GE = 0;    
    
    PIE1bits.TMR1IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    //intt1_1ms();
}

void setT1( unsigned long int c )
{
    t1cont = c;
    T1CONbits.TMR1ON = 1;
}


unsigned long int statusT1( void )
{
    return( t1cont );
}

