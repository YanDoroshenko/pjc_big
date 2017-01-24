CC=g++
CPPFLAGS=-pthread -std=c++14

OBJS=reader.o entry.o parser.o
reader: $(OBJS)
	$(CC) $(OBJS) $(CPPFLAGS) -o reader
