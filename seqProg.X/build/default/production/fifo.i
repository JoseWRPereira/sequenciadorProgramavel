# 1 "fifo.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.00\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "fifo.c" 2
# 1 "./dispLCD4vias.h" 1






void dispLCD_instReg( unsigned char i );
void dispLCD_dataReg( unsigned char d );
void dispLCD_lincol( unsigned char lin, unsigned char col);
void dispLCD_init( void );
void dispLCD( unsigned char lin, unsigned char col, const char * str );
void dispLCD_num( unsigned char lin, unsigned char col, int num, unsigned char tam );
void dispLCD_clr( void );
# 1 "fifo.c" 2

# 1 "./fifo.h" 1
# 14 "./fifo.h"
char ler_atuador(char tecla);
void alt_atuador( char tecla );
void init_atuadores(void);
void fifo_reset(void);
char fifo_lerPos(char i);
void fifo_modifica(char pos, char valor);
char * fifo_adrs(void);
void fifo_delete(void);
void fifo_add(const char passo);
void fifo_add_tempo(const char t);
char fifo_indice(void);
char fifo_disponivel( void );
char fifo_tam(void);
void fifo_indicePrint_inc( void );
void fifo_indicePrint_dec( void );
void fifo_print(void);
void dignum_conc(char dig, unsigned int * ptrNum );
void dignum_apagar(unsigned int * ptrNum );
unsigned char dignum_tam(int num);
void fifo_add_ciclo(unsigned int ciclos);
unsigned int getCiclos(void);
unsigned int getContCiclos(void);
void resetContCiclos(void);
void addContCiclos(void);
void addIndicePassos(void);
void decIndicePassos(void);
unsigned char getIndicePassos(void);
void resetIndicePassos(void);
void fifo_alinharPrint( void );
char posAtual(char ind);
void printExec (char col, char ind);
# 2 "fifo.c" 2







char fila[62];
char ind_fila = 2;
char ind_print = 2;
unsigned int ciclos = 0;
unsigned int cont_ciclos = 0;
unsigned char indice_passos = 2;


void addIndicePassos(void)
{
    if(indice_passos < ind_fila)
        indice_passos++;
}

void decIndicePassos(void)
{
    if(indice_passos > 2)
        indice_passos--;
}

unsigned char getIndicePassos(void)
{
    return(indice_passos);
}

void resetIndicePassos(void)
{
    indice_passos = 2;
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
        unsigned char A :1;
        unsigned char B :1;
        unsigned char C :1;
        unsigned char D :1;
        unsigned char :4;
    };
}atuador;







char ler_atuador(char tecla)
{
    switch(tecla)
    {
        case 'A': return(atuador.A); break;
        case 'B': return(atuador.B); break;
        case 'C': return(atuador.C); break;
        case 'D': return(atuador.D); break;
        default: return 0; break;
    }
}

void alt_atuador( char tecla )
{
    switch(tecla)
    {
        case 'A': atuador.A = atuador.A ^ 1; break;
        case 'B': atuador.B = atuador.B ^ 1; break;
        case 'C': atuador.C = atuador.C ^ 1; break;
        case 'D': atuador.D = atuador.D ^ 1; break;
    }
}

void init_atuadores(void)
{
    atuador.A = 0;
    atuador.B = 0;
    atuador.C = 0;
    atuador.D = 0;
}
# 119 "fifo.c"
void fifo_reset(void)
{
    for( char i=0; i<2; i++ )
        fila[i] = 0;

    fila[2] = ' ';
    fila[2 + 1] = 0;
    ind_fila = 2;
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
        if(ind_fila > 2)
            ind_fila--;
        alt_atuador(fila[ind_fila] & ~0x20);
        fila[ind_fila] = 0;
    }
    fifo_alinharPrint();
}

void fifo_add(const char passo)
{
    if(ind_fila < 62)
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
    if(ind_fila < 62)
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
    return( ind_fila < 62 );
}

char fifo_tam(void)
{
    return( 62 );
}

void fifo_indicePrint_inc( void )
{
    if( ind_print < 62 -1 )
    {
        if( ind_print < ind_fila-1 )
            ind_print++;
        else
            ind_print = ind_fila-1;
    }
    else
        ind_print = 62 -1;
}
void fifo_indicePrint_dec( void )
{
    if( ind_print > 2 )
        ind_print--;
}



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
    while( i <= 16 );
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

    for( i=ind_fila-1; i>=2; i-- )
    {
        aux = fila[i];
        if( aux & 0x80 )
        {
            cont += dignum_tam( aux & 0x7F )+1;
        }
        else if( aux >= 'A' && aux <= 'z' )
        {
            cont += 2;
        }

        if( cont > 16 )
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
