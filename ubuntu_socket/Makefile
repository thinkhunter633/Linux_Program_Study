# all: main
# main: main.c
# 	gcc -g -w -o $@ $<


# Targe
TARGET = main

# source files
SRCS = main.c frame_procedure.c http_parser.c

# header files
HDRS = frame_procedure.h http_parser.h

# comiplier
CC = gcc

# option
CFLAGS = -Wall -g

# generate the target file
$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# generate the o files
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# clean the target file and o files
clean:
	rm -f $(TARGET) *.o
