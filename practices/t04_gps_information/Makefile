CC=gcc
CFLAGS=-Wall
PROJECT=main
LDFLAGS=-lm -lpthread
DESTDIR=/usr/local/bin

all: $(PROJECT)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

$(PROJECT): $(patsubst %.c, %.o, $(wildcard *.c))
	$(CC) $^ -o $@ $(LDFLAGS)

.PHONY: clean run install uninstall

clean:
	rm -f *.o $(PROJECT)

run_server: $(PROJECT)
	./$(PROJECT) server

run_client: $(PROJECT)
	./$(PROJECT) client