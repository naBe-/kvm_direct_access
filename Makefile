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
INCLUDE = $(SRCDIR)/*.h

CC = gcc
CFLAGS = -O2 -Wall -I$(INCLUDE) $(LIBS)

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

$(OBJDIR):
	mkdir -p $@

$(BINDIR):
	mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDE)
	$(CC) $(CFLAGS)  -c $< -o $@

$(BINDIR)/$(PROGRAM): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LIBS)

all: $(OBJDIR) $(BINDIR) $(BINDIR)/$(PROGRAM)

.PHONY: clean

clean:
	rm -fr $(OBJDIR) $(BINDIR)

