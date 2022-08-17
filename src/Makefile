CC=emcc
CXX=g++
CPPFLAGS=-g
RM=rm -v

.PHONY: all clean test

compile: Trade.o

%.o: %.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	$(RM) *.o
