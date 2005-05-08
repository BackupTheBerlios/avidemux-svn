#!/bin/bash
export PRG=adm2_s
export SRC=./
echo "Running tests..."
rm -f out/* /tmp/md5
perl foreach.pl $SRC/script/* "echo %f && adm2_s    --run script/%f >& /dev/null"
echo "## ALL done checking...##"
md5sum out/*  > /tmp/md5
		
