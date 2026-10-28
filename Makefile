CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDLIBS = -lncursesw
TARGET = program

SRCS = $(wildcard *.c) $(wildcard menus/*.c)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean