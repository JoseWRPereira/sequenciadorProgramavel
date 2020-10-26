/* 
 * File:   fifo.h
 * Author: luis420516@gmail.com
 *
 * Created on 9/6/2020 10:15:56 PM UTC
 * "Created in MPLAB Xpress"
 */

#ifndef FIFO_H
#define	FIFO_H


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
char fifo_tam(void);
void fifo_indicePrint_inc( void );
void fifo_indicePrint_dec( void );
void fifo_print(void);
void dignum_conc(char dig, int * ptrNum );
void dignum_apagar(int * ptrNum );
unsigned char dignum_tam(int num);


#endif	/* FIFO_H */
