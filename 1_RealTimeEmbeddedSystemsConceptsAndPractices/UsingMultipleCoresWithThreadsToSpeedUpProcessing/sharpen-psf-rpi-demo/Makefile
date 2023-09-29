#CFLAGS= -O0 $(INCLUDE_DIRS) $(CDEFS)": This line sets the CFLAGS variable to include no optimization (-O0), and to include any directories specified in the INCLUDE_DIRS variable and any definitions specified in the CDEFS variable.
#CFLAGS= -O0 -msse3 -malign-double $(INCLUDE_DIRS) $(CDEFS)": This line sets the CFLAGS variable to include no optimization (-O0), to enable the use of SSE3 instructions (-msse3), and to align doubles on 16-byte boundaries (-malign-double). It also includes any directories specified in the INCLUDE_DIRS variable and any definitions specified in the CDEFS variable.
#CFLAGS= -O2 -msse3 -malign-double $(INCLUDE_DIRS) $(CDEFS)": This line sets the CFLAGS variable to include optimization level 2 (-O2), to enable the use of SSE3 instructions (-msse3), and to align doubles on 16-byte boundaries (-malign-double). It also includes any directories specified in the INCLUDE_DIRS variable and any definitions specified in the CDEFS variable.
#CFLAGS= -O3 -msse3 $(INCLUDE_DIRS) $(CDEFS)": This line sets the CFLAGS variable to include optimization level 3 (-O3), to enable the use of SSE3 instructions (-msse3), and to include any directories specified in the INCLUDE_DIRS variable and any definitions specified in the CDEFS variable.
#CFLAGS= -O0 $(INCLUDE_DIRS) $(CDEFS)": This line sets the CFLAGS variable to include no optimization (-O0), and to include any directories specified in the INCLUDE_DIRS variable and any definitions specified in the CDEFS variable.
#CFLAGS= -O3 -march=i686 $(INCLUDE_DIRS) $(CDEFS)": This line sets the CFLAGS variable to include optimization level 3 (-O3), and to optimize the code for the i686 architecture (-march=i686). It also includes any directories specified in the INCLUDE_DIRS variable and any definitions specified in the CDEFS variable.
#The CFLAGS variable is used to specify the compiler flags for the C compiler. These flags can be used to optimize the code, specify the target architecture, and include any necessary directories or definitions.


INCLUDE_DIRS = 
LIB_DIRS = 
CC = gcc

CDEFS=
#CFLAGS= -O0 $(INCLUDE_DIRS) $(CDEFS)
#CFLAGS= -O0 -msse3 -malign-double $(INCLUDE_DIRS) $(CDEFS)
#CFLAGS= -O2 -msse3 -malign-double $(INCLUDE_DIRS) $(CDEFS)
#CFLAGS= -O3 -msse3 $(INCLUDE_DIRS) $(CDEFS)
#CFLAGS= -O0 $(INCLUDE_DIRS) $(CDEFS) #lento
CFLAGS= -O3 -march=i686 $(INCLUDE_DIRS) $(CDEFS) #rapido optimizado para el priocesador i686
LIBS=-lpthread

PRODUCT=sharpen_grid sharpen

HFILES= 
CFILES= sharpen_grid.c sharpen.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	${PRODUCT}

clean:
	-rm -f *.o *.NEW *~
	-rm -f ${PRODUCT} ${DERIVED} ${GARBAGE}

sharpen_grid:	sharpen_grid.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ sharpen_grid.o $(LIBS)

sharpen:	sharpen.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ sharpen.o $(LIBS)

depend:

.c.o:
	$(CC) $(CFLAGS) -c $<
