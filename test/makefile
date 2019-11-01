CXX=c++
all:a
	#done
a:a.o
	cc a.o b.o c.o -o a
a.o:b.o c.o
	cc -c a.c
b.o:e.o
	cc -c b.c
c.o:
	cc -c c.c
e.o:
	#no things
clean:
	@rm -f a *.o
