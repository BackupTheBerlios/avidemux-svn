#!/bin/bash
fail()
{
        echo "*** fail $1 ***"
        exit 1
        cd $TOP
}

export TOP=$PWD
echo "* Avidemux simple build script *"
echo "* Building Main *"
rm -Rf buildMain
mkdir -p buildMain
cd buildMain
cmake .. || fail
make || fail
echp "*  Main Ok*"
echp "*  All Done  *"

