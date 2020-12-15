# THIS FILE WAS AUTO-GENERATED
#
#  $ lcitool dockerfile opensuse-151 libvirt+dist,libvirt-glib
#
# https://gitlab.com/libvirt/libvirt-ci/-/commit/b098ec6631a85880f818f2dd25c437d509e53680
FROM registry.opensuse.org/opensuse/leap:15.1

RUN zypper update -y && \
    zypper install -y \
           autoconf \
           automake \
           ca-certificates \
           ccache \
           gcc \
           gettext \
           gettext-devel \
           git \
           glib2-devel \
           glibc-locale \
           gobject-introspection-devel \
           gtk-doc \
           libtool \
           libvirt-devel \
           libxml2-devel \
           make \
           pkgconfig \
           rpm-build \
           vala && \
    zypper clean --all && \
    rpm -qa | sort > /packages.txt && \
    mkdir -p /usr/libexec/ccache-wrappers && \
    ln -s /usr/bin/ccache /usr/libexec/ccache-wrappers/cc && \
    ln -s /usr/bin/ccache /usr/libexec/ccache-wrappers/$(basename /usr/bin/gcc)

ENV LANG "en_US.UTF-8"
ENV MAKE "/usr/bin/make"
ENV CCACHE_WRAPPERSDIR "/usr/libexec/ccache-wrappers"
