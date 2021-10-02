# Variables
CC = clang
AR = ar
CFLAGS = -Werror -Wextra -Wno-unused-command-line-argument -O3 -std=c11 -g
CPPFLAGS = 

export CC
export AR
export CFLAGS
export CPPFLAGS
.PHONY: all
all:
	$(MAKE)
