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

#include "sys/pt-sem.h"

#include <string.h>

#include <stdio.h>

#include "uart2-midi.h"


#include "contiki-net.h"

#define MIDI_DATA_SIZE 0 // 1024

#define data_buffer_t char

/* Won't work for some reason!
 * #define URX_proc(x) (urx->size)
*/

#define P() printf("line: %d", __LINE__)

/*
 * To be able to handle more than one connection at a time,
 * each parallell connection needs its own protosocket.
*/
static struct psock TCP_thread;
static struct pt    URX_thread;

struct signal {
  struct timer time;
  unsigned short size;
  unsigned short flag;
};

static unsigned short
URX_proc(void *u)
{
  struct signal *urx = (struct signal *) u;

  return (unsigned short) &(urx)->size;
}


PROCESS(Talker, "MIDI Talker");
AUTOSTART_PROCESSES(&Talker);
U2_RXI_POLL_PROCESS(&Talker);

/*---------------------------------------------------------------------------*/

volatile struct signal RX;
static struct signal *urx = (struct signal *) &RX;

static data_buffer_t tcpbuf[10];

/*---------------------------------------------------------------------------*/
static
PT_THREAD(URX_fill(struct pt *p)) //, void *u))
{

  PT_BEGIN(p);

  //struct signal *urx = (struct signal *) u;

  if ( urx->flag != 0xff ) {
    PT_WAIT_UNTIL(p, (urx->flag == 0 || timer_expired(&urx->time)));
  }

  urx->size = 0;

  printf("URXCON=%d\n", *UART2_URXCON);

  while(*UART2_URXCON > 0) {
    bcopy(UART2_UDATA, &uip_appdata[urx->size++], 1);
    printf("f=%d; s=%d;\n", urx->flag, urx->size);
  }

  urx->flag = 1;
  P(); printf("\tf=%d\n", urx->flag);

  PT_END(p);
}

/*---------------------------------------------------------------------------*/
static
PT_THREAD(TCP_send(struct psock *p)) //, void *u))
{
  PSOCK_BEGIN(p);

  //struct signal *urx = (struct signal *) u;

  while(1) {

    P(); printf("\tf=%d\n", urx->flag);
    PT_WAIT_UNTIL(&((p)->pt), urx->flag == 1);

    printf("\ns=%d\n", URX_proc(NULL));

    PSOCK_GENERATOR_SEND(p, URX_proc, NULL);

    timer_reset(&urx->time);
    urx->flag = 0;
    P(); printf("\tf=%d\n", urx->flag);

  }
  
  /*
   * Next, we use the PSOCK_READTO() function to read incoming data
   * from the TCP connection until we get a newline character. The
   * number of bytes that we actually keep is dependant of the length
   * of the input buffer that we use. Since we only have a 10 byte
   * buffer here (the buffer[] array), we can only remember the first
   * 10 bytes received. The rest of the line up to the newline simply
   * is discarded.
   */
   //PSOCK_READTO(p, '\n');
  
  /*
   * And we send back the contents of the buffer. The PSOCK_DATALEN()
   * function provides us with the length of the data that we've
   * received. Note that this length will not be longer than the input
   * buffer we're using.
   */
   //PSOCK_SEND_STR(p, "Got the following data: ");
   //PSOCK_SEND(p, tcpbuf, PSOCK_DATALEN(p));
   //PSOCK_SEND_STR(p, "Good bye!\r\n");

  PSOCK_CLOSE(p);

  PSOCK_END(p);
}


/*---------------------------------------------------------------------------*/
PROCESS_THREAD(Talker, ev, data)
{

  PROCESS_POLLHANDLER(URX_fill(&URX_thread)); //, &urx));
  PROCESS_BEGIN();


  midi_uart_init();

  urx->flag = 0xff;

  timer_set(&urx->time, CLOCK_SECOND * 5);

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

	  if (ev == tcpip_event) TCP_send(&TCP_thread); //, &urx);
	  
        }
      }
    } else { urx->flag = 0xff; timer_reset(&urx->time); }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
