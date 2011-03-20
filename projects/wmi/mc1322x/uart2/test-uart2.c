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

#include "uart1.h"

#include <stdio.h> /* For printf() */

#define LOOPBACK_TEST 1
void loopback_test()
{
  uint8_t test_char = 255;

  while(test_char--) {
    printf("put: %d\n", test_char);
    uart2_putc(test_char);
    printf("get: %d\n", uart2_getc());
  }
}

#define INPUT_DUMP 2
void input_dump()
{
  while(1) uart1_putc(uart2_getc());
}


#ifndef UART2_TEST 
#define UART2_TEST LOOPBACK_TEST
#warning "Using default function: loopback_test()"
#endif

/*---------------------------------------------------------------------------*/
PROCESS(uart2_test, "Testing UART2 with loopback");
AUTOSTART_PROCESSES(&uart2_test);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(uart2_test, ev, data)
{
  PROCESS_BEGIN();

  uart2_init(INC,MOD,SAMP);

#if UART2_TEST == LOOPBACK_TEST
  loopback_test();
#elif UART2_TEST == INPUT_DUMP
  input_dump();
#endif

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
