# Variables
CC = gcc
AR = ar
CFLAGS = -Wall -O0 -std=c11 -g -msse4.2
CPPFLAGS = -DDEBUG -D__SIMD__

export CC
export AR
export CFLAGS
export CPPFLAGS
.PHONY: all
all:
	$(MAKE)
