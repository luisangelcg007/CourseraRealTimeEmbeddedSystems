INCLUDE_DIRS = 
LIB_DIRS = 
CC=gcc

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= 

HFILES= source/globalDefine.h
CFILES= source/sequencerGenerator.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	sequencerGenerator

clean:
	-rm -f *.o *.d
	-rm -f sequencerGenerator

sequencerGenerator: $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lsequencerGenerator

depend:

.c.o:
	$(CC) $(CFLAGS) -c $<
