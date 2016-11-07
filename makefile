CC = gcc
CFLAGS = -g -Wall
TARGET = ck8
MAIN = ck8.c

all: $(TARGET)

$(TARGET): $(MAIN)
	$(CC) $(CFLAGS) -o $(TARGET) $(MAIN)

test: 
	$(MAKE) -C tests

clean:
	$(RM) $(TARGET)
	$(MAKE) clean -C tests