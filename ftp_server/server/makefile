CC := gcc
CFLAGS := -Wall -g -Os -O0

SHDIR := ../common

OBJS = ftserve.o $(SHDIR)/common.o

all: ftserve

ftserve: $(OBJS)
	@$(CC) -o $@ $(CFLAGS) $(OBJS)


$(OBJS) : %.o: %.c
	@$(CC) -c $(CFLAGS) $< -o $@

.PHONY:
clean:
	@rm -f *.o ftserve
	@rm -f ../common/*.o
	@echo DOne cleaning
