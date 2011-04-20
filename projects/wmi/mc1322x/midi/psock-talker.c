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

/* #define P() printf("line: %d\n", __LINE__) */

/*
 * To be able to handle more than one connection at a time,
 * each parallell connection needs its own protosocket.
 */
static struct psock TCP_thread;
static struct pt    URX_thread;

//static struct pt_sem urxbuf_data;
// may be it could go as struct with pt_sem ?
//static uint8_t urxbuf_mask, urxbuf_test;

struct signal {
  unsigned short size;
  char flag;
};

static struct signal urx;

static unsigned short
URX_proc(void *s)
{
  struct signal *u = (struct signal *)s;

  return u->size;
}

PROCESS(Talker, "MIDI Talker");
AUTOSTART_PROCESSES(&Talker);
U2_RXI_POLL_PROCESS(&Talker);

/*---------------------------------------------------------------------------*/

static data_buffer_t tcpbuf[10]; /*,
	urxbuf[32], urxbuf_step,
	utxbuf[32], utxbuf_step;
	*/

/*---------------------------------------------------------------------------*/
static
PT_THREAD(URX_fill(struct pt *p))
{

  PT_BEGIN(p);

  if ( urx.flag != 0xff ) {
    PT_WAIT_UNTIL(p, urx.flag == 0);
  }

  urx.size = 0;

  printf("UART2_URXCON =%d\n", *UART2_URXCON);

  while(*UART2_URXCON > 0) {

  bcopy(UART2_UDATA, &uip_appdata[urx.size++], 1);

  }

  urx.flag = 1;


  //urxbuf_step = &urxbuf;

    //U2_GET_LOOP_DEBUG(urxbuf);

    /*for(urxbuf_mask = 0; *UART2_URXCON != 0; urxbuf_mask++) {

      *urxbuf_step = *UART2_UDATA;
      U2_DBG_RX_DATA(urxbuf);
      urxbuf_step++;

    }*/

  //PT_SEM_SIGNAL(p, &urxbuf_data);

  PT_END(p);
}

/*---------------------------------------------------------------------------*/
static
PT_THREAD(TCP_send(struct psock *p)) //, volatile process_event_t *ev, volatile data_buffer_t *buf))
{
  PSOCK_BEGIN(p);

  while(1) {

    PT_WAIT_UNTIL(&((p)->pt), urx.flag == 1);

    printf("\nurx.size=%d\n", URX_proc(&urx));

    PSOCK_GENERATOR_SEND(p, URX_proc, &urx);

    urx.flag = 0;

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
  PROCESS_POLLHANDLER(URX_fill(&URX_thread));
  PROCESS_BEGIN();

  midi_uart_init();

  urx.flag = 0xff;

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
    } else { urx.flag = 0xff; }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
