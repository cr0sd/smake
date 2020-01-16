CXX=g++
CC=gcc

# Files, directories
OBJS=smake.o
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
CXXFLAGS += -D GNULINUX
THIS=./$(PROG)
endif

# Exhibit Smake specific stuff
ifdef ($(SMAKE))
ISSMAKE=Smake :-) version: $(SMAKE)
else
ISSMAKE=Not Smake :-(
endif

# Make targets
all: $(OBJS)
	$(CXX) $(OBJS) -o $(PROG) $(CXXFLAGS) $(LDFLAGS)
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
	@$(RM) $(PROG) $(OBJS)
.PHONY: all docs install uninstall clean
