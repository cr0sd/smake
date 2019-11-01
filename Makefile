CXX=c++
all :dependentoid
	c++ smake.cc -o smake -Wfatal-errors -Wall -Wextra -Os
smake:all
	
dependentoid:
	@echo 'hi from dependentoid'
clean:
	@rm -f smake *.o
install:smake
	cp smake /usr/local/bin
uninstall:
	rm /usr/local/bin/smake
