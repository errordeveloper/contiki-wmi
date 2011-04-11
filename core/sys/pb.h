/*
 * Copyright (c) 2004, Swedish Institute of Computer Science.
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
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id: pt-sem.h,v 1.2 2008/10/14 12:46:39 nvt-se Exp $
 */

/**
 * \addtogroup pt
 * @{
 */

/**
 * \defgroup pb Proto Bus
 * @{
 *
 * This module implements counting semaphores on top of
 * protothreads. Semaphores are a synchronization primitive that
 * provide two operations: "wait" and "signal". The "wait" operation
 * checks the semaphore counter and blocks the thread if the counter
 * is zero. The "signal" operation increases the semaphore counter but
 * does not block. If another thread has blocked waiting for the
 * semaphore that is signaled, the blocked thread will become
 * runnable again.
 *
 * Semaphores can be used to implement other, more structured,
 * synchronization primitives such as monitors and message
 * queues/bounded buffers (see below).
 */
   
/**
 * \file
 * Counting semaphores implemented on protothreads
 * \author
 * Adam Dunkels <adam@sics.se>
 *
 */

#ifndef __PB_H__
#define __PB_H__

#include "sys/pt.h"

#ifndef pb_flag_t
typedef pb_flag_t unsigned char;
#endif

#ifndef pb_call_t
typedef pb_call_t unsigned char;
#endif

#ifndef pb_data_t
typedef pb_data_t long int;
#endif

typedef pb_size_t unsigned short int;

// volatile /* ? */
struct pb_path_t {
  pb_flag_t x; // execution flag
  pb_call_t c; // command call
  pb_data_t d; // data
  pb_size_t s; // size
  // pb_meta_t m; // something ??
};

#define PB_PATH(x) static struct pb_path_t pb_path_##x

#define PB_INIT(x, flag, call, data)

enum {
  // PB_FLAG_CALL = 0x22,
  // PB_FLAG_DATA = 0x44,
  // PB_FLAG_DONE = 0xdd,
  PB_FLAG_ZERO = 0x00,
  PB_FLAG_NEXT = 0x01,
  PB_FLAG_LOCK = 0xff,
  PB_FLAG_FREE = 0xee,
};

enum {
  PB_CALL_ZERO = 0x00,
  PB_CALL_PULL = 0x11,
  PB_CALL_DATA = 0x44,
  PB_CALL_WIPE = 0xcc,
  PB_CALL_DONE = 0xdd,
  PB_CALL_PUSH = 0xff,
};

#if 0
enum {
  PB_DATA_ZERO = 0x00,
}
#endif

#define PB_INCR(v) ++(v)->x
#define PB_DECR(v) --(v)->x

/* Set protobus flag to a specific value */
#define PB_FLAG(v) (v)->x

/* Store a command for execution */
#define PB_CALL(v) (v)->c
#define PB_DONE(v) PB_CALL(v) = PB_CALL_DONE

/* Instantiate a lock on protobus */
#define PB_LOCK(v) PB_FLAG(v) = PB_FLAG_LOCK
/* Release the lock from protobus */
#define PB_FREE(v) PB_FLAG(v) = PB_FLAG_FREE

/* Retrun data */
#define PB_DATA(v) (v)->d

/* Return size */
#define PB_SIZE(v) (v)->s

#define PB_WAIT(pt, v, y, x)			\
  PT_WAIT_UNTIL(pt, y(v) == y##x)

/* Send data when the lock is free */
#define PB_SEND(pt, v, data, size)		\
  do {						\
    PB_WAIT(pt, v, PB_FLAG, FREE);			\
    PB_DATA(v) = data;				\
    PB_SIZE(v) = size;				\
    PB_FLAG(v) = PB_FLAG_DATA;			\
  } while(0)

/* Same as PB_SEND() but ignore the lock and
 * notify the reciever about this fact. */
#define PB_PUSH(v, data, size)			\
  do {						\
    PB_DATA(v) = data;				\
    PB_SIZE(v) = size;				\
    PB_CALL(v) = PB_CALL_PUSH;			\
    PB_FLAG(v) = PB_FLAG_DATA;			\
  } while(0)

// #define PB_WIPE(v)


/* Example:
 *
 * producer loop:
 * 		PB_SEND(pt, pbus, data, size);
 * 		PB_WAIT(pbus, PB_CALL, DONE);
 *
 * consumer loop:
 * 		PB_WAIT(pt, pbus, PB_FLAG, CALL);
 * 		if(PB_CALL(pbus) == PB_CALL_DATA)
 *		{
 *			PB_LOCK(pbus);
 * 			data = PB_DATA(pbus);
 * 			PB_CALL(pbus) = PB_CALL_DONE;
 * 			PB_FREE(pbus);
 * 		} else { other_task(); }
 *
 */

/* READ() should be a function really */
// #define PB_READ(v)

#endif /* __PB_H__ */

/** @} */
/** @} */
