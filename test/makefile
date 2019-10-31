all:onions.o
	cc onions.c -o onions
a:a.o
	cc a.o b.o c.o -o a
a.o:b.o c.o
	cc -c a.c
b.o:
	cc -c b.c
c.o:
	cc -c c.c
clean:
	@rm -f onions *.o
