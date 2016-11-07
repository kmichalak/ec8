CC = gcc
CFLAGS = -g -Wall

TARGET = ck8
MAIN = ck8
OPCODES = opcodes


all: $(TARGET)


$(TARGET): $(MAIN).o $(OPCODES).o
	$(CC) $(CFLAGS) -o $(TARGET) $(MAIN).o $(OPCODES).o


$(MAIN).o: $(MAIN).c $(OPCODES).h
	$(CC) $(CFLAGS) -c $(MAIN).c


$(OPCODES).o: $(OPCODES).c $(OPCODES).h
	$(CC) $(CFLAGS) -c $(OPCODES).c


test: 
	$(MAKE) -C tests


clean:
	$(RM) $(TARGET) $(MAIN).o $(OPCODES).o
	$(MAKE) clean -C tests