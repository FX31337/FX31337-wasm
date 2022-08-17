CC=emcc
CXX=g++
CPPFLAGS=-g
RM=rm -v

.PHONY: all clean test

compile: Trade.o

%.o: %.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	$(RM) $(OBJS)
