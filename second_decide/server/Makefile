bin := main
obj := main.o server.o
src := main.cpp server.cpp
hdr := server.h

CC = g++

.PHONY: clean

$(bin): $(obj)
	$(CC) $(obj) -o $(bin) -pthread

$(obj): $(src)
	$(CC) $(src) -c -pthread

$(obj): $(hdr)

clean:
	rm $(bin) $(obj)