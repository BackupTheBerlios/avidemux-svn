# ------------------
#    patch pango 
# ------------------
# Harry van der Wolf, 2008 07

# This script will patch pango for OSX 
# It needs to be run once

patch -p0 <  "$REPOSITORYDIR/../scripts/patches/pango-patch-ltmain.sh.diff" 

