all:main clean

CC=g++
CPPFLAGS=-std=c++11  -g

main:main.o downloader.o spider.o
	$(CC)  $(CPPFLAGS) -o main main.o downloader.o spider.o -lcurl -lgumbo 

clean:
	rm -r *.o
