CXX=g++
CC=gcc

CXXFLAGS=-Wfatal-errors -Wall -Wextra -Os -Wno-unused-result
CXXFLAGS += -std=c++17
LDFLAGS=-lstdc++fs

PROG=smake
PREFIX=/usr/local/bin/

ifeq ($(OS),Windows_NT)
CXXFLAGS += -D WINDOWS
THIS=.\smake
else
CXXFLAGS += -D GNULINUX
THIS=./smake
endif

ifdef ($(SMAKE))
ISSMAKE=Smake :-) version: $(SMAKE)
else
ISSMAKE=Not Smake :-(
endif

APPEND=dd status=none oflag=append conv=notrunc of=
README=readme.md

# This is a makefile so for to as testingly :-D

# And this is a \
multiline comment!

all: \
	smake
smake: \

	#$(OS) $(ISSMAKE)
	# We will now build the thing...
	$(CXX) $(PROG).cc -o $(PROG) $(CXXFLAGS) $(LDFLAGS)
clean:
	@$(RM) smake *.o
docs: smake
	cat docs/head > $(README)
	$(THIS) -v | $(APPEND)$(README)
	$(THIS) -h | $(APPEND)$(README)
	cat docs/foot | $(APPEND)$(README)
install: \
	smake
	cp $(PROG) $(PREFIX)
uninstall:
	$(RM) $(PREFIX)$(PROG)
.PHONY: all docs install uninstall clean
