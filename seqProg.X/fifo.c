#include "dispLCD4vias.h"

#define TAM_VETOR 20

char fila[TAM_VETOR];
char ind_fila = 0;
char ind_print = 0;
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
}atuador;



/*
 *              Manipulação do estado dos atuadores        
 */

char ler_atuador(char tecla)
{
    switch(tecla)
    {
        case 'A':   return(atuador.A);      break;
        case 'B':   return(atuador.B);      break;
        case 'C':   return(atuador.C);      break;
        case 'D':   return(atuador.D);      break;  
        default:    return 0;               break;
    }
}

void alt_atuador( char tecla )
{
    switch(tecla)
    {
        case 'A':   atuador.A = atuador.A ^ 1;      break;
        case 'B':   atuador.B = atuador.B ^ 1;      break;
        case 'C':   atuador.C = atuador.C ^ 1;      break;
        case 'D':   atuador.D = atuador.D ^ 1;      break;
    }
}

void init_atuadores(void)
{
    atuador.A = 0;
    atuador.B = 0;
    atuador.C = 0;
    atuador.D = 0;    
}



/*
 *              Manipulação dos elementos/dados na fila
 */

void fifo_reset(void)
{        
    fila[0] = ' ';
    fila[1] = 0;
    ind_fila = 0;    
}
char fifo_lerPos(char i)
{
    return ( fila[i] );
}

void fifo_modifica(char pos, char valor)
{
    fila[pos] = valor;
}

char * fifo_adrs(void)
{
    return fila;
}

void fifo_delete(void)
{
    if(ind_fila)
    {
        ind_fila--;
        alt_atuador(fila[ind_fila] & ~0x20);
        fila[ind_fila] = 0;
    }
}

void fifo_add(const char passo)
{
    if(ind_fila < TAM_VETOR)
    {    
        if(passo)
        {
            fila[ind_fila] = passo;
            ind_fila++;
            fila[ind_fila] = 0;
        }
    }
}

void fifo_add_tempo(const char t)
{
    if(ind_fila < TAM_VETOR)
    {    
        if(t)
        {
            fila[ind_fila] = t | 0x80;
            ind_fila++;
            fila[ind_fila] = 0;
        }
    }
}



char fifo_indice(void)
{
    return ( ind_fila );
}

char fifo_tam(void)
{
    return( TAM_VETOR );
}


void fifo_indicePrint_inc( void )
{
    ++ind_print;
    if( ind_print < TAM_VETOR )
    {
        if( !fila[ind_print] )
            ind_print--;
    }
    else
        ind_print = TAM_VETOR-1;
}
void fifo_indicePrint_dec( void )
{
    if( ind_print )
        ind_print--;
}

//|_ _ _ _ _ _ _ _ |
// A+100A-B+10b-C+120
void fifo_print(void)
{
    char completar = 0;
    char comando = 0;
    char atraso = 0;
    char i = 0;
    char indPrint = ind_print;

    dispLCD_lincol(0, 0);
    do
    {
        if( completar )
        {
            dispLCD_dataReg(' '); 
            ++i;
        }
        else
        {
            comando = fila[indPrint++];
               
            if( comando == 0 )
                completar = 1;
            else if(comando & 0x80)
            {
                atraso = comando & ~0x80;
                if( atraso >= 100 )
                {
                    dispLCD_dataReg('1'); 
                    ++i;
                    atraso %= 100;
                }
                if( atraso >= 10 )
                {
                    dispLCD_dataReg(atraso/10 + '0');
                    ++i;
                    atraso %= 10;
                }
                dispLCD_dataReg(atraso + '0');
                ++i;
                
//                dispLCD_num(0, i, comando & ~0x80, 1);
                dispLCD_dataReg('s');
                ++i;
                //i+= tam_num(comando & ~0x80)+1;            
            }
            else
            {
                switch( comando )
                {
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':                           
                                dispLCD_dataReg( comando & ~0x20 );
                                ++i;
                                dispLCD_dataReg( comando & 0x20 ? '-': '+' );
                                ++i;
                                break;

                }
                    
            }
        }
    }
    while( i <= LCD_COLS );
}




/* 
 *              Rotinas de impressão da fifo
 */

void dignum_conc(char dig, int * ptrNum )
{        
    *ptrNum = (*ptrNum * 10) + (dig - '0');        
}

void dignum_apagar(int * ptrNum )
{
    *ptrNum /= 10;    
}

unsigned char dignum_tam(int num)
{
    unsigned char tam = 0;
    
    while(num != 0)
    {
        num /= 10;
        tam++;
    }
    return ( tam );
}






//void reset_fila(void)
//{        
//    fila[0] = ' ';
//    fila[1] = 0;
//    ind_fila = 0;    
//}
//char ler_posfila(char i)
//{
//    return ( fila[i] );
//}
//void mod_fila(char pos, char tecla)
//{
//    fila[pos] = tecla;
//}
//
//void retirar_fila(void)
//{
//    if(ind_fila)
//    {
//        ind_fila--;
//        alt_atuador(fila[ind_fila] & ~0x20);
//        fila[ind_fila] = 0;
//    }
//}
//void inserir_fila(const char passo)
//{
//    if(ind_fila < TAM_VETOR)
//    {    
//        if(passo)
//        {
//            fila[ind_fila] = passo;
//            ind_fila++;
//            fila[ind_fila] = 0;
//        }
//    }
//}
//char pos_fila(void)
//{
//    return ( ind_fila );
//}

