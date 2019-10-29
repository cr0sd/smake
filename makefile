all:
	c++ smake.cc -o smake -Wfatal-errors
clean:
	@rm -f smake *.o
