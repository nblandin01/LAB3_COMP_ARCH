# Configuration

CC		= gcc
CFLAGS		= -w -g -Wall -std=gnu99 

TARGETS = no_blocking blocking

all: $(TARGETS)

no_blocking: no_blocking.c 
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) -o $@ $^

blocking: blocking.c 
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) -o $@ $^

clean: 
	@echo "Removing objects" 
	@rm -f $(TARGETS) 
