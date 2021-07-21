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

OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*/*.c))
INCS := $(patsubst $(SRCDIR)/%.h, $(INCDIR)/%.h, $(wildcard $(SRCDIR)/*/*.h))
IMMM := $(patsubst $(OBJDIR)/%.o,$(LIBDIR)/%.so, $(foreach LIB, $(LLIST), $(filter $(OBJDIR)/$(LIB)/$(LIB).o,$(OBJS))))
LIBS := $(foreach PRE, $(LLIST), $(subst $(PRE)/,lib,$(filter $(LIBDIR)/$(PRE)/$(PRE).so,$(IMMM))))
LINK := $(foreach LIB, $(LLIST), -l$(LIB))


.PHONY: all 
all: $(TGTDIR)/$(TGT) 

$(TGTDIR)/$(TGT): $(LIBS) 
	$(CC) -L$(LIBDIR) $(LINK) -o $@ 

$(LIBS): $(OBJS)
	@mkdir -p $(@D)
	$(CC) -shared -o $@ $(filter $(subst lib,$(OBJDIR)/, $(basename $(notdir $@)))/%.o, $(OBJS))

$(OBJS): $(SRCDIR)/*/*.c
	@mkdir -p $(@D)
	$(CC) -o $@ -c $<

.PHONY: folders
folders:
	mkdir -p $(DIRS)

.PHONY: clean
clean:
	rm -rf $(DIRS)

