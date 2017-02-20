CC = gcc
CFLAGS = -ggdb -Wall -I/usr/include

LIBS = -lSDL2
LIBDIR = /usr/local/lib

TARGET = ck8
MAIN = ck8
OPCODES = opcodes
CPU = cpu
DISPLAY = display
ALL_OBJECTS = $(MAIN).o $(OPCODES).o $(CPU).o $(DISPLAY).o



all: $(TARGET)


$(TARGET): $(ALL_OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(ALL_OBJECTS) -L/usr/local/lib -lSDL2 #-L $(LIBDIR) $(LIBS)


$(MAIN).o: $(MAIN).c $(OPCODES).h $(CPU).h
	$(CC) $(CFLAGS) -c $(MAIN).c


$(OPCODES).o: $(OPCODES).c $(OPCODES).h $(CPU).h 
	$(CC) $(CFLAGS) -c $(OPCODES).c


$(CPU).o: $(CPU).c $(CPU).h $(DISPLAY).h
	$(CC) $(CFLAGS) -c $(CPU).c


$(DISPLAY).o: $(DISPLAY).c $(DISPLAY).h 
	$(CC) $(CFLAGS) -c $(DISPLAY).c 


test: 
	$(MAKE) -C tests


clean:
	$(RM) $(TARGET) $(ALL_OBJECTS)
	$(MAKE) clean -C tests