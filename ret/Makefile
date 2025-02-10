CC = gcc
CFLAGS = -std=gnu99 -Wpedantic -Wall -I. -o ret -ludis86
CFILES = file.c gadget.c list.c main.c search.c

ret:
	$(CC) $(CFILES) $(CFLAGS)

.PHONY: clean
clean:
	rm ret
