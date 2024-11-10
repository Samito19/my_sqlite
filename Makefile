CC=gcc
CFLAGS=-Wall -std=c99

my_sqlite:
	$(CC) $(CFLAGS) *.c -o my_sqlite

run: 
	./my_sqlite

clean_db:
	rm -f my_sqlite db

clean:
	rm -f my_sqlite
