CXX=g++
CC=gcc
CXXFLAGS=-Wfatal-errors -Wall -Wextra -Os -Wno-unused-result
PROG=smake
PREFIX=/usr/local/bin/


CXXFLAGS += -D GNULINUX
THIS=.\smake

ifdef ($(SMAKE))
ISSMAKE=Smake :-) version: $(SMAKE)
else
ISSMAKE=Not Smake :-(
endif

APPEND=dd status=none oflag=append conv=notrunc of=
README=readme.md

# This is a makefile so for to as testingly :-D

all: smake
smake:
	#@echo $(OS) $(ISSMAKE)
	# We will now build the thing...
	$(CXX) $(PROG).cc -o $(PROG) $(CXXFLAGS)
clean:
	@$(RM) smake *.o
docs: #smake
	cat doc/head > $(README)
	$(THIS) -v | $(APPEND)$(README)
	$(THIS) -h | $(APPEND)$(README)
	cat doc/foot | $(APPEND)$(README)
install: smake
	cp $(PROG) $(PREFIX)
uninstall:
	$(RM) $(PREFIX)$(PROG)
