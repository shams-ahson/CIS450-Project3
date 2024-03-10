CFLAGS= -g
LDFLAGS= -pthread
CC=g++

all: boundedBuffer bridge savingAccount savingAccountFCFS

# To make an executable
boundedBuffer: boundedBuffer.o 
	$(CC) $(LDFLAGS) -o boundedBuffer boundedBuffer.o

bridge: bridge.o 
	$(CC) $(LDFLAGS) -o bridge bridge.o

savingAccount: savingAccount.o
	$(CC) $(LDFLAGS) -o savingAccount savingAccount.o

savingAccountFCFS: savingAccountFCFS.o
	$(CC) $(LDFLAGS) -o savingAccountFCFS savingAccountFCFS.o
 
# To make an object from source
.c.o:
	$(CC) $(CFLAGS) -c $*.c

# clean out the dross
clean:
	-rm boundedBuffer bridge savingAccount savingAccountFCFS *.o

