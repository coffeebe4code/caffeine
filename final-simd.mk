# Variables
CC = clang
AR = ar
CFLAGS = -Werror -Wextra -O3 -std=c11 -g -msse4.2
CPPFLAGS = -D__SIMD__

export CC
export AR
export CFLAGS
export CPPFLAGS
.PHONY: all
all:
	$(MAKE)
