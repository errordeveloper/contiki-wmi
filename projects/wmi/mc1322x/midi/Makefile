MIDI_LIB_PATH = ../../lib/midi/

MIDI_LIB_FLAGS = -I$(MIDI_LIB_PATH)

MIDI_LIB_SOURCEFILES = $(MIDI_LIB_PATH)/midi.c


CONTIKI_PROJECT = print-values \
		  midi-rime-bctx \
		  midi-rime-bcrx \
		  test-uip-bctx \
		  test-uip-bcrx \
		  udp-client \
		  udp-server \
		  psock-listener \
		  psock-talker

PROJECT_SOURCEFILES += uart2.c #$(MIDI_LIB_SOURCEFILES) # uart2-midi.c

all: $(CONTIKI_PROJECT)


UIP_CONF_IPV6=1

CONTIKI = ../../../../
include $(CONTIKI)/Makefile.include

router:
	cd $(CONTIKI)/examples/ipv6/rpl-border-router/ \
	&& make border-router.load \
	&& make connect-router-xterm
