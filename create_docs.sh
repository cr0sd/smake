#!/usr/bin/env bash

cat head > doc.md
smake --help | tee -a doc.md
cat foot | tee -a doc.md
