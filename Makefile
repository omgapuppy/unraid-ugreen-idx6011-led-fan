CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic

.PHONY: all clean test

all: idx6011-fan-status

idx6011-fan-status: src/idx6011-fan-status.c
	$(CC) $(CFLAGS) -o $@ $<

test:
	sh test/idx6011-fan-status.test.sh

clean:
	rm -f idx6011-fan-status
