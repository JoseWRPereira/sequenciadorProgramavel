# 1 "atuadores.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.00\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "atuadores.c" 2
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
# 1 "atuadores.c" 2
# 53 "atuadores.c"
void set_passo(char passo, char* ptr)
{
    switch (passo)
    {
        case 'A': *ptr |= 0x01; break;
        case 'a': *ptr &= ~0x01; break;
        case 'B': *ptr |= 0x02; break;
        case 'b': *ptr &= ~0x02; break;
        case 'C': *ptr |= 0x04; break;
        case 'c': *ptr &= ~0x04; break;
        case 'D': *ptr |= 0x08; break;
        case 'd': *ptr &= ~0x08; break;
    }
}

char ler_sensor(char passo, char* ptr)
{
    switch (passo)
    {
        case 'A': return(*ptr & 0x02); break;
        case 'a': return(*ptr & 0x01); break;
        case 'B': return(*ptr & 0x08); break;
        case 'b': return(*ptr & 0x04); break;
        case 'C': return(*ptr & 0x20); break;
        case 'c': return(*ptr & 0x10); break;
        case 'D': return(*ptr & 0x80); break;
        case 'd': return(*ptr & 0x40); break;
        default: return( 0 ); break;
    }

}
