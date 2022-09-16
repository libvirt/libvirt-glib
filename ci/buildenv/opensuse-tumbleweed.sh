# THIS FILE WAS AUTO-GENERATED
#
#  $ lcitool manifest ci/manifest.yml
#
# https://gitlab.com/libvirt/libvirt-ci

function install_buildenv() {
    zypper dist-upgrade -y
    zypper install -y \
           ca-certificates \
           ccache \
           cppi \
           gcc \
           gettext-runtime \
           git \
           glib2-devel \
           glibc-locale \
           gobject-introspection-devel \
           gtk-doc \
           libvirt-devel \
           libxml2-devel \
           make \
           meson \
           ninja \
           pkgconfig \
           rpm-build \
           vala
    rpm -qa | sort > /packages.txt
    mkdir -p /usr/libexec/ccache-wrappers
    ln -s /usr/bin/ccache /usr/libexec/ccache-wrappers/cc
    ln -s /usr/bin/ccache /usr/libexec/ccache-wrappers/gcc
}

export CCACHE_WRAPPERSDIR="/usr/libexec/ccache-wrappers"
export LANG="en_US.UTF-8"
export MAKE="/usr/bin/make"
export NINJA="/usr/bin/ninja"
