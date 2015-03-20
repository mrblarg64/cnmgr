default: cnmgrDemo

LDFLAGS=-lGL -lm -lglfw
CC=gcc

cnmgrDemo: bin/main.o bin/cnmgr.o
	$(CC) $(LDFLAGS) bin/main.o bin/cnmgr.o -o cnmgrDemo
bin/main.o: src/main.c
	$(CC) $(CFLAGS) -c -I src -o bin/main.o src/main.c
bin/cnmgr.o: src/cnmgr.c
	$(CC) $(CFLAGS) -c -I src -o bin/cnmgr.o src/cnmgr.c
