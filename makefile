all:f
	c++ smake.cc -o smake -Wfatal-errors -Wall -Wextra -Os
	#this is line 2
clean:
	@rm -f smake *.o
f:
	@echo do stuff yes
feline:
	echo do stuff k
fillip:
	@echo do stuff okay
frogs:
	@echo do stuff mkay
