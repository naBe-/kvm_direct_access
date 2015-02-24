###############################################
#              KVM direct access              #
#              make instructions              #
#                                             #
# Author: Pavel Bliznakov                     #
# Last modified: 24/02/2015                   #
###############################################

PROGRAM = kvm_da

SRCDIR = src
OBJDIR = obj
BINDIR = bin

LIBS = 
INCLUDES = includes/

CC = gcc
CFLAGS = -O2 -Wall -I$(INCLUDES) $(LIBS)

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

.PHONY: clean all

all: $(OBJDIR) $(BINDIR) $(BINDIR)/$(PROGRAM)

clean:
	rm -fr $(OBJDIR) $(BINDIR)

$(OBJDIR):
	mkdir -p $@

$(BINDIR):
	mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(CFLAGS)  -c $< -o $@

$(BINDIR)/$(PROGRAM): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LIBS)

