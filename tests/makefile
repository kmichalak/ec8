CC = gcc
CFLAGS = -g -Wall
TARGET = ck8-test
MAIN = opcode_handler_test
LIBS = -lcmocka
LIBDIR = /usr/local/lib
INCLUDEDIR = ../ 

CPU = cpu
OPCODES = opcodes
ALL_OBJECTS = $(MAIN).o $(OPCODES).o $(CPU).o

all: $(TARGET)

$(TARGET): $(ALL_OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(ALL_OBJECTS) -L $(LIBDIR) $(LIBS)

$(MAIN).o: $(MAIN).c ../$(OPCODES).h ../$(CPU).h
	$(CC) $(CFLAGS) -c $(MAIN).c

$(OPCODES).o: ../$(OPCODES).c ../$(OPCODES).h ../$(CPU).h
	$(CC) $(CFLAGS) -c ../$(OPCODES).c

$(CPU).o: ../$(CPU).c ../$(CPU).h
	$(CC) $(CFLAGS) -c ../$(CPU).c

clean: 
	$(RM) $(TARGET) $(ALL_OBJECTS)