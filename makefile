# Variables
CC = gcc
AR = ar
CFLAGS = -Wall -O0 -std=c11 -DDEBUG -g
ODUMP = objdump
ODFLAGS = -d
OBJCOPY = objcopy
OBJFLAGS = -O binary

# Directories
OBJDIR = obj
TGTDIR = target
INCDIR = include
SRCDIR = src
TSTDIR = tests
LIBDIR = lib
DEPDIR = .d
DIRS = $(OBJDIR) $(TGTDIR) $(INCDIR) $(LIBDIR) $(DEPDIR)

# Target name
TGT = caffeine
LLIST = utils middleware

OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*/*.c))
INCS := $(patsubst $(SRCDIR)/%.h,$(INCDIR)/%.h,$(wildcard $(SRCDIR)/*/*.h))
IMMM := $(patsubst $(OBJDIR)/%.o,$(LIBDIR)/%.a,$(foreach LIB,$(LLIST),$(filter $(OBJDIR)/$(LIB)/$(LIB).o,$(OBJS))))
LIBS := $(foreach PRE,$(LLIST),$(subst $(PRE)/,lib,$(filter $(LIBDIR)/$(PRE)/$(PRE).a,$(IMMM))))
LINK := $(foreach LIB,$(LLIST),-l$(LIB))
TSTS := $(wildcard $(TSTDIR)/*.c)
TSTE := $(foreach LIB,$(LLIST),$(TGTDIR)/$(LIB).exe)

.PHONY: all 
all: $(TGTDIR)/$(TGT)

$(TGTDIR)/$(TGT): $(LIBS) | tests
	$(CC) -o $@ bench/main.c $(LIBS) 

tests: $(TSTE)
	for test in $^ ; do \
		chmod +x ./$${test} ; \
		./$${test}; \
	done
	
$(TSTE): $(OBJS)
	@mkdir -p $(@D)
	$(CC) -o $@ $(OBJS) $(filter $(TSTDIR)/$(basename $(notdir $@)).c, $(TSTS))

$(LIBS): $(OBJS)
	@mkdir -p $(@D)
	$(AR) rcs $@ $(filter $(subst lib,$(OBJDIR)/, $(basename $(notdir $@)))/%.o, $(OBJS))

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) -o $@ -c $<

.PHONY: folders
folders:
	mkdir -p $(DIRS)

.PHONY: clean
clean:
	rm -rf $(DIRS)

