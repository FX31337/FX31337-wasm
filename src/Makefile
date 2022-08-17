CC=gcc
CXX=g++
CPPFLAGS=-g $(shell root-config --cflags)
RM=rm -v

.PHONY: all clean test

compile: Trade.o

%.o: %.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	$(RM) $(OBJS)
