CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wvla -g
LDFLAGS = -lm

EXECUTABLE = gattaca
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
