CC=clang++
CFLAGS=-std=c++11
LDFLAGS=-std=c++11

SRCS=main.cc util.cc scan.cc parse.cc ast.cc print.cc
HDRS=util.h scan.h parse.h ast.h print.h
OBJS=$(SRCS:.cc=.o)
EXEC=ts

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

main.o: main.cc $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cc $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

.phony: clean

clean:
	rm -f $(OBJS) $(EXEC)
