CC=emcc
CXX=g++
CPPFLAGS=-g
OBJS=*.o *.html *.js *.wasm
RM=rm -v

.PHONY: all clean test

compile: Trade.o Trade.html

%.html: %.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	$(RM) $(OBJS)
