# ------------------
#    ncurses 
# ------------------
# Based on the works of (c) 2007, Ippei Ukai
# Modified for avidemux by Harry van der Wolf 200806

# download location ftp://ftp.gnu.org/gnu/ncurses

# ncursesw and ncurses are both run from the same ncurses tar.gz and
# from the same directory.
# The ncursesw script needs to be run before the ncurses script
# and comes with one extra compile setting --enable-widec


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

MAIN_LIB_VER="5"
#FULL_LIB_VER="$MAIN_LIB_VER.6"


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


 #patch first
 #patch -p0 < patch-configure 

 export PATH=/usr/bin:$PATH

 env CFLAGS="-isysroot $MACSDKDIR -arch $ARCH $ARCHARGs $OTHERARGs -O2 -dead_strip" \
  CXXFLAGS="-isysroot $MACSDKDIR -arch $ARCH $ARCHARGs $OTHERARGs -O2 -dead_strip" \
  CPPFLAGS="-I$REPOSITORYDIR/include -I/usr/include -no-cpp-precomp" \
  LDFLAGS="-L$REPOSITORYDIR/lib -L/usr/lib -dead_strip -prebind" \
  NEXT_ROOT="$MACSDKDIR" \
  ./configure --prefix="$REPOSITORYDIR" --disable-dependency-tracking \
  --host="$TARGET" --exec-prefix=$REPOSITORYDIR/arch/$ARCH \
  --with-shared --disable-rpath --without-progs \
  --without-debug --without-ada \
  --enable-safe-sprintf --enable-sigwinch \
;

 make clean;
 make;
 make install;

done


# merge libs

for liba in lib/libncurses.a lib/libncurses.$MAIN_LIB_VER.dylib lib/libpanel.a lib/libpanel.$MAIN_LIB_VER.dylib lib/libmenu.a lib/libmenu.$MAIN_LIB_VER.dylib lib/libform.a lib/libform.$MAIN_LIB_VER.dylib
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

if [ -f "$REPOSITORYDIR/lib/libncurses.$MAIN_LIB_VER.dylib" ]
then
 install_name_tool -id "$REPOSITORYDIR/lib/libncurses.$MAIN_LIB_VER.dylib" "$REPOSITORYDIR/lib/libncurses.$MAIN_LIB_VER.dylib"
 ln -sfn libncurses.$MAIN_LIB_VER.dylib $REPOSITORYDIR/lib/libncurses.dylib;
fi

if [ -f "$REPOSITORYDIR/lib/libpanel.$MAIN_LIB_VER.dylib" ]
then
 install_name_tool -id "$REPOSITORYDIR/lib/libpanel.$MAIN_LIB_VER.dylib" "$REPOSITORYDIR/lib/libpanel.$MAIN_LIB_VER.dylib"
 ln -sfn libpanel.$MAIN_LIB_VER.dylib $REPOSITORYDIR/lib/libpanel.dylib;
fi

if [ -f "$REPOSITORYDIR/lib/libmenu.$MAIN_LIB_VER.dylib" ]
then
 install_name_tool -id "$REPOSITORYDIR/lib/libmenu.$MAIN_LIB_VER.dylib" "$REPOSITORYDIR/lib/libmenu.$MAIN_LIB_VER.dylib"
 ln -sfn libmenu.$MAIN_LIB_VER.dylib $REPOSITORYDIR/lib/libmenu.dylib;
fi

if [ -f "$REPOSITORYDIR/lib/libform.$MAIN_LIB_VER.dylib" ]
then
 install_name_tool -id "$REPOSITORYDIR/lib/libform.$MAIN_LIB_VER.dylib" "$REPOSITORYDIR/lib/libform.$MAIN_LIB_VER.dylib"
 ln -sfn libform.$MAIN_LIB_VER.dylib $REPOSITORYDIR/lib/libform.dylib;
fi

#Copy shell script
for ARCH in $ARCHS
do
 mkdir -p "$REPOSITORYDIR/bin";
 sed 's/^exec_prefix.*$/exec_prefix=\$\{prefix\}/' "$REPOSITORYDIR/arch/$ARCH/bin/ncurses5-config" > "$REPOSITORYDIR/bin/ncurses5-config";
 break;
done


