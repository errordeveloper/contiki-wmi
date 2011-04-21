/*
 *
 * The Listener only handles one connection at a time.
 *
 */

#include "contiki.h"
#include "sys/ctimer.h"
#include "net/uip.h"
#include "net/uip-ds6.h"
#include "net/uip-udp-packet.h"
#include "sys/ctimer.h"

#include <string.h>

#include <stdio.h>

#include "uart2-midi.h"

#define STAT 0
#define INFO 0
#include "debug.h"

#include "contiki-net.h"


#define data_buffer_t char

/*
 * To be able to handle more than one connection at a time,
 * each parallell connection needs its own protosocket.
 */
static struct psock TCP_thread;
static struct pt    UTX_thread;

struct signal {
	struct timer	time;
	unsigned short	size;
	unsigned short	flag;
#if STAT
	unsigned long	stat;
	unsigned long	sent;
	unsigned long	lost
#endif
};

PROCESS(Listener, "MIDI Listener");
AUTOSTART_PROCESSES(&Listener);
U2_TXI_CALL(utx->flag = 1);

/*---------------------------------------------------------------------------*/

volatile struct signal TX;
static struct signal *utx = (struct signal *) &TX;

static data_buffer_t tcpbuf[BL];

/*---------------------------------------------------------------------------*/
static
PT_THREAD(UTX_fill(struct pt *p))
{

  PT_BEGIN(p);

  PT_END(p);
}

/*---------------------------------------------------------------------------*/
static
PT_THREAD(TCP_send(struct psock *p))
{
  PSOCK_BEGIN(p);

  while(1) {

    PSOCK_WAIT_UNTIL(p, (utx->flag = 0));

    //PSOCK_READTO(p, '\n');

  }
  
  PSOCK_CLOSE(p);

  PSOCK_END(p);
}


/*---------------------------------------------------------------------------*/
PROCESS_THREAD(Listener, ev, data)
{
  PROCESS_BEGIN();

  stat_init(utx);

  midi_uart_init();
  timer_set(&utx->time, CLOCK_SECOND * 120);

  /* Is it needed: PT_INIT(&URX_thread); ? */

  tcp_listen(UIP_HTONS(4040));

  while(1) {

    PROCESS_WAIT_EVENT();
    
    if(ev == tcpip_event) {
     
      if(uip_connected()) {
        
        PSOCK_INIT(&TCP_thread, tcpbuf, sizeof(tcpbuf));
     
        while(!(uip_aborted() || uip_closed() || uip_timedout())) {
     
          //PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
	  PROCESS_WAIT_EVENT();

	  if (ev == tcpip_event) TCP_send(&TCP_thread);
	  
        }
      }
    }
  }  
  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
