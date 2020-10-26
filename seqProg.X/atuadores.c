#include "fifo.h"

//      ATUADORES
#define ATUADOR_A 0x01
#define ATUADOR_B 0x02
#define ATUADOR_C 0x04
#define ATUADOR_D 0x08
//      SENSORES
#define SENSOR_A0 0x01
#define SENSOR_A1 0x02
#define SENSOR_B0 0x04
#define SENSOR_B1 0x08
#define SENSOR_C0 0x10
#define SENSOR_C1 0x20
#define SENSOR_D0 0x40
#define SENSOR_D1 0x80

/*typedef union
{ 
    struct
    {
        unsigned char A  :1;
        unsigned char B  :1;
        unsigned char C  :1;
        unsigned char D  :1;
        unsigned char    :4;
    };  
}serialOut_t;


serialOut_t estadoAtuadores;*/



/*typedef union
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
}serialIn_t;

serialIn_t sensor;*/


void set_passo(char passo, char* ptr)
{
    switch (passo)
    {
        case 'A': *ptr |= ATUADOR_A;     break;                         
        case 'a': *ptr &= ~ATUADOR_A;    break;                        
        case 'B': *ptr |= ATUADOR_B;     break;
        case 'b': *ptr &= ~ATUADOR_B;    break;
        case 'C': *ptr |= ATUADOR_C;     break;
        case 'c': *ptr &= ~ATUADOR_C;    break;
        case 'D': *ptr |= ATUADOR_D;     break;
        case 'd': *ptr &= ~ATUADOR_D;    break;        
    }    
}

char ler_sensor(char passo, char* ptr)
{
    switch (passo)
    {         
        case 'A': return(*ptr & SENSOR_A1);    break;
        case 'a': return(*ptr & SENSOR_A0);    break;
        case 'B': return(*ptr & SENSOR_B1);    break;
        case 'b': return(*ptr & SENSOR_B0);    break;
        case 'C': return(*ptr & SENSOR_C1);    break;
        case 'c': return(*ptr & SENSOR_C0);    break;
        case 'D': return(*ptr & SENSOR_D1);    break;
        case 'd': return(*ptr & SENSOR_D0);    break;
        default:  return( 0 );                 break;
    }
        
}
