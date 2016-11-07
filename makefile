CC = gcc
CFLAGS = -g -Wall

TARGET = ck8
MAIN = ck8
OPCODES = opcodes
CPU = cpu
ALL_OBJECTS = $(MAIN).o $(OPCODES).o $(CPU).o

all: $(TARGET)


$(TARGET): $(ALL_OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(ALL_OBJECTS)


$(MAIN).o: $(MAIN).c $(OPCODES).h $(CPU).h
	$(CC) $(CFLAGS) -c $(MAIN).c


$(OPCODES).o: $(OPCODES).c $(OPCODES).h $(CPU.h)
	$(CC) $(CFLAGS) -c $(OPCODES).c


$(CPU).o: $(CPU).c $(CPU).h
	$(CC) $(CFLAGS) -c $(CPU).c

test: 
	$(MAKE) -C tests


clean:
	$(RM) $(TARGET) $(ALL_OBJECTS)
	$(MAKE) clean -C tests