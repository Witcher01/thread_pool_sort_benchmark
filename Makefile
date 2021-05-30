CC=g++
CPPFLAGS=-std=c++17 -pthread
DEPS=thread_pool.h

default: thread_pool_sort

%.o: %.c $(DEPS)
	$(CC) $(CPPFLAGS) -c -o $@ $<

thread_pool_sort: main.o
	$(CC) $(CPPFLAGS) -o $@ $<
