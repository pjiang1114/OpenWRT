#build httppost executable when user execute "make"
IDIR=../include
CC=gcc
CFLAGS=-Wall -I$(IDIR)
LDFLAGS=-L/usr/lib/i386-linux-gnu
LDLIBS=-lcurl
ODIR=obj
LDIR=../lib


httppost: httppost.o
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ $(LOADLIBS) $(LDLIBS) -o $@
httppost.o: httppost.c
	$(CC) $(CFLAGS) $(TARGET_ARCH)-c -o $@ $<

#remove object files when user executes "make clean"
.PHONY: clean
clean:
	rm *.o 
