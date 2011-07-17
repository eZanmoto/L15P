CC=gcc
CFLAGS=-Wall
CF=$(CC) $(CFLAGS)
CO=$(CF) -c
TARGET=l15p
OBJS= $(TARGET).o print.o read.o

cleanbuild: clean all

all: $(OBJS)
	$(CF) $(OBJS) -o $(TARGET)

$(TARGET).o:
	$(CO) $(TARGET).c

print.o:
	$(CO) print.c

read.o:
	$(CO) read.c

clean:
	rm -rf $(OBJS) $(TARGET)
