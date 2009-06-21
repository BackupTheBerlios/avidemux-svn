# ------------------
#    aften 
# ------------------
# Based on the works of (c) 2007, Ippei Ukai
# Modified for avidemux by Harry van der Wolf 200806

# download location http://aften.sourceforge.net/

# prepare

# export REPOSITORYDIR="/PATH2HUGIN/mac/ExternalPrograms/repository" \
# ARCHS="ppc i386" \
#  ppcTARGET="powerpc-apple-darwin8" \
#  i386TARGET="i386-apple-darwin8" \
#  ppcMACSDKDIR="/Developer/SDKs/MacOSX10.4u.sdk" \
#  i386MACSDKDIR="/Developer/SDKs/MacOSX10.4u.sdk" \
#  ppcONLYARG="-mcpu=G3 -mtune=G4" \
#  i386ONLYARG="-mfpmath=sse -msse2 -mtune=pentium-m -ftree-vectorize" \
#  OTHERARGs="";


# init

let NUMARCH="0"

for i in $ARCHS
do
  NUMARCH=$(($NUMARCH + 1))
done

mkdir -p "$REPOSITORYDIR/bin";
mkdir -p "$REPOSITORYDIR/lib";
mkdir -p "$REPOSITORYDIR/include";

MAIN_LIB_VER="0"
FULL_LIB_VER="$MAIN_LIB_VER.0.8"


# compile

for ARCH in $ARCHS
do

 mkdir -p "$REPOSITORYDIR/arch/$ARCH/bin";
 mkdir -p "$REPOSITORYDIR/arch/$ARCH/lib";
 mkdir -p "$REPOSITORYDIR/arch/$ARCH/include";

 ARCHARGs=""
 MACSDKDIR=""

 if [ $ARCH = "i386" -o $ARCH = "i686" ]
 then
  TARGET=$i386TARGET
  MACSDKDIR=$i386MACSDKDIR
  ARCHARGs="$i386ONLYARG"
 elif [ $ARCH = "ppc" -o $ARCH = "ppc750" -o $ARCH = "ppc7400" ]
 then
  TARGET=$ppcTARGET
  MACSDKDIR=$ppcMACSDKDIR
  ARCHARGs="$ppcONLYARG"
 elif [ $ARCH = "ppc64" -o $ARCH = "ppc970" ]
 then
  TARGET=$ppc64TARGET
  MACSDKDIR=$ppc64MACSDKDIR
  ARCHARGs="$ppc64ONLYARG"
 elif [ $ARCH = "x86_64" ]
 then
  TARGET=$x64TARGET
  MACSDKDIR=$x64MACSDKDIR
  ARCHARGs="$x64ONLYARG"
 fi


 mkdir -p $ARCH;
 cd $ARCH;
 rm CMakeCache.txt;

export CMAKE_INCLUDE_PATH="$REPOSITORYDIR/include"
export CMAKE_LIBRARY_PATH="$REPOSITORYDIR/lib"
export  LDFLAGS="-L$REPOSITORYDIR/lib -dead_strip" \
export  NEXT_ROOT="$MACSDKDIR" \
export  PKG_CONFIG_PATH=$REPOSITORYDIR/lib/pkgconfig \
export PATH=/usr/bin:$PATH


cmake  \
  -DCMAKE_INSTALL_PREFIX="$REPOSITORYDIR" \
  -DCMAKE_OSX_ARCHITECTURES="$TARGET" \
  -DCMAKE_OSX_SYSROOT="$MACSDKDIR"\
   -DSHARED=OFF \
  -DCMAKE_C_FLAGS="-arch $ARCH -fno-common -O2 -dead_strip" \
  -DCMAKE_CXX_FLAGS="-arch $ARCH -fno-common -O2 -dead_strip" \
  ..;

#   -DSHARED=ON \


 make;
 make install;
 cd ..
done


# merge libaften

for liba in lib/libaften.a lib/libaften.$FULL_LIB_VER.dylib   
do

 if [ $NUMARCH -eq 1 ]
 then
  mv "$REPOSITORYDIR/arch/$ARCHS/$liba" "$REPOSITORYDIR/$liba";
  ranlib "$REPOSITORYDIR/$liba";
  continue
 fi

 LIPOARGs=""
 
 for ARCH in $ARCHS
 do
  LIPOARGs="$LIPOARGs $REPOSITORYDIR/arch/$ARCH/$liba"
 done

 lipo $LIPOARGs -create -output "$REPOSITORYDIR/$liba";
 ranlib "$REPOSITORYDIR/$liba";

done

if [ -f "$REPOSITORYDIR/lib/libaften.$FULL_LIB_VER.dylib" ]
then
 install_name_tool -id "$REPOSITORYDIR/lib/libaften.$FULL_LIB_VER.dylib" "$REPOSITORYDIR/lib/libaften.$FULL_LIB_VER.dylib"
 ln -sfn $REPOSITORYDIR/lib/libaften.$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libaften.$MAIN_LIB_VER.dylib
 ln -sfn $REPOSITORYDIR/lib/libaften.$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libaften.dylib;
fi

