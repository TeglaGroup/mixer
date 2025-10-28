CC = gcc
AR = ar
CFLAGS = -fPIC -I include `pkg-config --cflags sndfile` `pkg-config --cflags ao`
LDFLAGS =
LIBS = `pkg-config --libs sndfile` `pkg-config --libs ao` -lpthread

OBJS = src/ao-backend.o

.PHONY: all clean
.SUFFIXES: .c .o

all: libmixer.so

libmixer.so: $(OBJS)
	$(CC) $(LDFLAGS) -shared -o $@ $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $@ $<

clean:
	rm -f libmixer.so
