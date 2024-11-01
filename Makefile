CC=gcc
CFLAGS=-Wall -std=c99

my_sqlite:
	$(CC) $(CFLAGS) *.c -o my_sqlite

run: 
	./my_sqlite

clean:
	rm -f my_sqlite db
