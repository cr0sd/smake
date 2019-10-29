all:
	c++ smake.cc -o smake -Wfatal-errors -Os
clean:
	@rm -f smake *.o
