### smake

Make utility that can list the targets contained in 
the makefile.

<code>usage: smake [options] [target]<br>

-h, --help	This help<br>

-v, --version	Show version info<br>

-f, --file FILE	Use FILE as makefile<br>

-c		Set target to 'clean'<br>

-p		Print internal data<br>

-P		Alias to '-np'<br>

-R		Disable built-in macros<br>
-n		Print rules (do not execute)</code>

### Features include

* Single target

* Silent commands/rules (prefixed with `@`)

* Multiple target dependencies

* Dry-run (show rules but don't execute them)

* Use `-c` to set target to `clean`

* Use `-p` to print rules and macros defined in makefile (or built-in)

### Notes

For now only one build target can be specified at a time.

