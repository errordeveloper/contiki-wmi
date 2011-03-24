/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
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
 * This file is part of the Contiki operating system.
 *
 * $Id: hello-world.c,v 1.1 2006/10/02 21:46:46 adamdunkels Exp $
 */

/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 *         Ilya Dmitrichenko <erroredveloper@gmail.com>
 */

#include "contiki.h"

#include "uart2-midi.h"

#include <stdio.h> /* For printf() on UART1 */

#if 0
void input_dump()
{
  // while(1) uart1_putc(uart2_getc());

  while(1) { midi_uart_init();
	  printf("%d ", uart2_getc()); }

}
#endif

int x, s;

/* This need improvements, because x should be a
 * buffer, the size of which yet to be found */
#define U2_GET_LOOP_BEGIN(x) while(*UART2_URXCON != 0) { x = *UART2_UDATA

/* This can be done a bit better with switch(),
 * also 's' should be uniq ... */
#define U2_DBG_RX_DATA(x) int s = *UART2_USTAT; \
		  printf(":%i (Err: ", x); \
		  if(s == 0x80) { printf("N"); } else { \
	  if(bit_is_set(s, START_BIT_ERROR))	{ printf("F"); } \
	  if(bit_is_set(s, STOP_BIT_ERROR))	{ printf("S"); } \
	  if(bit_is_set(s, PARITY_ERROR))	{ printf("P"); } \
	  if(bit_is_set(s, RX_FIFO_OVERRUN))	{ printf("O"); } \
	  if(bit_is_set(s, RX_FIFO_UNDERRUN))	{ printf("U"); } \
		  } printf(" =%x)\n", s)

#define U2_GET_LOOP_END() }

/*---------------------------------------------------------------------------*/
void 
uart2_rxi_handler(void){

/* Test it here and now! */

U2_GET_LOOP_BEGIN(x);

U2_DBG_RX_DATA(x);

U2_GET_LOOP_END();

}

/*---------------------------------------------------------------------------*/
void 
uart2_txi_handler(void){

#if STILL_TODO

    printf("t");

    *UART2_UDATA = 0x85;

#endif
  
}


/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS(uart2_test, "Testing UART2 in MIDI mode");
AUTOSTART_PROCESSES(&uart2_test);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(uart2_test, ev, data)
{
  PROCESS_BEGIN();

  midi_uart_init();

  // input_dump();

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
