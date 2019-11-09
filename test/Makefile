# Use builtin macros
#CXX = c++
#CC = cc
#RM = rm

#hey this is a \
comment

all:a
	# This one is done after all dependencies \
	this are comments boys\
	this are still comments boys
	@echo Compiling for $(OS)...
a:a.o
	$(CC) a.o b.o c.o -o a
a.o:b.o c.o
	$(CC) -c a.c
b.o:e.o
	$(CC) -c b.c
c.o:
	$(CC) -c c.c
e.o:
	# This one has no dependencies
clean:
	@$(RM) -f a *.o
