CC=clang++
CFLAGS=-std=c++11
LDFLAGS=-std=c++11

SRCS=main.cc
OBJS=$(SRCS:.cc=.o)
EXEC=ts

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@

.phony: clean

clean:
	rm -f $(OBJS) $(EXEC)
