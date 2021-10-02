# Variables
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
LLIST = server barista requester responder utils

OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(foreach LIB,$(LLIST),$(filter $(SRCDIR)/$(LIB)/$(LIB).c,$(wildcard $(SRCDIR)/*/*.c))))
INCS := $(patsubst $(SRCDIR)/%.h,$(INCDIR)/%.h,$(wildcard $(SRCDIR)/*/*.h))
IMMM := $(patsubst $(OBJDIR)/%.o,$(LIBDIR)/%.a,$(foreach LIB,$(LLIST),$(filter $(OBJDIR)/$(LIB)/$(LIB).o,$(OBJS))))
LIBS := $(foreach PRE,$(LLIST),$(subst $(PRE)/,lib,$(filter $(LIBDIR)/$(PRE)/$(PRE).a,$(IMMM))))
LINK := $(foreach LIB,$(LLIST),-l$(LIB))
TSTO := $(patsubst $(TSTDIR)/%.c,$(OBJDIR)/$(TSTDIR)/%.o, $(wildcard $(TSTDIR)/*.c))
TSTE := $(foreach LIB,$(LLIST),$(TGTDIR)/$(LIB).exe)

.PHONY: all 
all: $(TGTDIR)/$(TGT)

$(TGTDIR)/$(TGT): $(LIBS) | tests 
	$(CC) $(CFLAGS) -o $@ bench/main.c $(LIBS) -lpthread

tests: $(TSTE)
	err=0; \
	for test in $^ ; do \
		chmod +x ./$${test} ; \
		./$${test} || err=$$?; \
	done; \
	exit $$err
	
$(TSTE): $(TSTO) | $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $(filter $(OBJDIR)/$(TSTDIR)/$(basename $(notdir $@)).o,$(TSTO)) $(OBJS) $(LFLAGS)

$(OBJDIR)/$(TSTDIR)/%.o: $(TSTDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $< $(LFLAGS)

$(LIBS): $(OBJS)
	@mkdir -p $(@D)
	$(AR) rcs $@ $(filter $(subst lib,$(OBJDIR)/, $(basename $(notdir $@)))/%.o, $(OBJS))

$(OBJDIR)/%.o: $(SRCDIR)/%.c | folders
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

.PHONY: folders
folders:
	@mkdir -p $(DIRS)
	
.PHONY: clean
clean:
	@rm -rf $(DIRS)

