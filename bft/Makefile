P=bft
CC=clang
CFLAGS=`pkg-config --cflags libusb-1.0` -g -Wall -std=c11 -O3
LDFLAGS=`pkg-config --libs libusb-1.0`
OBJECTS=bft.o

build: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(P) $(LDFLAGS)

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm $(OBJECTS) $(P)
