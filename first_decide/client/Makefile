bin := main
obj := main.o client.o
src := main.cpp client.cpp
hdr := client.h

CC = g++

.PHONY: clean

$(bin): $(obj)
	$(CC) $(obj) -o $(bin) -pthread

$(obj): $(src)
	$(CC) $(src) -c -pthread

$(obj): $(hdr)

clean:
	rm $(bin) $(obj)