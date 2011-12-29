#!/bin/bash
#
fail()
{
        echo "*** fail $1 ***"
        exit 1
        cd $TOP
}
export PREFIX=/usr
export TOP=$PWD
echo "* Avidemux simple build script *"
echo "* Need sudo to install libs *"
sudo -v
echo "* Building Main *"
rm -Rf buildMain_debug
mkdir -p buildMain_debug
cd buildMain_debug
cmake -DCMAKE_EDIT_COMMAND=vim -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$PREFIX -DAVIDEMUX_INSTALL_PREFIX=$PREFIX -G "CodeBlocks - Unix Makefiles" .. || fail cmake
make -j 3 || fail make_main
sudo make install || fail install_main
sudo ldconfig
echo "*  Main Ok*"
cd $TOP
echo "* Making plugins *"
rm -Rf buildPlugins_debug
mkdir -p buildPlugins_debug
cd buildPlugins_debug
cmake -DCMAKE_EDIT_COMMAND=vim -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$PREFIX -DAVIDEMUX_INSTALL_PREFIX=$PREFIX  -DAVIDEMUX_SOURCE_DIR=$TOP/  -DAVIDEMUX_CORECONFIG_DIR=$TOP/buildMain_debug/config -G "CodeBlocks - Unix Makefiles" ../plugins || fail cmake_plugins
make -j 3  || fail make_plugins
sudo make install
echo "*  All Done  *"


