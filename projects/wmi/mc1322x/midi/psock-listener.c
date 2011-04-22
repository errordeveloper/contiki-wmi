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
#define INFO 1
#include "debug.h"

#include "contiki-net.h"

enum {

  SKIP = 0xff,
  FULL = 1,
  FREE = 0,
  STOP = 0xfe,

};

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

//static struct psock sender; // TCP_thread
//static struct pt    worker; // URX_thread

};

//
// Buffer Length (has to be defined)
#define BL 32
// Queue Length (set to zero to disable)
#define QL (BL/2)

void dbg(void) { printf("\nDebug Interrupt\n"); }

/*---------------------------------------------------------------------------*/

volatile struct signal TX;
static struct signal *utx = (struct signal *) &TX;

static data_buffer_t tcpbuf[BL];

static data_buffer_t utxbuf_test[] =  { 0xf1, 0x21, 0x15, 0x61 };

unsigned short n = 0;

/*---------------------------------------------------------------------------*/

PROCESS(Listener, "MIDI Listener");
AUTOSTART_PROCESSES(&Listener);
//U2_TXI_CALL(utx->flag = FREE); // FULL);
U2_TXI_CALL(disable_irq(UART2); utx->flag = FREE); // FULL);
U2_RXI_CALL(U2_TX_ONLY());

/*---------------------------------------------------------------------------*/
static
PT_THREAD(TCP_send(struct psock *p))
{
  PSOCK_BEGIN(p);

  info1("> psock_begin\n");

  while(1) {

    info1("> utx->flag = 0x%x\n", utx->flag);
    PSOCK_WAIT_UNTIL(p, (utx->flag == FREE));
    info1("> utx->flag = 0x%x\n", utx->flag);

    PSOCK_READTO(p, '\n');

    info1("> sizeof(tcpbuf) = %d\n", sizeof(tcpbuf));


    /*for(n = p->msglen; n > 0; n--) {
    	*UART2_UDATA = (0xff|n);
    }*/

    // dbg();


    for(n = sizeof(tcpbuf); n > 0; --n) {
      *UART2_UDATA = tcpbuf[n];
    }


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

  /* The timer is unsed, but it's probably
   * better to initialise it to avoid any
   * misbehaviour (just in case)
  */
  timer_set(&utx->time, CLOCK_SECOND * 120);

  /* Is it needed: PT_INIT(&URX_thread); ? */

  tcp_listen(UIP_HTONS(4040));

  while(1) {

    PROCESS_WAIT_EVENT();
    
    if(ev == tcpip_event) {
	    info1("> tcpip_event\n");
     
      if(uip_connected()) {
        
        PSOCK_INIT(&TCP_thread, tcpbuf, sizeof(tcpbuf));
     
        while(!(uip_aborted() || uip_closed() || uip_timedout())) {
     
	  info1("> uip_connected\n");
          //PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
	  PROCESS_WAIT_EVENT();

	  if (ev == tcpip_event) TCP_send(&TCP_thread);
	  
        }
      }
    } info1("> idle\n");
  }
  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
