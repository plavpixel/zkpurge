CC = cc
CFLAGS ?= -O2 -Wall -Wextra -pedantic
CFLAGS += -std=c23
TARGET = zkpurge
OBJS = main.o util.o zbm.o

.PHONY: all build clean install uninstall

all: build

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c defs.h zbm.h util.h
util.o: util.c util.h defs.h
zbm.o: zbm.c zbm.h util.h defs.h

clean:
	rm -f $(TARGET) $(OBJS)

install: build
	install -m 755 $(TARGET) /usr/local/bin/$(TARGET)

uninstall:
	rm -f /usr/local/bin/$(TARGET)