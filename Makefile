CXX=c++
CC=cc
CXXFLAGS=-Wfatal-errors -Wall -Wextra -Os
CXXFLAGS += -Wall
PROG=smake

# This is a makefile so for to as testingly :-D

all :dependentoid
	# This comment
	echo $(balogna)
	$(CXX) $(PROG).cc -o $(PROG) $(CXXFLAGS)
smake:all
	
dependentoid: #hi
	@echo 'hi from dependentoid'
clean:
	@rm -f smake *.o
install:smake
	cp smake /usr/local/bin
uninstall:
	rm /usr/local/bin/smake
