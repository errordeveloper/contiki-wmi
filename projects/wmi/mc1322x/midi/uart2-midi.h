/* Firsly baud rate has to be set to 31250:
 * use maxima script to solve this:
 * % maxima -b mc1322x-baudrate-solver.mac
 * See Section 13.4.2 of the manual for details
 *
 * For BAUD= 31250, PBUS= 2400000, UMOD= 9999
 * and using 8X oversampling, we get:
 * UINC = 103.15625
 * and using 16X oversampling:
 * UINC = 207.3125
 * but with 1X it will be:
 * UINC = 12.01953125
 *
 * Therefore without oversampling it should be
 * more accurate.
*/

#include "uart1.h"
#define MIDI_UART2_INC 103
#define MIDI_UART2_MOD MOD
#define MIDI_UART2_SAMP 1

/* This is a bit starnge, but it doesn't work
 * if SMAP is 0, which kind of should .. ?
*/

#if 0 /* These functions are unsuitable */

#define midi_uart_init() uart2_init(MIDI_UART2_INC,MIDI_UART2_MOD,MIDI_UART2_SAMP)

#define midi_uart_getc() uart2_getc()
#define midi_uart_putc() uart2_putc()

#else

#define midi_uart_init() uart2_midi_init(MIDI_UART2_INC,MIDI_UART2_MOD,MIDI_UART2_SAMP)

#define midi_uart_getc() uart2_midi_getc()
#define midi_uart_putc() uart2_midi_putc()

#endif
