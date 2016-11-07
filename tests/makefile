CC = gcc
CFLAGS = -g -Wall
TARGET = ck8-test
MAIN = opcode_handler_test.c 
LIBS = -lcmocka
LIBDIR = /usr/local/lib

all: $(TARGET)

$(TARGET): $(MAIN)
	$(CC) $(CFLAGS) -o $(TARGET) $(MAIN) -L $(LIBDIR) $(LIBS)

clean: 
	$(RM) $(TARGET)