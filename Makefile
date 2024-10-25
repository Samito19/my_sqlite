CC=gcc

my_sqlite:
	$(CC) *.c -o my_sqlite

clean:
	rm -f my_sqlite
