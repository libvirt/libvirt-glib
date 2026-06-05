# THIS FILE WAS AUTO-GENERATED
#
#  $ lcitool manifest ci/manifest.yml
#
# https://gitlab.com/libvirt/libvirt-ci

FROM registry.fedoraproject.org/fedora:rawhide

RUN dnf --quiet update -y --nogpgcheck fedora-gpg-keys && \
    dnf --quiet install -y nosync && \
    printf '#!/bin/sh\n\
if test -d /usr/lib64\n\
then\n\
    export LD_PRELOAD=/usr/lib64/nosync/nosync.so\n\
else\n\
    export LD_PRELOAD=/usr/lib/nosync/nosync.so\n\
fi\n\
exec "$@"\n' > /usr/bin/nosync && \
    chmod +x /usr/bin/nosync && \
    nosync dnf --quiet distro-sync -y && \
    nosync dnf --quiet install -y \
                       ca-certificates \
                       ccache \
                       cppi \
                       git \
                       glib2-devel \
                       glibc-langpack-en \
                       gtk-doc \
                       make \
                       meson \
                       ninja-build \
                       rpm-build \
                       vala && \
    nosync dnf --quiet autoremove -y && \
    nosync dnf --quiet clean all -y

ENV CCACHE_WRAPPERSDIR="/usr/libexec/ccache-wrappers"
ENV LANG="en_US.UTF-8"
ENV MAKE="/usr/bin/make"
ENV NINJA="/usr/bin/ninja"

RUN nosync dnf --quiet install -y \
                       mingw64-gcc \
                       mingw64-gettext \
                       mingw64-glib2 \
                       mingw64-headers \
                       mingw64-libvirt \
                       mingw64-libxml2 \
                       mingw64-pkg-config && \
    nosync dnf --quiet clean all -y && \
    rpm -qa | sort > /packages.txt && \
    mkdir -p /usr/libexec/ccache-wrappers && \
    ln -s /usr/bin/ccache /usr/libexec/ccache-wrappers/x86_64-w64-mingw32-cc && \
    ln -s /usr/bin/ccache /usr/libexec/ccache-wrappers/x86_64-w64-mingw32-gcc

ENV ABI="x86_64-w64-mingw32"
ENV MESON_OPTS="--cross-file=/usr/share/mingw/toolchain-mingw64.meson"
