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
 *         A test application for I2C bus on MC1322x devices
 * \author
 *         Adam Dunkels <adam@sics.se>
 *         Ilya Dmitrichenko <erroredveloper@gmail.com>
 */

#include "contiki.h"

#include "i2c.h"

#include "dev/adxl345-sensor.h"
#include "dev/tmp102-sensor.h"

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(i2c_test, "I2C bus test process");
AUTOSTART_PROCESSES(&i2c_test);
/*---------------------------------------------------------------------------*/

#define TEST_BUFFER_SIZE 4

#define ADXL345_DEFAULT_CONFIG 0x00

/* Array of configureatio register values */
static uint8_t slave_conf[] = { ADXL345_DEFAULT_CONFIG };

/* Buffre for received data */
static uint8_t slave_data[TEST_BUFFER_SIZE];

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(i2c_test, ev, data)
{
  PROCESS_BEGIN();

  i2c_enable();

  printf("+++ I2C bus is enabled.\n");

  /* transmit the register to start reading from */
  //i2c_transmitinit(ADXL345_ADDR, c.tx_ctr, c.tx_buf);
  i2c_transmitinit(ADXL345_ADDR, 1, &slave_conf);
  while (!i2c_transferred()); // wait for data to arrive
  printf("+++ I2C TX initialisation completed.\n");

  /* receive the data */
  //i2c_receiveinit(ADXL345_ADDR, c.rx_ctr, c.rx_buf);
  i2c_receiveinit(ADXL345_ADDR, TEST_BUFFER_SIZE, slave_data);
  while (!i2c_transferred()); // wait for data to arrive
  printf("+++ I2C RX initialisation completed.\n");
  //while(1) {
  if(i2c_busy()) printf("+++ I2C bus is busy.\n");
  //}

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
