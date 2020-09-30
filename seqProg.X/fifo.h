/* 
 * File:   fifo.h
 * Author: luis420516@gmail.com
 *
 * Created on 9/6/2020 10:15:56 PM UTC
 * "Created in MPLAB Xpress"
 */

#ifndef FIFO_H
#define	FIFO_H

void inserir_fila(const char tecla);
void reset_fila(void);
void mod_fila(char pos, char tecla);
char* ler_fila (void);
char pos_fila(void);
void alt_estado( char tecla );
char ler_estado(char tecla);
void init_atuadores(void);
char char_fila(char i);
void retirar_fila(void);
char ler_posfila(char i);


#endif	/* FIFO_H */
