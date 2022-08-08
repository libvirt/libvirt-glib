#!/bin/sh

set -e
set -v

rm -rf build

meson --prefix="`pwd`/build/vroot" -Ddocs=enabled build/native

ninja -C build/native dist

if [ -f /usr/bin/rpmbuild ]; then
  rpmbuild --nodeps \
           --define "_sourcedir `pwd`/build/native/meson-dist" \
           -ba --clean build/native/libvirt-glib.spec
fi

# Test mingw32 cross-compile
if test -x /usr/bin/i686-w64-mingw32-gcc ; then
  meson --prefix="`pwd`/build/vroot" \
        --cross-file=/usr/share/mingw/toolchain-mingw32.meson build/win32

  ninja -C build/win32
fi

# Test mingw64 cross-compile
if test -x /usr/bin/x86_64-w64-mingw32-gcc ; then
  meson --prefix="`pwd`/build/vroot" \
        --cross-file=/usr/share/mingw/toolchain-mingw64.meson build/win64

  ninja -C build/win64
fi
