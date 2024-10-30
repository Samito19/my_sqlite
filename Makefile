CC=gcc
CFLAGS=-Wall -std=c11

my_sqlite:
	$(CC) $(CFLAGS) *.c -o my_sqlite

run: 
	./my_sqlite

clean:
	rm -f my_sqlite db
