CC=emcc
CPPFLAGS=-g -std=c++11
CWD=$(abspath $(lastword $(MAKEFILE_LIST)))
CXX=gcc
EMCC_CFLAGS=-s WASM=1 -Wall -s MODULARIZE=1 -s ERROR_ON_UNDEFINED_SYMBOLS=0 --bind
#OBJS=*.o *.html *.js *.wasm
OBJS=*.o *.wasm
RM=rm -v

.PHONY: all clean test

#compile: TaskRunner.o TaskRunner.js TradeSignalManager.o TradeSignalManager.js
compile: TradeSignalManager.o TradeSignalManager.js

%.js: %.cpp
	$(CC) $(EMCC_CFLAGS) $(CFLAGS) $(CPPFLAGS) $< -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	$(RM) $(OBJS)
