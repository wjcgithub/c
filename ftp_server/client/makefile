CC := gcc
CFLAGS := -Wall -g -Os -O0

SHDIR := ../common

OBJS = ftclient.o $(SHDIR)/common.o

all: ftclient

ftclient: $(OBJS)
	@$(CC) -o $@ $(CFLAGS) $(OBJS)

$(OBJS) : %.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY:
clean:
	@rm -f *.o ftclient
	@rm -f ../common/*.o
	@echo Done cleaning
