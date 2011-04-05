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


#include "contiki-net.h"

#define MIDI_DATA_SIZE 0 // 1024
#define data_buffer_t char

#define P() printf("line: %d\n", __LINE__)
/*
 * To be able to handle more than one connection at a time,
 * each parallell connection needs its own protosocket.
 */
static struct psock TCP_thread;
static struct pt    URX_thread;

PROCESS(Talker, "MIDI Talker");
AUTOSTART_PROCESSES(&Talker);
U2_RXI_POLL_PROCESS(&Talker);

/*---------------------------------------------------------------------------*/

static data_buffer_t tcpbuf[10], urxbuf[32], utxbuf[32];
char status;

static process_event_t urxbuf_full;

/*---------------------------------------------------------------------------*/
static
PT_THREAD(URX_fill(struct pt *u, process_event_t ev, data_buffer_t *buf))
{

  PT_BEGIN(u);

  while(1) { //P(); //PT_YIELD_UNTIL(u, ev == PROCESS_EVENT_POLL);

    U2_GET_LOOP_DEBUG(urxbuf);

    process_post(PROCESS_BROADCAST, urxbuf_full, 2);

    PT_YIELD(u);

  }

  PT_END(u);

}

//static void URX_fill(void) { U2_GET_LOOP_DEBUG(urxbuf); return; }

/*---------------------------------------------------------------------------*/
static
PT_THREAD(TCP_send(struct psock *p, process_event_t ev, data_buffer_t *buf))
{
  PSOCK_BEGIN(p);

   while(1) { P(); PSOCK_WAIT_UNTIL(p, ev == urxbuf_full);

    //PSOCK_SEND(p, urxbuf, 32);

    P(); PSOCK_SEND_STR(p, "UART2 data is ready.\n");

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
  // PSOCK_READTO(p, '\n');
  
  /*
   * And we send back the contents of the buffer. The PSOCK_DATALEN()
   * function provides us with the length of the data that we've
   * received. Note that this length will not be longer than the input
   * buffer we're using.
   */
  // PSOCK_SEND_STR(p, "Got the following data: ");
  // PSOCK_SEND(p, tcpbuf, PSOCK_DATALEN(p));
  // PSOCK_SEND_STR(p, "Good bye!\r\n");

  PSOCK_CLOSE(p);

  PSOCK_END(p);
}


/*---------------------------------------------------------------------------*/
PROCESS_THREAD(Talker, ev, data)
{
  //PROCESS_POLLHANDLER(URX_fill());
  //PROCESS_POLLHANDLER(URX_fill(&URX_thread, ev, &urxbuf));
  PROCESS_BEGIN();

  midi_uart_init(); P();

  urxbuf_full = process_alloc_event();

  //PT_INIT(&URX_thread);

  tcp_listen(UIP_HTONS(1010));

  while(1) {

    PROCESS_WAIT_EVENT(); P();
    
    if(ev ==  PROCESS_EVENT_POLL) {

      //printf("POLLED\n");
      P(); URX_fill(&URX_thread, ev, &urxbuf);

      //while(*UART2_URXCON != 0); { urxbuf[0] = *UART2_UDATA; }

    } else if(ev == tcpip_event) {
     
      if(uip_connected()) {
        
        P(); PSOCK_INIT(&TCP_thread, tcpbuf, sizeof(tcpbuf));
     
        while(!(uip_aborted() || uip_closed() || uip_timedout())) {
     
          PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event); // || ev == urxbuf_full);
     
          TCP_send(&TCP_thread, ev, urxbuf);
        }
      }
    }
  }  
  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
