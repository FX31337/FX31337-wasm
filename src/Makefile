CC=emcc
CPPFLAGS=-g -std=c++11
CWD=$(abspath $(lastword $(MAKEFILE_LIST)))
CXX=gcc
EMCC_CFLAGS=-s WASM=1 -s ENVIRONMENT=node -s EXIT_RUNTIME=0 -s NO_EXIT_RUNTIME=1 -s ASSERTIONS=1 -Wall -s MODULARIZE=1 -s ERROR_ON_UNDEFINED_SYMBOLS=0 --bind -s EXPORTED_FUNCTIONS="[]"
#OBJS=*.o *.html *.js *.wasm
OBJS=*.o *.wasm
RM=rm -v

.PHONY: all clean test

#compile: TaskRunner.o TaskRunner.js TradeSignalManager.o TradeSignalManager.js
#compile: TradeSignalManager.o TradeSignalManager.js
compile: TaskRunner.o TaskRunner.js

%.js: %.cpp
	$(CC) $(EMCC_CFLAGS) $(CFLAGS) $(CPPFLAGS) $< -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	$(RM) $(OBJS)
