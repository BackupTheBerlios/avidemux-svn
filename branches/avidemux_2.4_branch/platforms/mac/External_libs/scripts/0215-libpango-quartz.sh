# ------------------
#     libpango
# ------------------
# Based on the works of (c) 2007, Ippei Ukai
# Modified for avidemux by Harry van der Wolf

# download location http://ftp.gnome.org/pub/GNOME/sources/pango

# prepare

# export REPOSITORYDIR="/PATH2HUGIN/mac/ExternalPrograms/repository" \
# ARCHS="ppc i386" \
# ppcTARGET="powerpc-apple-darwin7" \
# i386TARGET="i386-apple-darwin8" \
#  ppcMACSDKDIR="/Developer/SDKs/MacOSX10.4u.sdk" \
#  i386MACSDKDIR="/Developer/SDKs/MacOSX10.3.9.sdk" \
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

MAIN_LIB_VER="1.0"
EXT_MAIN_LIB_VER="1.0.0"
FULL_LIB_VER="$EXT_MAIN_LIB_VER.2002.1"



# compile

for ARCH in $ARCHS
do

 mkdir -p "$REPOSITORYDIR/arch/$ARCH/bin";
 mkdir -p "$REPOSITORYDIR/arch/$ARCH/lib";
 mkdir -p "$REPOSITORYDIR/arch/$ARCH/include";

 ARCHARGs=""
 MACSDKDIR=""
 #OTHERARGs="-no-cpp-precomp"
 OTHERARGs=""
 OTHERMAKEARGs=""

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

 export PATH=/usr/bin:$REPOSITORYDIR/arch/$ARCH/bin:$PATH

# The separated build trees should guarantee that the right endian value is used
# but just to make sure that the right endian is compiled we copy the glibconfig.h
# into $REPOSITORYDIR/lib/glib-2.0/include
# cp $REPOSITORYDIR/arch/$ARCH/lib/glib-2.0/include/glibconfig.h $REPOSITORYDIR/lib/glib-2.0/include

# env CFLAGS="-isysroot $MACSDKDIR -arch $ARCH $ARCHARGs $OTHERARGs -O2" \
#  CXXFLAGS="-isysroot $MACSDKDIR -arch $ARCH $ARCHARGs $OTHERARGs -O2" \
#  CPPFLAGS="-I$REPOSITORYDIR/include -I$REPOSITORYDIR/arch/$ARCH/include -I/usr/include" \
#  LDFLAGS="-L$REPOSITORYDIR/lib -L$REPOSITORYDIR/arch/$ARCH/lib -L/usr/lib -dead_strip -arch $ARCH -no-undefined -bind_at_load" \
#  NEXT_ROOT="$MACSDKDIR" \
#  PKG_CONFIG_PATH="$REPOSITORYDIR/arch/$ARCH/lib/pkgconfig:/usr/lib/pkgconfig" \


 env CFLAGS="-isysroot $MACSDKDIR -arch $ARCH $ARCHARGs $OTHERARGs -O2" \
  CXXFLAGS="-isysroot $MACSDKDIR -arch $ARCH $ARCHARGs $OTHERARGs -O2" \
  CPPFLAGS="-I$REPOSITORYDIR/include -I$REPOSITORYDIR/arch/$ARCH/include -I/usr/include" \
  LDFLAGS="-L$REPOSITORYDIR/lib -L$REPOSITORYDIR/arch/$ARCH/lib -L/usr/lib -dead_strip -arch $ARCH" \
  NEXT_ROOT="$MACSDKDIR" \
  PKG_CONFIG_PATH="$REPOSITORYDIR/arch/$ARCH/lib/pkgconfig:/usr/lib/pkgconfig" \
  ./configure --prefix="$REPOSITORYDIR"  --disable-dependency-tracking \
  --host="$TARGET" --exec-prefix=$REPOSITORYDIR/arch/$ARCH \
  --without-x --disable-gtk-doc --enable-static --enable-shared \
 ;

 make clean;
 make $OTHERMAKEARGs;
 make $OTHERMAKEARGs install;

# Hack to get all modules copied to an intermediate place. This hack is based on a 2 architecture (i386 and ppc)
# build only
mkdir -p ~/tmp ~/tmp/modules
for f in $(find modules -name '*.so'); do cp $f ~/tmp/pango-modules; done

# Remove glibconfig.h again from $REPOSITORYDIR/lib/glib-2.0/include
 rm $REPOSITORYDIR/lib/glib-2.0/include/glibconfig.h
done


# merge libpango

for liba in lib/libpango.a lib/libpango-$FULL_LIB_VER.dylib lib/libpangocairo-$FULL_LIB_VER.dylib lib/libpangoft2-$FULL_LIB_VER.dylib
do

 if [ $NUMARCH -eq 1 ]
 then
  mv "$REPOSITORYDIR/arch/$ARCHS/$liba" "$REPOSITORYDIR/$liba";
  if [[ $liba == *.a ]]
  then 
   ranlib "$REPOSITORYDIR/$liba";
  fi
  continue
 fi

 LIPOARGs=""
 
 for ARCH in $ARCHS
 do
  LIPOARGs="$LIPOARGs $REPOSITORYDIR/arch/$ARCH/$liba"
 done

 lipo $LIPOARGs -create -output "$REPOSITORYDIR/$liba";
 if [[ $liba == *.a ]]
 then 
  ranlib "$REPOSITORYDIR/$liba";
 fi

done


if [ -f "$REPOSITORYDIR/lib/libpango-$FULL_LIB_VER.dylib" ]
then
 install_name_tool -id "$REPOSITORYDIR/lib/libpango-$FULL_LIB_VER.dylib" "$REPOSITORYDIR/lib/libpango-$FULL_LIB_VER.dylib";
 ln -sfn libpango-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libpango-$EXT_MAIN_LIB_VER.dylib;
 ln -sfn libpango-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libpango-$MAIN_LIB_VER.dylib;
 ln -sfn libpango-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libpango-dylib;
fi

if [ -f "$REPOSITORYDIR/lib/libpangocairo-$FULL_LIB_VER.dylib" ]
then
 install_name_tool -id "$REPOSITORYDIR/lib/libpangocairo-$FULL_LIB_VER.dylib" "$REPOSITORYDIR/lib/libpangocairo-$FULL_LIB_VER.dylib";
 ln -sfn libpangocairo-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libpangocairo-$EXT_MAIN_LIB_VER.dylib;
 ln -sfn libpangocairo-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libpangocairo-$MAIN_LIB_VER.dylib;
 ln -sfn libpangocairo-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libpangocairo-dylib;
fi

if [ -f "$REPOSITORYDIR/lib/libpangoft2-$FULL_LIB_VER.dylib" ]
then
 install_name_tool -id "$REPOSITORYDIR/lib/libpangoft2-$FULL_LIB_VER.dylib" "$REPOSITORYDIR/lib/libpangoft2-$FULL_LIB_VER.dylib";
 ln -sfn libpangoft2-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libpangoft2-$EXT_MAIN_LIB_VER.dylib;
 ln -sfn libpangoft2-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libpangoft2-$MAIN_LIB_VER.dylib;
 ln -sfn libpangoft2-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libpangoft2-dylib;
fi

# Now we copy pango modules into place using the hack from before and doing it by an even dirtier hack
for f in $(find ~/tmp/i386-modules/modules -name '*.so'); do sofile=`basename $f`; sudo lipo -create $f ~/tmp/pango-modules/$sofile -output $REPOSITORYDIR/lib/pango/1.6.0/modules/$sofile; done
# Remove intermediate modules from first hack
# rm -rf ~/tmp/pango-modules

