#ifndef TIMER1_H
#define TIMER1_H

#define INT_T1                          \
{                                       \
    TMR1H = ((65536-625)>>8) & 0x00FF;  \
    TMR1L =  (65536-625)     & 0x00FF;  \
    if(t1cont)                          \
        --t1cont;                       \
    else                                \
        T1CONbits.TMR1ON = 0;           \
}

void intt1_1ms( void );
void timer1_init(void);
void setT1( unsigned long int c );
unsigned long int statusT1( void );
void stopTimer(void);


#endif