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
docs:
	# Overwrite readme.md and insert
	# up-to-date help info into file
	cat doc/head > readme.md
	./smake --help | tee -a readme.md
	cat doc/foot | tee -a readme.md
install:smake
	cp $(PROG) $(PREFIX)
uninstall:
	$(RM) $(PREFIX)$(PROG)
