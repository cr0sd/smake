CXX=g++
CC=gcc
CXXFLAGS=-Wfatal-errors -Wall -Wextra -Os
PROG=smake
PREFIX=/usr/local/bin/

#CXXFLAGS += -D WINDOWS
#CXXFLAGS += -U GNULINUX

CXXFLAGS += -U WINDOWS
CXXFLAGS += -D GNULINUX

# This is a makefile so for to as testingly :-D

all :
	# We will now build the thing...
	$(CXX) $(PROG).cc -o $(PROG) $(CXXFLAGS)
clean:
	@$(RM) smake *.o
install:smake
	cp $(PROG) $(PREFIX)
uninstall:
	$(RM) $(PREFIX)$(PROG)
