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

enum {

  SKIP = 0xff,

};

#define data_buffer_t char

/* Won't work for some reason!
 * #define URX_proc(x) (urx->size)
*/

#define P() printf("line: %d", __LINE__)

#define STAT 1

#define INFO 1


#   if defined(INFO)
#   define info0(...) printf(__VA_ARGS__)
#   else
#   define info0(...)
#   endif

#   if INFO >= 1
#   define info1(...) printf(__VA_ARGS__)
#   else
#   define info1(...)
#   endif

#   if INFO >= 2
#   define info2(...) printf(__VA_ARGS__)
#   else
#   define info2(...)
#   endif


/*
 * To be able to handle more than one connection at a time,
 * each parallell connection needs its own protosocket.
*/
static struct psock TCP_thread;
static struct pt    URX_thread;

struct signal {
  	struct timer	time;
  	unsigned short	size;
  	unsigned short	flag;
#ifdef STAT
	unsigned long	stat;
#endif

//static struct psock sender; // TCP_thread
//static struct pt    worker; // URX_thread

};

#ifdef STAT
#define incr_stat() do { \
	urx->stat += urx->size; \
	printf("\n%lu\t++stat= %d\n", clock_time(), urx->stat); \
} while(0)

#define decr_stat() do { \
	urx->stat -= urx->size; \
	printf("\n%lu\t--stat= %d\n", clock_time(), urx->stat); \
} while(0)

#define zero_stat() do { \
	urx->stat = 0; \
	printf("\n%lu\t..stat= %d\n", clock_time(), urx->stat); \
} while(0)

#else
#  define zero_stat()
#  define incr_stat()
#  define decr_stat()
#endif


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

static data_buffer_t tcpbuf[32];

/*---------------------------------------------------------------------------*/
static
PT_THREAD(URX_fill(struct pt *p))
{

  PT_BEGIN(p);

  if (urx->flag != SKIP) {
    PT_WAIT_UNTIL(p, (urx->flag == 0 || timer_expired(&urx->time)));
    info2("okay!\n");
  }

  if (urx->stat < 32) {
    urx->size = urx->stat;
  } else {
    urx->size = 0;
  }

  info1("URXCON=%d\n", *UART2_URXCON);

  while(*UART2_URXCON > 0) {
    bcopy(UART2_UDATA, &uip_appdata[urx->size++], 1);
    info1("f=%d; s=%d;\n", urx->flag, urx->size);
  }

  info0("\n<< %d", urx->size);

  incr_stat();

  urx->flag = 1;

  PT_END(p);
}

/*---------------------------------------------------------------------------*/
static
PT_THREAD(TCP_send(struct psock *p))
{
  PSOCK_BEGIN(p);

  zero_stat();

  while(1) {

    PT_WAIT_UNTIL(&((p)->pt), urx->flag == 1);

    info0("\n>> %d", urx->size);

    info1("\ns=%d\n", URX_proc(urx));

    info2("MSS=%d\n", uip_mss());


    PSOCK_GENERATOR_SEND(p, URX_proc, urx);

    timer_reset(&urx->time);

    decr_stat();

    urx->flag = 0;

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

  zero_stat();

  midi_uart_init();
  urx->flag = SKIP;
  timer_set(&urx->time, CLOCK_SECOND * 120);

  /* Is it needed: PT_INIT(&URX_thread); ? */

  tcp_listen(UIP_HTONS(2020));

  info2("Initial MSS=%d\n", uip_initialmss());

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
    } else { urx->flag = SKIP; zero_stat(); timer_reset(&urx->time); info2("skip!\n"); }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
