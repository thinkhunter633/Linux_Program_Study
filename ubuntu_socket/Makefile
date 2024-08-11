# all: main
# main: main.c
# 	gcc -g -w -o $@ $<


# 目标文件
TARGET = main

# 源文件
SRCS = main.c frame_procedure.c

# 头文件
HDRS = frame_procedure.h

# 编译器
CC = gcc

# 编译选项
CFLAGS = -Wall -g

# 生成目标文件
$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# 生成源文件对应的.o文件
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(TARGET) *.o
