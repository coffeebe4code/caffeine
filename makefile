# Variables
CC = gcc
AR = ar
CFLAGS = -Wall -O0 -std=c11 -g -msse4.2
CPPFLAGS = -DDEBUG -D__SIMD__
LFLAGS = -lpthread
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
DIRS = $(OBJDIR) $(TGTDIR) $(INCDIR) $(LIBDIR) 

# Target name
TGT = caffeine
LLIST = utils requester responder header server

OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*/*.c))
INCS := $(patsubst $(SRCDIR)/%.h,$(INCDIR)/%.h,$(wildcard $(SRCDIR)/*/*.h))
IMMM := $(patsubst $(OBJDIR)/%.o,$(LIBDIR)/%.a,$(foreach LIB,$(LLIST),$(filter $(OBJDIR)/$(LIB)/$(LIB).o,$(OBJS))))
LIBS := $(foreach PRE,$(LLIST),$(subst $(PRE)/,lib,$(filter $(LIBDIR)/$(PRE)/$(PRE).a,$(IMMM))))
LINK := $(foreach LIB,$(LLIST),-l$(LIB))
TSTO := $(patsubst $(TSTDIR)/%.c,$(OBJDIR)/$(TSTDIR)/%.o, $(wildcard $(TSTDIR)/*.c))
TSTE := $(foreach LIB,$(LLIST),$(TGTDIR)/$(LIB).exe)
DEPS := $(OBJS:.o=.d)
DEPT := $(TSTO:.o=.d)

.PHONY: all 
all: $(TGTDIR)/$(TGT)

$(TGTDIR)/$(TGT): $(LIBS) | tests 
	$(CC) $(CFLAGS) -o $@ bench/main.c $(LIBS) -lpthread

##includes: $(INCS)
##	for inc in $^; do \
##		cp -u $${inc} $(INCDIR)/$(basename $(notdir $${inc}

tests: $(TSTE)
	for test in $^ ; do \
		chmod +x ./$${test} ; \
		./$${test}; \
	done
	
$(TSTE): $(TSTO) $(DEPT) | $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $(filter $(OBJDIR)/$(basename $(notdir $@))/$(basename $(notdir $@)).o,$(OBJS)) $(filter $(OBJDIR)/$(TSTDIR)/$(basename $(notdir $@)).o,$(TSTO)) $(LFLAGS)

$(OBJDIR)/$(TSTDIR)/%.o: $(TSTDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $< $(LFLAGS)

$(OBJDIR)/$(TSTDIR)/%.d: $(TSTDIR)/%.c
	@mkdir -p $(@D)
	$(CC) -MM -MG $< $(CFLAGS)

$(LIBS): $(OBJS)
	@mkdir -p $(@D)
	$(AR) rcs $@ $(filter $(subst lib,$(OBJDIR)/, $(basename $(notdir $@)))/%.o, $(OBJS))

$(OBJDIR)/%.o: $(SRCDIR)/%.c | folders
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/%.d: $(SRCDIR)/%.c | folders
	@mkdir -p $(@D)
	$(CC) -MM -MG $< $(CFLAGS)

.PHONY: folders
folders:
	mkdir -p $(DIRS)
	
.PHONY: clean
clean:
	rm -rf $(DIRS)

