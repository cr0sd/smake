all:
	c++ smake.cc -o smake -Wfatal-errors -Os -s
clean:
	rm -f smake *.o
