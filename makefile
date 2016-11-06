CC = gcc
CFLAGS = -g -Wall
TARGET = ck8
MAIN = ck8.c

all: $(TARGET)

$(TARGET): $(MAIN)
	$(CC) $(CFLAGS) -o $(TARGET) $(MAIN)

clean:
	$(RM) $(TARGET)