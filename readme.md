[![Coverage Status](https://coveralls.io/repos/github/cr0sd/smake/badge.svg?branch=master)](https://coveralls.io/github/cr0sd/smake?branch=master)

### smake

Make utility that can list the targets contained in 
the makefile. Also defines `$(OS)` for Windows_NT or
GNU/Linux.

```bash
smake 0.5.3 
Build Nov 12 2019 13:16:24
usage: smake [options] [target]
-h, --help			This help
-v, --version			Show version info
-f, --file FILE			Use FILE as makefile
-c				Set target to 'clean'
-p				Print internal data
-P				Alias to '-np'
-R, --no-builtin-variables	Disable built-in macros
-B				Unconditionally build all
-n, --just-print		Print rules (do not execute)
```

### Features include

* Single target

* Silent commands/rules (prefixed with `@`)

* Multiple target dependencies

* Dry-run (show rules but don't execute them)

* Use `-c` to set target to `clean`

* Use `-p` to print rules and macros defined in makefile (or built-in)

* `ifeq`, `ifdef` conditionals supported

### Notes

For now only one build target can be specified at a time.

See the main Makefile for an example of what Smake can do.

