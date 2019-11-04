# Use builtin macros
#CXX = c++
#CC = cc
#RM = rm

all:a
	#done
a:a.o
	$(CC) a.o b.o c.o -o a
a.o:b.o c.o
	$(CC) -c a.c
b.o:e.o
	$(CC) -c b.c
c.o:
	$(CC) -c c.c
e.o:
	#no things
clean:
	@$(RM) -f a *.o
