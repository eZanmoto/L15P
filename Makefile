CC=gcc
CFLAGS=-Wall
CF=$(CC) $(CFLAGS)
CO=$(CF) -c
TARGET=l15p
OBJS=l15p.o read.o print.o eval.o
BINDIR=/usr/local/bin

cleanbuild: clean all

all: $(OBJS)
	$(CF) $(OBJS) -o $(TARGET)

l15p.o:
	$(CO) l15p.c

read.o:
	$(CO) read.c

print.o:
	$(CO) print.c

eval.o:
	$(CO) eval.c

install:
	cp $(TARGET) $(BINDIR)/$(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET)

distclean: clean
	rm -rf $(BINDIR)/$(TARGET)
