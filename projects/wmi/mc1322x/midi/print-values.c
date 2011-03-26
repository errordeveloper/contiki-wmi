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

char urxbuf[32], utxbuf[32], status;

char uart_init_done = 0;

/*---------------------------------------------------------------------------*/
PROCESS(uart2_test, "Testing UART2 in MIDI mode");
AUTOSTART_PROCESSES(&uart2_test);
U2_RXI_POLL_PROCESS(&uart2_test);
/*---------------------------------------------------------------------------*/
void 
uart2_txi_handler(void){

#if STILL_TODO

    printf("t");

    //*UART2_UDATA = 0x85;

#endif
  
}


/*---------------------------------------------------------------------------*/
PROCESS_THREAD(uart2_test, ev, data)
{
  PROCESS_BEGIN();

#ifndef APPCONF_REDUNDUNT_CHECK
  if(!uart_init_done){
    midi_uart_init();
    uart_init_done=1;
  } for( ; ; ) {
#else
  for( midi_uart_init(); ; ) {
#endif


    #if 1

    /* To see proper value of
     * USTAT this should run
     * in the interrupt context.
     */
    U2_GET_LOOP_DEBUG(urxbuf);

    #else

    U2_GET_LOOP(urxbuf);

    #endif

  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
