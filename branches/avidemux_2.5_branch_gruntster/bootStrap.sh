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
cmake -DCMAKE_INSTALL_PREFIX=/usr -DAVIDEMUX_INSTALL_PREFIX=/usr/ .. || fail cmake
make || fail make_main
sudo make install | fail install_main
echo "*  Main Ok*"
cd $TOP
echo "* Making plugins *"
rm -Rf buildPlugins
mkdir -p buildPlugins
cd buildPlugins
cmake -DAVIDEMUX_INSTALL_PREFIX=/usr/  -DAVIDEMUX_SOURCE_DIR=$TOP/  -DAVIDEMUX_CORECONFIG_DIR=$TOP/buildMain/config ../plugins || fail cmake_plugins
make || fail make_plugins
echo "*  All Done  *"


