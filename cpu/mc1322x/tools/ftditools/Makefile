INSTALL= /usr/local/bin

################

LDFLAGS = `libftdi-config --libs`

TARGETS = bbmc

CFLAGS = -Wall -Wextra `libftdi-config --cflags`
DEBUG_CLFLAGS = -Werror

all: $(TARGETS)

clean:
	-rm -f $(TARGETS)

install: all
	cp bbmc $(INSTALL)
