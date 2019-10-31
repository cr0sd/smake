all:f
	c++ smake.cc -o smake -Wfatal-errors -Wall -Wextra -Os
	#this is line 2
f:
	@echo 'hi from f'
clean:
	@rm -f smake *.o
