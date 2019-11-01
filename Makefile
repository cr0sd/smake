all :dependentoid
	c++ smake.cc -o smake -Wfatal-errors -Wall -Wextra -Os
	#this is line 2
smake:all
	#nothing
dependentoid:
	@echo 'hi from dependentoid'
clean:
	@rm -f smake *.o
install:smake
	#now we do the installing! xD
