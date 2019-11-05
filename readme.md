### smake

Make utility that can list the targets contained in 
the makefile.

`usage: smake [options] [target]

-h, --help	This help

-v, --version	Show version info

-f, --file FILE	Use FILE as makefile

-c		Set target to 'clean'

-p		Print internal data

-P		Alias to '-np'

-R		Disable built-in macros

-n		Print rules (do not execute)`

### Features include

* Single target

* Silent commands/rules (prefixed with `@`)

* Multiple target dependencies

* Dry-run (show rules but don't execute them)

* Use `-c` to set target to `clean`

* Use `-p` to print rules and macros defined in makefile (or built-in)

### Notes

For now only one build target can be specified at a time.

