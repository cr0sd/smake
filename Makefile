CXX=g++
CC=gcc
CXXFLAGS=-Wfatal-errors -Wall -Wextra -Os
CXXFLAGS += -Wall
PROG=smake
PREFIX=/usr/local/bin/

# This is a makefile so for to as testingly :-D

all :
	$(CXX) $(PROG).cc -o $(PROG) $(CXXFLAGS)
clean:
	@$(RM) smake *.o
install:smake
	cp $(PROG) $(PREFIX)
uninstall:
	rm $(PREFIX)$(PROG)
