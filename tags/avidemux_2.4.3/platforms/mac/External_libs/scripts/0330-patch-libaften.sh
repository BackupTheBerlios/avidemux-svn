# ------------------
#    patch aften 
# ------------------
# Harry van der Wolf, 2008

# This script will patch aften for OSX to make the PPC side compile correctly.
# It needs to be run once

patch -p0 <  "$REPOSITORYDIR/../scripts/patches/aften-0.0.8.patch" 

