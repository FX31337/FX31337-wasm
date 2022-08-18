CC=emcc
CXX=gcc
CPPFLAGS=-g -std=c++11
EMCC_CFLAGS=-s ERROR_ON_UNDEFINED_SYMBOLS=0
OBJS=*.o *.html *.js *.wasm
RM=rm -v

.PHONY: all clean test

compile: TaskRunner.o TaskRunner.html TradeSignalManager.o TradeSignalManager.html

%.html: %.cpp
	$(CC) $(EMCC_CFLAGS) $(CFLAGS) $(CPPFLAGS) $< -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	$(RM) $(OBJS)
