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

typedef char data_buffer_t, *pb_data_t;

#include "sys/pb.h"

#include <string.h>

#include <stdio.h>

#include "uart2-midi.h"


#include "contiki-net.h"

#define MIDI_DATA_SIZE 0 // 1024


/* #define P() printf("line: %d\n", __LINE__) */

/*
 * To be able to handle more than one connection at a time,
 * each parallell connection needs its own protosocket.
 */
static struct psock TCP_thread;
static struct pt    URX_thread;

static struct pb_path_t urx;

// may be it could go as struct with pt_sem ?
static uint8_t urxbuf_mask, urxbuf_test;

PROCESS(Talker, "MIDI Talker");
AUTOSTART_PROCESSES(&Talker);
U2_RXI_POLL_PROCESS(&Talker);

/*---------------------------------------------------------------------------*/

static data_buffer_t tcpbuf[10],
	urxbuf[32], *urxbuf_step,
	utxbuf[32], *utxbuf_step;

#define PB_CALL_SKIP PB_CALL_DONE
/*---------------------------------------------------------------------------*/
/* A breakpoint trick for gdb */
/* void bug(void) {} */
/*---------------------------------------------------------------------------*/

static
PT_THREAD(URX_fill(struct pt *p))
{
  PT_BEGIN(p);

  urxbuf_step = &urxbuf;

  //PB_WAIT(p, &urx, PB_CALL, DONE);
    //U2_GET_LOOP_DEBUG(urxbuf);

    for(urxbuf_mask = 0; *UART2_URXCON != 0; urxbuf_mask++) {

      *urxbuf_step = *UART2_UDATA;
      U2_DBG_RX_DATA(urxbuf);
      urxbuf_step++;

    }

  if((PB_CALL(&urx) != PB_CALL_ZERO) 
  && (PB_FLAG(&urx) != PB_FLAG_FREE)) {
    PB_WAIT(p, &urx, PB_CALL, DONE);
  }

  PB_SEND(p, &urx, &urxbuf, urxbuf_mask);

  PT_END(p);
}

static unsigned short
URX_proc(void *data)
{
  struct pb_path_t *d = (struct pb_path_t *)data;

  printf("\nurxbuf_mask=%d", PB_SIZE(d));

  memcpy(uip_appdata, *PB_DATA(d), PB_SIZE(d));

  return PB_SIZE(d);
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(TCP_send(struct psock *p)) //, volatile process_event_t *ev, volatile data_buffer_t *buf))
{
  PSOCK_BEGIN(p);

  while(1) {


    PB_WAIT(&((p)->pt), &urx, PB_CALL, DATA);
    PB_LOCK(&urx);

    //PSOCK_SEND(p, PB_DATA(&urx), PB_SIZE(&urx));
    PSOCK_GENERATOR_SEND(p, URX_proc, &urx);

    // TODO: send the data instead!
    //PSOCK_SEND_STR(p, "UART2 data is ready.");
    PB_CALL(&urx) = PB_CALL_DONE;
    PB_FREE(&urx);

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

  PB_INIT(&urx);

  midi_uart_init();

  /* Is it needed: PT_INIT(&URX_thread); ? */

  tcp_listen(UIP_HTONS(1010));

  while(1) {

    PROCESS_WAIT_EVENT();
    
    if(ev == tcpip_event) {
     
      if(uip_connected()) {
        
        PSOCK_INIT(&TCP_thread, tcpbuf, sizeof(tcpbuf));
     
        while(!(uip_aborted() || uip_closed() || uip_timedout())) {
     
	  PROCESS_WAIT_EVENT();

	  if (ev == tcpip_event) { TCP_send(&TCP_thread); }
	  
        }
      }
    } else { PB_CALL(&urx) = PB_CALL_SKIP; PB_FREE(&urx); }
  }  
  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
