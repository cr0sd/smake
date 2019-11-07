CXX=g++
CC=gcc
CXXFLAGS=-Wfatal-errors -Wall -Wextra -Os -Wno-unused-result
PROG=smake
PREFIX=/usr/local/bin/

ifeq ($(OS),Windows_NT)
CXXFLAGS += -D WINDOWS
CXXFLAGS += -U GNULINUX
THIS=.\smake
endif

ifeq ($(OS),GNU/Linux)
CXXFLAGS += -U WINDOWS
CXXFLAGS += -D GNULINUX
THIS=./smake
endif

#ifdef ($(SMAKE),$(OS))
#ISSMAKE=Smake version: $(SMAKE)
#endif

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
