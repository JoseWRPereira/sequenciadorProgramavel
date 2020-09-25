/*
 * File:   delay.c
 * Author: josewrpereira
 *
 * Created on 5 de Agosto de 2020, 21:35
 */
#include <xc.h>
#include "config.h"

void delay( unsigned int t )
{
    while( t )
    {
        __delay_ms(1);
        --t;
    }
}
