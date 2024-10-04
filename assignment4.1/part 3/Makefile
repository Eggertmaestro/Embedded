CC = g++
CFLAGS = -Iinclude -c -pthread
OUT = output
OBJS = main.o increment.o decrement.o fifo.o

all: $(OUT)

$(OUT): $(OBJS) 
	$(CC) $(OBJS) -o $(OUT)

main.o: src/main.cpp include/increment.h include/decrement.h include/fifo.h
	$(CC) $(CFLAGS) src/main.cpp -o main.o

increment.o: src/increment.cpp include/increment.h include/fifo.h
	$(CC) $(CFLAGS) src/increment.cpp -o increment.o

decrement.o: src/decrement.cpp include/decrement.h include/fifo.h
	$(CC) $(CFLAGS) src/decrement.cpp -o decrement.o

fifo.o: src/fifo.cpp include/fifo.h
	$(CC) $(CFLAGS) src/fifo.cpp -o fifo.o

clean:
	rm -f $(OBJS) $(OUT)