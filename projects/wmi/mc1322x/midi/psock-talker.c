/*
 *
 * The Talker only handles one connection at a time.
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
#define INFO 2
#include "debug.h"

#include "contiki-net.h"

enum {

  SKIP = 0xff,

};

#define data_buffer_t char

/*
 * To be able to handle more than one connection at a time,
 * each parallell connection needs its own protosocket.
*/
static struct psock TCP_thread;
static struct pt    URX_thread;

/* This a nice struct that
 * may need to be changed,
 * the timer is not used.
*/
struct signal {
  	struct timer	time;
  	unsigned short	size;
  	unsigned short	flag;
#if STAT
	unsigned long	stat;
	unsigned long	sent;
	unsigned long	lost
#endif

//static struct psock sender; // TCP_thread
//static struct pt    worker; // URX_thread

};


// Buffer Length (has to be defined)
#define BL 32
// Queue Length (set to zero to disable)
#define QL (BL/2)

/* This function is needed for PSOCK_GENERATOR_SEND() */
static unsigned short
URX_proc(void *x)
{
  return ((struct signal *)x)->size;
}


PROCESS(Talker, "MIDI Talker");
AUTOSTART_PROCESSES(&Talker);
U2_RXI_POLL_PROCESS(&Talker);

/*---------------------------------------------------------------------------*/

volatile struct signal RX;
static struct signal *urx = (struct signal *) &RX;

static data_buffer_t tcpbuf[BL];

/*---------------------------------------------------------------------------*/
static
PT_THREAD(URX_fill(struct pt *p))
{

  PT_BEGIN(p);

  if (urx->flag != SKIP) {
    PT_WAIT_UNTIL(p, (urx->flag == 0));
    info2("okay!\n");
  }

#if QL
  if (urx->size < QL) {
    urx->size += urx->size;
    norm_stat(urx);
  } else {
    stat_lost(uxr);
    info2("drop!\n");
    urx->size = 0;
  }
#else
  // may be no need for #if/#else
  // compiler can figure this ?
  stat_lost(urx);
  urx->size = 0;
#endif

  info1("URXCON=%d\n", *UART2_URXCON);

  while(*UART2_URXCON > 0) {
    bcopy(UART2_UDATA, &uip_appdata[urx->size++], 1);
    info1("f=%d; s=%d;\n", urx->flag, urx->size);
  }

  info0("<<+ %d\n", urx->size);

  incr_stat(urx);
  
  if(uip_conn != NULL) {
    info2("poll!\n");
    tcpip_poll_tcp(uip_conn);
  } else { info2("null!\n"); }

  urx->flag = 1;

  PT_END(p);
}

/*---------------------------------------------------------------------------*/
static
PT_THREAD(TCP_send(struct psock *p))
{
  PSOCK_BEGIN(p);

  while(1) {

    PSOCK_WAIT_UNTIL(p, (urx->flag == 1));

    info0("+>> %d\n", urx->size);

    info1("s=%d\n", URX_proc(urx));

    // info2("MSS=%d\n", uip_mss());

    PSOCK_GENERATOR_SEND(p, URX_proc, urx);

    timer_reset(&urx->time);

    decr_stat(urx);
    stat_sent(urx);

    urx->size = 0;
    urx->flag = 0;

  }
  
  PSOCK_CLOSE(p);

  PSOCK_END(p);
}


/*---------------------------------------------------------------------------*/
PROCESS_THREAD(Talker, ev, data)
{

  PROCESS_POLLHANDLER(URX_fill(&URX_thread));
  PROCESS_BEGIN();

  stat_init(urx);

  midi_uart_init();
  urx->flag = SKIP;
  timer_set(&urx->time, CLOCK_SECOND * 120);

  /* Is it needed: PT_INIT(&URX_thread); ? */

  tcp_listen(UIP_HTONS(2020));

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
    } else { urx->flag = SKIP; zero_stat(urx); timer_reset(&urx->time); info2("skip!\n"); }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
