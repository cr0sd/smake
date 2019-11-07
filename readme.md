### smake

Make utility that can list the targets contained in 
the makefile.

```bash
smake 0.5.1 
Build Nov  6 2019 21:06:03
usage: smake [options] [target]
-h, --help			This help
-v, --version			Show version info
-f, --file FILE			Use FILE as makefile
-c				Set target to 'clean'
-p				Print internal data
-P				Alias to '-np'
-R, --no-builtin-variables	Disable built-in macros
-n, --just-print		Print rules (do not execute)
```

### Features include

* Single target

* Silent commands/rules (prefixed with `@`)

* Multiple target dependencies

* Dry-run (show rules but don't execute them)

* Use `-c` to set target to `clean`

* Use `-p` to print rules and macros defined in makefile (or built-in)

### Notes

For now only one build target can be specified at a time.

