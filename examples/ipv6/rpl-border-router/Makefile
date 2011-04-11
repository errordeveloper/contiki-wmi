all: border-router

CONTIKI=../../..

WITH_UIP6=1
UIP_CONF_IPV6=1

#Override inclusion of internal webserver with make WITH_WEBSERVER=0
WITH_WEBSERVER=1

ifeq ($(WITH_WEBSERVER), 0)
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
PROJECT_SOURCEFILES += slip-bridge.c
else
APPS += webserver
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\" -DWEBSERVER=1
PROJECT_SOURCEFILES += slip-bridge.c httpd-simple.c
endif

include $(CONTIKI)/Makefile.include

# This is necessary on some systems
# and it will not break on other.
P = env PATH="/sbin:/usr/sbin/:/usr/local/sbin/:$(PATH)"

XTERM ?= urxvt # $(TERM) is not always the same as command

$(CONTIKI)/tools/tunslip6:	$(CONTIKI)/tools/tunslip6.c
	(cd $(CONTIKI)/tools && $(MAKE) tunslip6)

#ifdef UDEV_UART
#	ARG_UART = -s $(UDEV_UART)
#endif

connect-router:	$(CONTIKI)/tools/tunslip6
	sudo $(P) \
	$(CONTIKI)/tools/tunslip6 -L \
	-s `readlink -e $(UDEV_UART)` aaaa::1/64
	# readlink -e is needed because it segafaults
	# due to unexpected long name

connect-router-xterm:	$(CONTIKI)/tools/tunslip6
	$(XTERM) -title "Contiki RPL border router log" \
		-e sudo $(P) \
		$(CONTIKI)/tools/tunslip6 -L \
		-s `readlink -e $(UDEV_UART)` aaaa::1/64

#TODO: add connect-router-xscreen here!
#	see TODO in Makefile.mc1322x :)
#	we will need Makefile.utils

connect-router-cooja:	$(CONTIKI)/tools/tunslip6
	sudo $(P) $(CONTIKI)/tools/tunslip6 $(ARG_UART) -a localhost aaaa::1/64
