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

#ifndef pb_data_t
typedef pb_exec_t unsigned int;
#endif

#ifndef pb_call_t
typedef pb_call_t unsigned char;
#endif

#ifndef pb_data_t
typedef pb_data_t long int;
#endif

// volatile /* ? */
struct pb_path_t {
  pb_flag_t x; // execution flag
  pb_call_t c; // command call
  pb_data_t d; // data signal
  // pb_meta_t m; // something ??
};

#define PB_PATH(x) static struct pb_path_t pb_path_##x

#define PB_INIT(x, flag, call, data) 

enum {
  PB_FLAG_ZERO = 0x00,
  PB_FLAG_NEXT = 0x01,
  PB_FLAG_LOCK = 0xff,
};

enum {
  PB_CALL_ZERO = 0x00,
  PB_CALL_PULL = 0x11,
  PB_CALL_PUSH = 0xdd,
  PB_CALL_WIPE = 0xcc,
};

#if 0
enum {
  PB_DATA_ZERO = 0x00,
}
#endif

/**
 * Initialize a semaphore
 *
 * This macro initializes a semaphore with a value for the
 * counter. Internally, the semaphores use an "unsigned int" to
 * represent the counter, and therefore the "count" argument should be
 * within range of an unsigned int.
 *
 * \param s (struct pt_sem *) A pointer to the pt_sem struct
 * representing the semaphore
 *
 * \param c (unsigned int) The initial count of the semaphore.
 * \hideinitializer
 */
#define PB_FALG(p, s, flag) (s)->x = flag
#define PB_CALL(p, s, call) (s)->c = call
#define PB_DATA(p, s, data) 			\
  do {						\
    PT_WAIT_UNTIL((s)->x != PB_FLAG_LOCK);	\
    (s)->d = data;				\
  } while(0)

#define PB_PUSH(s, data)			\
  do {						\
    (s)->c = PB_CALL_PUSH;			\
    (s)->d = data;				\
    (s)->x = PB_FLAG_NEXT;			\
  } while(0)

#define PB_WIPE(s)

/**
 * Wait for a semaphore
 *
 * This macro carries out the "wait" operation on the semaphore. The
 * wait operation causes the protothread to block while the counter is
 * zero. When the counter reaches a value larger than zero, the
 * protothread will continue.
 *
 * \param pt (struct pt *) A pointer to the protothread (struct pt) in
 * which the operation is executed.
 *
 * \param s (struct pt_sem *) A pointer to the pt_sem struct
 * representing the semaphore
 *
 * \hideinitializer
 */
#define PB_WAIT(pt, s)				\
  do {						\
    PT_WAIT_UNTIL(pt, (s)->x > 0);		\
    --(s)->x;					\
  } while(0)

/**
 * Signal a semaphore
 *
 * This macro carries out the "signal" operation on the semaphore. The
 * signal operation increments the counter inside the semaphore, which
 * eventually will cause waiting protothreads to continue executing.
 *
 * \param pt (struct pt *) A pointer to the protothread (struct pt) in
 * which the operation is executed.
 *
 * \param s (struct pt_sem *) A pointer to the pt_sem struct
 * representing the semaphore
 *
 * \hideinitializer
 */
#define PB_INCR(pt, s) ++(s)->x
#define PB_DECR(pt, s) --(s)->x

#endif /* __PB_H__ */

/** @} */
/** @} */
