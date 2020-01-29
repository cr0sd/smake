CXX=g++
CC=gcc

# Files, directories
PROG=smake
PREFIX=/usr/local/bin/
README=readme.md

# Compiler + linker flags
CXXFLAGS=-Wfatal-errors -Wall -Wextra -O2 -Wno-unused-result
CXXFLAGS += -std=c++17
LDFLAGS=-lstdc++fs

# OS Specific
ifeq ($(OS),Windows_NT)
CXXFLAGS += -D WINDOWS
THIS=.\$(PROG)
else
ifeq ($(OS),FreeBSD)
CXXFLAGS += -D FREEBSD
THIS=./$(PROG)
else
CXXFLAGS += -D GNULINUX
THIS=./$(PROG)
endif
endif

# Exhibit Smake specific stuff
ifdef ($(SMAKE))
ISSMAKE=Smake :-) version: $(SMAKE)
else
ISSMAKE=Not Smake :-(
endif

# Make targets
all: smake.o
	$(CXX) smake.o -o $(PROG) $(CXXFLAGS) $(LDFLAGS)
smake.o:
	$(CXX) -c smake.cc $(CXXFLAGS) $(LDFLAGS)
docs: $(PROG)
	cat docs/head > $(README)
	$(THIS) -v >> $(README)
	$(THIS) -h >> $(README)
	cat docs/foot >> $(README)
install: $(PROG)
	cp $(PROG) $(PREFIX)
uninstall:
	$(RM) $(PREFIX)$(PROG)
clean:
	@$(RM) $(PROG) smake.o
.PHONY: all docs install uninstall clean
