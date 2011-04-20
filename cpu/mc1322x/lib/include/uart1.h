/*
 * Copyright (c) 2010, Mariano Alvira <mar@devl.org> and other contributors
 * to the MC1322x project (http://mc1322x.devl.org)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of libmc1322x: see http://mc1322x.devl.org
 * for details. 
 *
 *
 */

#ifndef UART1_H
#define UART1_H

#include <stdint.h>

#define UCON      (0)
/* UCON bits */
#define UCON_SAMP     10
#define UCON_SAMP_8X   0
#define UCON_SAMP_16X  1

#define USTAT     (0x04)
#define UDATA     (0x08)
#define URXCON    (0x0c)
#define UTXCON    (0x10)
#define UCTS      (0x14)
#define UBRCNT    (0x18)

#define UART1_BASE (0x80005000)
#define UART2_BASE (0x8000b000)

#define UART1_UCON       ((volatile uint32_t *) ( UART1_BASE + UCON   ))
#define UART1_USTAT      ((volatile uint32_t *) ( UART1_BASE + USTAT  ))
#define UART1_UDATA      ((volatile uint32_t *) ( UART1_BASE + UDATA  ))
#define UART1_URXCON     ((volatile uint32_t *) ( UART1_BASE + URXCON ))
#define UART1_UTXCON     ((volatile uint32_t *) ( UART1_BASE + UTXCON ))
#define UART1_UCTS       ((volatile uint32_t *) ( UART1_BASE + UCTS   ))
#define UART1_UBRCNT     ((volatile uint32_t *) ( UART1_BASE + UBRCNT ))

#define UART2_UCON       ((volatile uint32_t *) ( UART2_BASE + UCON   ))
#define UART2_USTAT      ((volatile uint32_t *) ( UART2_BASE + USTAT  ))
#define UART2_UDATA      ((volatile uint32_t *) ( UART2_BASE + UDATA  ))
#define UART2_URXCON     ((volatile uint32_t *) ( UART2_BASE + URXCON ))
#define UART2_UTXCON     ((volatile uint32_t *) ( UART2_BASE + UTXCON ))
#define UART2_UCTS       ((volatile uint32_t *) ( UART2_BASE + UCTS   ))
#define UART2_UBRCNT     ((volatile uint32_t *) ( UART2_BASE + UBRCNT ))


/** \name UART Control Register (UCON) <i>read-write</i>
 * \brief is used to specify transmission parameters, such as
 *        flow control, stop bits, parity, etc.
 * \{
*/
enum {

  LOOPBACK_TEST		= 15,
  RX_READY_MASK		= 14,
  TX_READY_MASK		= 13,
  FLOW_CONTROL_ENABLE	= 12,
  FLOW_CONTROL_POLARITY	= 11,
  OVERSAMPLING		= 10,
  TX_OUTPUT_ENABLE	= 7,
  TX_TEST		= 6,
  SEND_BREAK		= 5,
  STOP_BITS		= 4,
  PARITY_TYPE		= 3,
  PARITY_ENABLE		= 2,
  RX_ENABLE		= 1,
  TX_ENABLE		= 0,

};
/** \} */

/** \name UCON macros
 * \{
*/

#define UART_ON		( ( 1 << TX_ENABLE ) | ( 1 << RX_ENABLE ) )
#define UART_OFF	( ( 0 << TX_ENABLE ) | ( 0 << RX_ENABLE ) )

#define U_TX_ONLY	( ( 1 << TX_ENABLE ) | ( 0 << RX_ENABLE ) )
#define U_RX_ONLY	( ( 0 << TX_ENABLE ) | ( 1 << RX_ENABLE ) )


#define U_PARITY_OFF	( 0 << PARITY_ENABLE )  // default: disabled=0
#define U_PARITY_ON	( 1 << PARITY_ENABLE )	//           enabled=1

#define U_EVEN_PARITY	( 0 << PARITY_TYPE )	// default: even=0
#define U_ODD_PARITY	( 1 << PARITY_TYPE ) 	//           odd=1

#define U_ONE_STOP_BIT	( 0 << STOP_BITS )	// default: 1bit=0
#define U_TWO_STOP_BIT	( 1 << STOP_BITS )	//          2bit=1

#define U_BREAK_OFF	( 0 << SEND_BREAK )	// default: off=0
#define U_BREAK_ON	( 1 << SEND_BREAK )	//           on=1

#define U_FC_RTS_AL	( 0 << FLOW_CONTROL_POLARITY ) // default: active-low=0
#define U_FC_RTS_AH	( 1 << FLOW_CONTROL_POLARITY ) //         active-high=1

#define U_FC_OFF	( 0 << FLOW_CONTROL_ENABLE ) // default: off=0
#define U_FC_ON		( 1 << FLOW_CONTROL_ENABLE ) //           on=1

#define U_TXI_ON	( 0 << TX_READY_MASK )	// default: enabled=0
#define U_TXI_OFF	( 1 << TX_READY_MASK )	//           masked=1

#define U_RXI_ON	( 0 << RX_READY_MASK )	// default: enabled=0
#define U_RXI_OFF	( 1 << RX_READY_MASK )	//           masked=1

