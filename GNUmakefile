prog=wam

CC=gcc
CXX=g++
CPPFLAGS=-Ihw1
CFLAGS=-pthread -MMD -MP
CXXFLAGS=-pthread -MMD -MP
LDFLAGS=-pthread
LDLIBS=-lX11 -lfltk

objects=main.o lawn.o mole.o lawnimp.o hw1/deq.o

all: $(prog)

$(prog): $(objects)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o: %.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

hw1/deq.o: hw1/deq.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

-include $(objects:.o=.d)

clean:
	rm -f $(prog) $(objects) $(objects:.o=.d)

.PHONY: all clean
