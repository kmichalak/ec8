CC = gcc
CFLAGS = -ggdb -Wall -I/usr/include
TARGET = ck8-test
MAIN = opcode_handler_test
LIBS = -lcmocka -lSDL2 -lm
LIBDIR = /usr/local/lib
INCLUDEDIR = ../ 

CPU = cpu
OPCODES = opcodes
DISPLAY = display
ALL_OBJECTS = $(MAIN).o $(OPCODES).o $(CPU).o $(DISPLAY).o

all:  $(TARGET)

	# $(TARGET)

timer_test_target: timer_test.o timer.o
	$(CC) $(CFLAGS) -o timer_test timer_test.o timer.o -L $(LIBDIR) $(LIBS)

timer_test: timer_test.c ../timer.h
	$(CC) $(CFLAGS) -c timer_test.c

timer.o: ../timer.c ../timer.h
	$(CC) $(CFLAGS) -c ../timer.c



$(TARGET): $(ALL_OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(ALL_OBJECTS) -L $(LIBDIR) $(LIBS)

$(MAIN).o: $(MAIN).c ../$(OPCODES).h ../$(CPU).h
	$(CC) $(CFLAGS) -c $(MAIN).c

$(OPCODES).o: ../$(OPCODES).c ../$(OPCODES).h ../$(CPU).h ../$(DISPLAY).h
	$(CC) $(CFLAGS) -c ../$(OPCODES).c

$(CPU).o: ../$(CPU).c ../$(CPU).h
	$(CC) $(CFLAGS) -c ../$(CPU).c

$(DISPLAY).o: ../$(DISPLAY).c ../$(DISPLAY).h
	$(CC) $(CFLAGS) -c ../$(DISPLAY).c

clean: 
	$(RM) $(TARGET) $(ALL_OBJECTS) timer_test.o timer.o timer_test