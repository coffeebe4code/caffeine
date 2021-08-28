# Variables
CC = gcc
AR = ar
CFLAGS = -Wall -O3 -std=c11 -g 
CPPFLAGS = -DDEBUG -D__PERF__

export CC
export AR
export CFLAGS
export CPPFLAGS
.PHONY: all
all:
	$(MAKE)
