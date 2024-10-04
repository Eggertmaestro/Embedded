OBJS = main.o increment.o decrement.o
CC = g++
CFLAGS = -Iinclude -c
THREADFLAG = -lpthread
OUT = output

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(OBJS) $(THREADFLAG) -o $(OUT)

main.o: src/main.cpp include/increment.h include/decrement.h
	$(CC) $(CFLAGS) src/main.cpp -o main.o

increment.o: src/increment.cpp include/increment.h
	$(CC) $(CFLAGS) src/increment.cpp -o increment.o

decrement.o: src/decrement.cpp include/decrement.h
	$(CC) $(CFLAGS) src/decrement.cpp -o decrement.o

clean:
	rm -f $(OBJS) $(OUT)