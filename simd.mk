CC = gcc
AR = ar
CFLAGS = -Wall -O0 -std=c11 -g 
CPPFLAGS = -DDEBUG

export CC
export AR
export CFLAGS
export CPPFLAGS
.PHONY: all
all:
	$(MAKE)
