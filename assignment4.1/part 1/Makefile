OBJS = src/main.o

all: hello

hello: $(OBJS)
	g++ -o hello $(OBJS)

src/%.o: src/%.cpp
	g++ -o $@ -c $<