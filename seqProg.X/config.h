/* Microchip Technology Inc. and its subsidiaries.  You may use this software
 * and any derivatives exclusively with Microchip products.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
 * TERMS.
 */

/* Microchip Technology Inc. e suas subsidi?rias.
 * Voc? pode usar este software e quaisquer derivados
 * exclusivamente com produtos Microchip.

 * ESTE SOFTWARE ? FORNECIDO PELA MICROCHIP "TAL COMO EST?".
 * NENHUMA GARANTIA, EXPRESSA, IMPL?CITA OU ESTATUT?RIA,
 * SE APLICA A ESTE SOFTWARE, INCLUINDO QUALQUER GARANTIA IMPL?CITA
 * DE N?O INFRAC??O, COMERCIALIZA??O E ADEQUA??O A UM PROP?SITO ESPEC?FICO OU
 * SUA INTERA??O COM PRODUTOS MICROCHIP,
 * COMBINA??O COM QUALQUER OUTRO PRODUTO OU UTILIZA??O EM QUALQUER APLICA??O.
 *
 * EM NENHUM CASO A MICROCHIP SER? RESPONS?VEL POR QUALQUER PERDA INDIRETO,
 * ESPECIAL, PUNITIVA, INCIDENTAL OU CONSEQUENCIAL,DANOS, CUSTO OU
 * DESPESAS DE QUALQUER TIPO DE RELA??O COM O SOFTWARE, NO ENTANTO, CAUSADO,
 * MESMO QUE MICROCHIP TENHA SIDO AVISADA DA POSSIBILIDADE OU
 * DOS DANOS PREVIS?VEIS.
 * NA M?XIMA EXTENS?O PERMITIDA POR LEI,A RESPONSABILIDADE TOTAL DA MICROCHIP
 * EM TODAS AS REIVINDICA??ES DE QUALQUER FORMA RELATIVA A ESTE SOFTWARE
 * N?O EXCEDER? A QUANTIDADE DE TAXAS, SE HOUVER,
 * QUE VOC? PAGOU DIRETAMENTE ? MICROCHIP POR ESTE SOFTWARE.
 *
 * A MICROCHIP FORNECE ESTE SOFTWARE CONDICIONALMENTE COM SUA ACEITA??O
 * DESTES TERMOS.
?*/

/*
 * File:    config.h
 * Author:  José W R Pereira
 * Comments:Arquivo de configura??o do microcontrolador via diretivas de
 *          compila??o e defini??es (defines).
 * Revision history:
 *      1:  Cria??o do arquivo de configura??o.
 *      2:  Tradu??o dos termos de aceita??o de uso do MPLABX da Microchip;
 *          Coment?rio nas diretivas e defini??es (defines).
 */


#ifndef CONFIG_H
#define CONFIG_H

//#pragma config FOSC  = EXTRC_NOCLKOUT   // INTRC_NOCLKOUT   // Fonte de clock: oscilador interno
//#pragma config WDTE  = OFF              // Desabilita WatchDog Timer
//#pragma config MCLRE = OFF              // Desab. Master Clear via pino
//#pragma config LVP   = OFF              // Desab. grava??o em baixa tens?o
//
//#define _XTAL_FREQ     8000000          // Freq. clock interno: 4MHz(padrao), nao estava funcionando corretamente foi mudado para 8

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#define _XTAL_FREQ     20000000

#endif                                  // alterado tambem para frequencia de clock externa
