MIDI_LIB_PATH = ../../lib/midi/

MIDI_LIB_FLAGS = -I$(MIDI_LIB_PATH)

MIDI_LIB_SOURCEFILES = $(MIDI_LIB_PATH)/midi.c


CONTIKI_PROJECT = print-values midi-rime-bctx midi-rime-bcrx test-uip-bctx test-uip-bcrx test-psock-server udp-client udp-server

PROJECT_SOURCEFILES += uart2.c uart2-midi.c $(MIDI_LIB_SOURCEFILES)

all: $(CONTIKI_PROJECT)

UIP_CONF_IPV6=1

CONTIKI = ../../../../
include $(CONTIKI)/Makefile.include
