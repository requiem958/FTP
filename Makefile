.PHONY: all

# Disable implicit rules
.SUFFIXES:

# Keep intermediate files
.PRECIOUS: %.o

CC = clang
CFLAGS = -Wall -Werror -g
LDFLAGS =

# Note: -lnsl does not seem to work on Mac OS but will
# probably be necessary on Solaris for linking network-related functions
#LIBS += -lsocket -lnsl -lrt
LIBS += -lpthread

INCLUDE = csapp.h commands.h
OBJS = csapp.o commands.o
INCLDIR = -I.

PROGS =ftpserver ftpclient ftpslave

all: $(PROGS)

%: %.o $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)


%.o: %.c $(INCLUDE)
	$(CC) $(CFLAGS) $(INCLDIR) -c -o $@ $<


#	$(CC) -o $@ $(LDFLAGS) $(LIBS) $^

ftpserver: ftpserver_utils.o
ftpclient: ftpclient_commands.o
ftpslave: ftpslave_utils.o

ftpserver.c: ftpserver_utils.h
ftpclient.c: ftpclient_commands.h
ftpslave.c: ftpslave_utils.h
clean:
	rm -f $(PROGS) *.o
