#include "fifo.h"

union 
{ 
    struct
    {
        unsigned char A  :1;
        unsigned char B  :1;
        unsigned char C  :1;
        unsigned char D  :1;
        unsigned char    :4;
    };  
}estadoAtuadores;

union 
{ 
    struct
    {
        unsigned char A0  :1;
        unsigned char A1  :1;
        unsigned char B0  :1;
        unsigned char B1  :1;
        unsigned char C0  :1;
        unsigned char C1  :1;
        unsigned char D0  :1;
        unsigned char D1  :1;
    };  
}sensor;

void set_passo(char passo)
{
    switch (passo)
    {
        case 'A': estadoAtuadores.A = 1;    break;
        case 'a': estadoAtuadores.A = 0;    break;
        case 'B': estadoAtuadores.B = 1;    break;
        case 'b': estadoAtuadores.B = 0;    break;
        case 'C': estadoAtuadores.C = 1;    break;
        case 'c': estadoAtuadores.C = 0;    break;
        case 'D': estadoAtuadores.D = 1;    break;
        case 'd': estadoAtuadores.D = 0;    break;
        default: break;
    }
}

char ler_sensor(char passo)
{
    switch (passo)
    {
        case 'A': return(sensor.A1);    break;
        case 'a': return(sensor.A0);    break;
        case 'B': return(sensor.B1);    break;
        case 'b': return(sensor.B0);    break;
        case 'C': return(sensor.C1);    break;
        case 'c': return(sensor.C0);    break;
        case 'D': return(sensor.D1);    break;
        case 'd': return(sensor.D0);    break;
        default: break;
    }
}


