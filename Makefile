CXX=g++
CC=gcc

CXXFLAGS=-Wfatal-errors -Wall -Wextra -O2 -Wno-unused-result
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

README=readme.md

all: smake
smake:
	$(CXX) $(PROG).cc -o $(PROG) $(CXXFLAGS) $(LDFLAGS)
clean:
	@$(RM) smake *.o
docs: smake
	cat docs/head > $(README)
	$(THIS) -v >> $(README)
	$(THIS) -h >> $(README)
	cat docs/foot >> $(README)
install: smake
	cp $(PROG) $(PREFIX)
uninstall:
	$(RM) $(PREFIX)$(PROG)
.PHONY: all docs install uninstall clean
