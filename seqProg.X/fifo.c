#include "dispLCD4vias.h"
#include "fifo.h"


#define FIFO_INDICE_INICIO 2
#define TAM_VETOR 62


char fila[TAM_VETOR];
char ind_fila = FIFO_INDICE_INICIO;
char ind_print = FIFO_INDICE_INICIO;
unsigned int ciclos = 0;
unsigned int cont_ciclos = 0;
unsigned char indice_passos = FIFO_INDICE_INICIO;


void addIndicePassos(void)
{
    if(indice_passos < ind_fila)
        indice_passos++;
}

void decIndicePassos(void)
{
    if(indice_passos > FIFO_INDICE_INICIO)
        indice_passos--;
}

unsigned char getIndicePassos(void)
{
    return(indice_passos);
}

void resetIndicePassos(void)
{
    indice_passos = FIFO_INDICE_INICIO;
}

unsigned int getCiclos(void)
{
    int aux = fila[1];
    aux <<= 8;
    aux +=fila[0];
    return(aux);
}

unsigned int getContCiclos(void)
{
    return(cont_ciclos);
}

void resetContCiclos(void)
{
    cont_ciclos = 0;
}

void addContCiclos(void)
{
    cont_ciclos++;
}

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
    for( char i=0; i<FIFO_INDICE_INICIO; i++ )
        fila[i] = 0;

    fila[FIFO_INDICE_INICIO] = ' ';
    fila[FIFO_INDICE_INICIO + 1] = 0;
    ind_fila = FIFO_INDICE_INICIO;    
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
        if(ind_fila > FIFO_INDICE_INICIO)
            ind_fila--;
        alt_atuador(fila[ind_fila] & ~0x20);
        fila[ind_fila] = 0;
    }
    fifo_alinharPrint();
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
    fifo_alinharPrint();
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
    fifo_alinharPrint();
}

void fifo_add_ciclo(unsigned int ciclos)
{   
    fila[0] = ciclos & 0x00ff;
    fila[1] = ciclos >> 8;
}



char fifo_indice(void)
{
    return ( ind_fila );
}

char fifo_disponivel( void )
{
    return( ind_fila < TAM_VETOR );
}

char fifo_tam(void)
{
    return( TAM_VETOR );
}

void fifo_indicePrint_inc( void )
{
    if( ind_print < TAM_VETOR-1 )
    {
        if( ind_print < ind_fila-1 )
            ind_print++;
        else
            ind_print = ind_fila-1;
    }
    else
        ind_print = TAM_VETOR-1;
}
void fifo_indicePrint_dec( void )
{
    if( ind_print > FIFO_INDICE_INICIO )
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
                    if(atraso < 10)
                    {    
                        dispLCD_dataReg('0');
                        ++i;
                    }    
                }
                if( atraso >= 10 )
                {                    
                    dispLCD_dataReg(atraso/10 + '0');                    
                    ++i;
                    atraso %= 10;
                }
                
                dispLCD_dataReg(atraso + '0');
                ++i;
                    
                dispLCD_dataReg('s');
                ++i;
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

void dignum_conc(char dig, unsigned int * ptrNum )
{        
    *ptrNum = (*ptrNum * 10) + (dig - '0');        
}

void dignum_apagar(unsigned int * ptrNum )
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

void fifo_alinharPrint( void )
{
    unsigned char i;
    unsigned char aux;
    unsigned char cont = 0;
    
    for( i=ind_fila-1; i>=FIFO_INDICE_INICIO; i-- )
    {
        aux = fila[i];
        if( aux & 0x80 ) // Máscara para tempo
        {
            cont += dignum_tam( aux & 0x7F )+1;
        }
        else if( aux >= 'A' && aux <= 'z' )
        {
            cont += 2;
        }
        
        if( cont > LCD_COLS )
        {
            break;
        }
        else
            ind_print = i;
    }
}

char posAtual(char ind)
{
    char passo = 0;
    
    passo = fifo_lerPos( getIndicePassos() + ind );
    return(passo);
}

void printExec (char col, char ind)
{
    char passo = posAtual( ind );
    
    if(passo & 0x80)
    {
        passo &= 0x7F;
        if( dignum_tam( (int) passo ) == 3 )
            col-=1;
        dispLCD_num(0, col, (int)passo, dignum_tam( (int)passo) );
        dispLCD_dataReg('s');
    }   
    else if( (passo >= 'A') && (passo <= 'z') )
    {
        dispLCD_lincol(0, col);
        dispLCD_dataReg( passo & ~0x20 );                                
        dispLCD_dataReg( passo & 0x20 ? '-': '+' );
    }
}
