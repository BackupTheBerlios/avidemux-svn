# ------------------
#     libglib
# ------------------
# Based on the works of (c) 2007, Ippei Ukai
# Modified for avidemux by Harry van der Wolf 200806

# download location ftp://ftp.gtk.org/pub/glib/

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

MAIN_LIB_VER="2.0"
EXT_MAIN_LIB_VER="2.0.0"
FULL_LIB_VER="$EXT_MAIN_LIB_VER.1400.5"



# compile

for ARCH in $ARCHS
do

 mkdir -p "$REPOSITORYDIR/arch/$ARCH/bin";
 mkdir -p "$REPOSITORYDIR/arch/$ARCH/lib";
 mkdir -p "$REPOSITORYDIR/arch/$ARCH/include";

 ARCHARGs=""
 MACSDKDIR=""
 OTHERARGs="-funroll-loops -fstrict-aliasing"
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

 export PATH=/usr/bin:$REPOSITORYDIR/bin:$PATH

 env CFLAGS="-isysroot $MACSDKDIR -arch $ARCH $ARCHARGs $OTHERARGs -O2" \
  CXXFLAGS="-isysroot $MACSDKDIR -arch $ARCH $ARCHARGs $OTHERARGs -O2" \
  CPPFLAGS="-I$REPOSITORYDIR/arch/$ARCH/include -I$REPOSITORYDIR/include -I/usr/include" \
  LDFLAGS="-L$REPOSITORYDIR/arch/$ARCH/lib -L$REPOSITORYDIR/lib -L/usr/lib -dead_strip -bind_at_load" \
  NEXT_ROOT="$MACSDKDIR" \
  PKG_CONFIG_PATH="$REPOSITORYDIR/lib/pkgconfig" \
  ./configure --disable-gtk-doc --disable-html --prefix="$REPOSITORYDIR"  --disable-dependency-tracking \
  --host="$TARGET" --exec-prefix=$REPOSITORYDIR/arch/$ARCH  --with-libiconv --enable-shared --enable-static\
 ;


 make clean;
  make $OTHERMAKEARGs;
 make $OTHERMAKEARGs install;

done


# merge libglib

for liba in lib/libglib-2.0.a lib/libgobject-2.0.a lib/libgthread-2.0.a lib/libgmodule-2.0.a lib/libglib-$FULL_LIB_VER.dylib lib/libgmodule-$FULL_LIB_VER.dylib lib/libgobject-$FULL_LIB_VER.dylib lib/libgthread-$FULL_LIB_VER.dylib
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


if [ -f "$REPOSITORYDIR/lib/libglib-$FULL_LIB_VER.dylib" ]
then
 install_name_tool -id "$REPOSITORYDIR/lib/libglib-$FULL_LIB_VER.dylib" "$REPOSITORYDIR/lib/libglib-$FULL_LIB_VER.dylib";
 ln -sfn libglib-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libglib-$EXT_MAIN_LIB_VER.dylib;
 ln -sfn libglib-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libglib-$MAIN_LIB_VER.dylib;
 ln -sfn libglib-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libglib-dylib;
fi

if [ -f "$REPOSITORYDIR/lib/libgmodule-$FULL_LIB_VER.dylib" ]
then
 install_name_tool -id "$REPOSITORYDIR/lib/libgmodule-$FULL_LIB_VER.dylib" "$REPOSITORYDIR/lib/libgmodule-$FULL_LIB_VER.dylib";
 ln -sfn libgmodule-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libgmodule-$EXT_MAIN_LIB_VER.dylib;
 ln -sfn libgmodule-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libgmodule-$MAIN_LIB_VER.dylib;
 ln -sfn libgmodule-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libgmodule-dylib;
fi

if [ -f "$REPOSITORYDIR/lib/libgobject-$FULL_LIB_VER.dylib" ]
then
 install_name_tool -id "$REPOSITORYDIR/lib/libgobject-$FULL_LIB_VER.dylib" "$REPOSITORYDIR/lib/libgobject-$FULL_LIB_VER.dylib";
 ln -sfn libgobject-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libgobject-$EXT_MAIN_LIB_VER.dylib;
 ln -sfn libgobject-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libgobject-$MAIN_LIB_VER.dylib;
 ln -sfn libgobject-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libgobject-dylib;
fi

if [ -f "$REPOSITORYDIR/lib/libgthread-$FULL_LIB_VER.dylib" ]
then
 install_name_tool -id "$REPOSITORYDIR/lib/libgthread-$FULL_LIB_VER.dylib" "$REPOSITORYDIR/lib/libgthread-$FULL_LIB_VER.dylib";
 ln -sfn libgthread-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libgthread-$EXT_MAIN_LIB_VER.dylib;
 ln -sfn libgthread-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libgthread-$MAIN_LIB_VER.dylib;
 ln -sfn libgthread-$FULL_LIB_VER.dylib $REPOSITORYDIR/lib/libgthread-dylib;
fi

# Create directory structure for big_endian / little_endian
mkdir -p $REPOSITORYDIR/lib/glib-$MAIN_LIB_VER $REPOSITORYDIR/lib/glib-$MAIN_LIB_VER/include

#pkgconfig
for ARCH in $ARCHS
do
 mkdir -p "$REPOSITORYDIR/lib/pkgconfig";
 sed 's/^exec_prefix.*$/exec_prefix=\$\{prefix\}/' "$REPOSITORYDIR/arch/$ARCH/lib/pkgconfig/glib-2.0.pc" > "$REPOSITORYDIR/lib/pkgconfig/glib-2.0.pc";
 sed 's/^exec_prefix.*$/exec_prefix=\$\{prefix\}/' "$REPOSITORYDIR/arch/$ARCH/lib/pkgconfig/gmodule-2.0.pc" > "$REPOSITORYDIR/lib/pkgconfig/gmodule-2.0.pc";
 sed 's/^exec_prefix.*$/exec_prefix=\$\{prefix\}/' "$REPOSITORYDIR/arch/$ARCH/lib/pkgconfig/gmodule-export-2.0.pc" > "$REPOSITORYDIR/lib/pkgconfig/gmodule-export-2.0.pc";
 sed 's/^exec_prefix.*$/exec_prefix=\$\{prefix\}/' "$REPOSITORYDIR/arch/$ARCH/lib/pkgconfig/gmodule-no-export-2.0.pc" > "$REPOSITORYDIR/lib/pkgconfig/gmodule-no-export-2.0.pc";
 sed 's/^exec_prefix.*$/exec_prefix=\$\{prefix\}/' "$REPOSITORYDIR/arch/$ARCH/lib/pkgconfig/gobject-2.0.pc" > "$REPOSITORYDIR/lib/pkgconfig/gobject-2.0.pc";
 sed 's/^exec_prefix.*$/exec_prefix=\$\{prefix\}/' "$REPOSITORYDIR/arch/$ARCH/lib/pkgconfig/gthread-2.0.pc" > "$REPOSITORYDIR/lib/pkgconfig/gthread-2.0.pc";
 break;
done

#Copy shell script
for ARCH in $ARCHS
do
 mkdir -p "$REPOSITORYDIR/bin";
 cp "$REPOSITORYDIR/arch/$ARCH/bin/glib-mkenums" "$REPOSITORYDIR/bin/glib-mkenums";
 cp "$REPOSITORYDIR/arch/$ARCH/bin/glib-gettextize" "$REPOSITORYDIR/bin/glib-gettextize";
 break;
done

# merge execs
for program in bin/glib-genmarshal bin/gobject-query
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

