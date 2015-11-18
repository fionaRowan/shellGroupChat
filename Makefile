CC  = gcc
CXX = g++

CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)


.PHONY: default
default: shellGroupChat

shellGroupChat: shellGroupChat.o

shellGroupChat.o: shellGroupChat.c

.PHONY: clean
clean:
	rm -f *.o *~ a.out core aim-in

.PHONY: all
all: clean default
