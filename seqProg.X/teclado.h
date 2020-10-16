#ifndef TECLADO_H
#define TECLADO_H

#define TECLA_LEFT       '4'
#define TECLA_RIGHT      '6'
#define TECLA_PAUSE      '5'
#define TECLA_ENTER      '#'
#define TECLA_PLAY       '#'
#define TECLA_DELETE     '*'
#define TECLA_STOP       '*'
#define TECLA_T          '0'

void teclado_init( void );
unsigned char teclado_scan( void );
unsigned char teclado_borda(void);
char altera_tecla(char tecla);

#endif
