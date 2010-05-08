# ------------------
#    libvxid 
# ------------------
# Based on the works of (c) 2007, Ippei Ukai
# Created for avidemux by Harry van der Wolf

# download location daily build http://downloads.xvid.org/downloads/xvid_latest.tar.gz
# or download latest source from CVS
# cvs -d:pserver:anonymous@cvs.xvid.org:/xvid co xvidcore

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

#MAIN_LIB_VER="4"
FULL_LIB_VER="$MAIN_LIB_VER.4"

# Xvid is a little strange. We need to dive into the tree and do a make clean (just in case)
cd xvidcore/build/generic
make clean

# As we are using a nightly build or a CVS we need to run bootstrap first
./bootstrap.sh


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
   ExtraARG="--disable-assembly"
 elif [ $ARCH = "ppc" -o $ARCH = "ppc750" -o $ARCH = "ppc7400" ]
 then
  TARGET=$ppcTARGET
  MACSDKDIR=$ppcMACSDKDIR
  ARCHARGs="$ppcONLYARG $ppcONLYExtraARG"
  ExtraARG="-maltivec"
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


 env CFLAGS="-isysroot $MACSDKDIR -arch $ARCH $ARCHARGs $OTHERARGs -O2 -dead_strip" \
  CXXFLAGS="-isysroot $MACSDKDIR -arch $ARCH $ARCHARGs $OTHERARGs -O2 -dead_strip" \
  CPPFLAGS="-I$REPOSITORYDIR/include" \
  LDFLAGS="-L$REPOSITORYDIR/lib -isysroot $MACSDKDIR" \
  NEXT_ROOT="$MACSDKDIR" \
  ./configure --prefix="$REPOSITORYDIR" \
  --host="$TARGET" --target="$TARGET" CC="gcc -arch $ARCH $ExtraARG" --exec-prefix=$REPOSITORYDIR/arch/$ARCH \
  --enable-shared --enable-static \
;


# make clean;
 make;
 make install;
 make clean;

done


# merge libvxid

for liba in lib/libvxid.a lib/libvxid.$FULL_LIB_VER.dylib   
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

if [ -f "$REPOSITORYDIR/lib/libvxid.$FULL_LIB_VER.dylib" ]
then
 install_name_tool -id "$REPOSITORYDIR/lib/libvxid.$FULL_LIB_VER.dylib" "$REPOSITORYDIR/lib/libvxid.$FULL_LIB_VER.dylib"
 ln -sfn $REPOSITORYDIR/lib/libvxid.$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libvxid.dylib;
fi

