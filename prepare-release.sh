#!/bin/sh

set -e
set -v

test -n "$1" && RESULTS=$1 || RESULTS=results.log

# Make things clean.
test -f Makefile && make -k distclean || :

rm -rf build
mkdir build
cd build

../autogen.sh --prefix=$HOME/builder \
    --enable-gtk-doc

make
make install

# set -o pipefail is a bashism; this use of exec is the POSIX alternative
exec 3>&1
st=$(
  exec 4>&1 >&3
  { make check syntax-check 2>&1 3>&- 4>&-; echo $? >&4; } | tee "$RESULTS"
)
exec 3>&-
test "$st" = 0

rm -f *.tar.gz
make dist

if [ -f /usr/bin/rpmbuild ]; then
  rpmbuild --nodeps \
     --define "_sourcedir `pwd`" \
     -ba --clean libvirt-glib.spec
fi

# Test mingw32 cross-compile
if test -x /usr/bin/i686-w64-mingw32-gcc ; then
  make distclean

  PKG_CONFIG_PATH="$HOME/builder/i686-w64-mingw32/sys-root/mingw/lib/pkgconfig" \
  CC="i686-w64-mingw32-gcc" \
  ../configure \
    --build=$(uname -m)-pc-linux \
    --host=i686-w64-mingw32 \
    --prefix="$HOME/builder/i686-w64-mingw32/sys-root/mingw" \
    --enable-werror \
    --enable-introspection=no

  make
  make install

fi

# Test mingw64 cross-compile
if test -x /usr/bin/x86_64-w64-mingw32-gcc ; then
  make distclean

  PKG_CONFIG_PATH="$HOME/builder/x86_64-w64-mingw32/sys-root/mingw/lib/pkgconfig" \
  CC="x86_64-w64-mingw32-gcc" \
  ../configure \
    --build=$(uname -m)-pc-linux \
    --host=x86_64-w64-mingw32 \
    --prefix="$HOME/builder/i686-w64-mingw32/sys-root/mingw" \
    --enable-werror \
    --enable-introspection=no

  make
  make install

fi

if test -x /usr/bin/i686-w64-mingw32-gcc && test -x /usr/bin/x86_64-w64-mingw32-gcc ; then
  if test -f /usr/bin/rpmbuild ; then
    rpmbuild --nodeps \
       --define "_sourcedir `pwd`" \
       -ba --clean mingw-libvirt-glib.spec
  fi
fi
