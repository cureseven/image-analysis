#	 ↓コンパイルしたいソースファイルを書き並べて下さい。
SRCS	= sample.c mv.c
PROGS	= $(SRCS:.c=)

# common (*.o)
OBJS = pnmerr.o pnmio.o
LDLIBS	= $(OBJS) -lm

# C (*.c)
CC	= gcc
CFLAGS	= -g -Wall
CPPFLAGS= -I.

# C++ (*.cc)
CXX	= g++
CXXFLAGS	= -g -Wall

RM	= rm -f

### rules ###

.SUFFIXES:
.SUFFIXES: .o .c .cc

all:  $(PROGS)

$(PROGS): Makefile $(OBJS)

.c:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDLIBS) $< -o $@
.cc:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS) $< -o $@

pnmerr.o: pnmerr.c pnmimg.h
pnmio.o: pnmio.c pnmimg.h

clean:
	$(RM) $(PROGS) *.o

realclean:
	$(RM) $(PROGS) *.o
	$(RM) core gmon.out #*# *~