#define U_TEST		( 1 << LOOPBACK_TEST )

#ifndef U1_ENABLE_DEFAULT
#  define U1_ENABLE_DEFAULT UART_ON
# else
#  warning "U1_ENABLE_DEFAULT is not default"
#endif

#ifndef U2_ENABLE_DEFAULT
#  define U2_ENABLE_DEFAULT ( U_RX_ONLY | U_TXI_OFF )
# else
#  warning "U2_ENABLE_DEFAULT is not default"
#endif

#define U_EN_FC	( UART_ON | U_FC_ON )

#define U_EN_FCEP ( UART_ON|U_FC_ON|U_PARITY_ON|U_EVEN_PARITY )
#define U_EN_FCOP ( UART_ON|U_FC_ON|U_PARTIY_ON|U_ODD_PARITY )

/** \} */

/** \name UART Status Register (USTAT) <i>read-only</i>
 * \brief indicates interrupt request status and any errors
 *        that have been detected during transmission, such
 *        as FIFO buffer overflow or underflow, parity error,
 *        and frame, start, or stop bit error.
 * \{
*/
enum {

  TX_READY		= 7,
  RX_READY		= 6,
  RX_FIFO_UNDERRUN	= 5,
  RX_FIFO_OVERRUN	= 4,
  TX_FIFO_OVERRUN	= 3,
  STOP_BIT_ERROR	= 2,
  PARITY_ERROR		= 1,
  START_BIT_ERROR	= 0,

};
/** \} */

#ifndef U1_TXFIFO_SIZE
#  define U1_TXFIFO_SIZE 31
# else
#  warning "U1_TXFIFO_SIZE is not default"
#endif
#  ifndef U1_RXFIFO_SIZE
#  define U1_RXFIFO_SIZE 31
# else
#  warning "U1_RXFIFO_SIZE is not default"
#endif

#ifndef U2_TXFIFO_SIZE
#  define U2_TXFIFO_SIZE 31
# else
#  warning "U2_TXFIFO_SIZE is not default"
#  endif
#ifndef U2_RXFIFO_SIZE
#  define U2_RXFIFO_SIZE 31
# else
#  warning "U2_RXFIFO_SIZE is not default"
#endif

/** Circular buffer */
extern volatile uint32_t  u1_head, u1_tail;

void uart1_putc(char c);
#define uart1_can_get() (*UART1_URXCON > 0)
uint8_t uart1_getc(void);

#define uart1_txi_check() bit_is_set(*UART1_USTAT, TX_READY)
#define uart1_rxi_check() bit_is_set(*UART1_USTAT, RX_READY)
extern void uart1_txi_handler(void)  __attribute__((weak));
extern void uart1_rxi_handler(void)  __attribute__((weak));

/** Circular buffer (fallback only) */
extern volatile uint32_t  u2_head, u2_tail;

void uart2_putc(char c);
#define uart2_can_get() (*UART2_URXCON > 0)
uint8_t uart2_getc(void);

#define uart2_txi_check() bit_is_set(*UART2_USTAT, TX_READY)
#define uart2_rxi_check() bit_is_set(*UART2_USTAT, RX_READY)
extern void uart2_txi_handler(void)  __attribute__((weak));
extern void uart2_rxi_handler(void)  __attribute__((weak));


/* This can be done a bit better with switch() */
#define U2_DBG_RX_DATA(x) x##_test = *UART2_USTAT; \
		  printf(":%u (Err: ", *x##_step); \
		  if(x##_test == 0x80) { printf("N"); } else { \
	  if(bit_is_set(x##_test, START_BIT_ERROR))	{ printf("F"); } \
	  if(bit_is_set(x##_test, STOP_BIT_ERROR))	{ printf("S"); } \
	  if(bit_is_set(x##_test, PARITY_ERROR))	{ printf("P"); } \
	  if(bit_is_set(x##_test, RX_FIFO_OVERRUN))	{ printf("O"); } \
	  if(bit_is_set(x##_test, RX_FIFO_UNDERRUN))	{ printf("U"); } \
		  } printf(" =%x)\n", x##_test)


/* Perhaps __BASE_FILE__ could be used for uniq name,
 * but this should be sufficient. */
#define U2_GET_LOOP(x) static char *x##_step = &x; \
	while(*UART2_URXCON != 0) { *(x##_step++) = *UART2_UDATA; }

#define U2_GET_POST(x) static char *x##_step = &x;	\
	if(*UART2_URXCON != 0) { do		\
	{ *(x##_step++) = *UART2_UDATA; }		\
	while(*UART2_URXCON != 0);		\
	process_post(PROCESS_BROADCAST, x##_full, 2); }

/* Note that it will only print proper values of USTAT
 * when called inside of the rxi_handler function. */
#define U2_GET_LOOP_DEBUG(x) static char *x##_step = &x;\
	while(*UART2_URXCON != 0) {			\
		*x##_step = *UART2_UDATA;			\
		U2_DBG_RX_DATA(x); x##_step++; }

#define U2_TXI_POLL_PROCESS(x) void uart2_txi_handler(void){ process_poll(x); }
#define U2_RXI_POLL_PROCESS(x) void uart2_rxi_handler(void){ process_poll(x); }

#endif
