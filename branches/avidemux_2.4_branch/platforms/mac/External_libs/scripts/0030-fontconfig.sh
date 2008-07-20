# ------------------
#    fontconfig 
# ------------------
# Based on the works of (c) 2007, Ippei Ukai
# Modified for avidemux by Harry van der Wolf 200806

# download location http://fontconfig.org/release/

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

MAIN_LIB_VER="1"
FULL_LIB_VER="$MAIN_LIB_VER.3.0"


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

export PATH=/usr/bin:$REPOSITORYDIR/bin:$PATH


 env CFLAGS="-isysroot $MACSDKDIR -arch $ARCH $ARCHARGs $OTHERARGs -O2 -dead_strip" \
  CXXFLAGS="-isysroot $MACSDKDIR -arch $ARCH $ARCHARGs $OTHERARGs -O2 -dead_strip" \
  CPPFLAGS="-I$REPOSITORYDIR/include -I/usr/include -no-cpp-precomp" \
  LDFLAGS="-L$REPOSITORYDIR/lib -L/usr/lib -dead_strip" \
  NEXT_ROOT="$MACSDKDIR" \
  PKG_CONFIG_PATH=$REPOSITORYDIR/lib/pkgconfig \
  ./configure --prefix="$REPOSITORYDIR" --disable-dependency-tracking \
  --host="$TARGET" --target="$TARGET" --exec-prefix=$REPOSITORYDIR/arch/$ARCH \
  --with-add-fonts=/Library/Fonts,/Network/Library/Fonts,/System/Library/Fonts,${prefix}/share/fonts \
  --enable-shared  --disable-docs --with-freetype-config=$REPOSITORYDIR/arch/$ARCH/bin/freetype-config \
;
#   ./configure --prefix="$REPOSITORYDIR" --disable-dependency-tracking --enable-libxml2 \


 make clean;
 make;
 make install;

done


# merge libfontconfig

for liba in lib/libfontconfig.a lib/libfontconfig.$FULL_LIB_VER.dylib   
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

if [ -f "$REPOSITORYDIR/lib/libfontconfig.$FULL_LIB_VER.dylib" ]
then
 install_name_tool -id "$REPOSITORYDIR/lib/libfontconfig.$FULL_LIB_VER.dylib" "$REPOSITORYDIR/lib/libfontconfig.$FULL_LIB_VER.dylib"
 ln -sfn $REPOSITORYDIR/lib/libfontconfig.$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libfontconfig.$MAIN_LIB_VER.dylib
 ln -sfn $REPOSITORYDIR/lib/libfontconfig.$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libfontconfig.dylib;
fi

#pkgconfig
for ARCH in $ARCHS
do
 mkdir -p "$REPOSITORYDIR/lib/pkgconfig";
 sed 's/^exec_prefix.*$/exec_prefix=\$\{prefix\}/' "$REPOSITORYDIR/arch/$ARCH/lib/pkgconfig/fontconfig.pc" > "$REPOSITORYDIR/lib/pkgconfig/fontconfig.pc.tmp";
 # fontconfig needs an extra step
 sed 's+arch/ppc/++' "$REPOSITORYDIR/lib/pkgconfig/fontconfig.pc.tmp" > "$REPOSITORYDIR/lib/pkgconfig/fontconfig.pc";
 rm "$REPOSITORYDIR/lib/pkgconfig/fontconfig.pc.tmp"
break;
done

# merge execs
for program in bin/fc-cache bin/fc-cat bin/fc-list bin/fc-match
do

 if [ $NUMARCH -eq 1 ]
 then
  mv "$REPOSITORYDIR/arch/$ARCHS/$program" "$REPOSITORYDIR/$program";
  strip "$REPOSITORYDIR/$program";
  continue
 fi

 LIPOARGs=""

 for ARCH in $ARCHS
 do
  LIPOARGs="$LIPOARGs $REPOSITORYDIR/arch/$ARCH/$program"
 done

 lipo $LIPOARGs -create -output "$REPOSITORYDIR/$program";

 strip "$REPOSITORYDIR/$program";

done


