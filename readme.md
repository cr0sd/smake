### smake

Make utility that can list the targets contained in 
the makefile.

## Features include

* Single target

* Silent commands/rules (prefixed with `@`)

* Multiple target dependencies

* Dry-run (show rules but don't execute them)

* List all targets within makefile

## Notes

For now only one build target can be specified at a time.

Variables cannot be used yet.

Only `makefile` is searched for at the moment (i.e., `Makefile` isn't searched for)

