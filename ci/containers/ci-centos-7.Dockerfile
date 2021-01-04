# THIS FILE WAS AUTO-GENERATED
#
#  $ lcitool dockerfile centos-7 libvirt+dist,libvirt-glib
#
# https://gitlab.com/libvirt/libvirt-ci/-/commit/99a72b2d54f069cca979c04f1907c5444fd73b96
FROM registry.centos.org/centos:7

RUN yum update -y && \
    echo 'skip_missing_names_on_install=0' >> /etc/yum.conf && \
    yum install -y epel-release && \
    yum install -y \
        ca-certificates \
        ccache \
        gcc \
        gettext \
        git \
        glib2-devel \
        glibc-common \
        gobject-introspection-devel \
        gtk-doc \
        libvirt-devel \
        libxml2-devel \
        make \
        ninja-build \
        pkgconfig \
        python3-pip \
        python3-setuptools \
        python3-wheel \
        rpm-build \
        vala && \
    yum autoremove -y && \
    yum clean all -y && \
    rpm -qa | sort > /packages.txt && \
    mkdir -p /usr/libexec/ccache-wrappers && \
    ln -s /usr/bin/ccache /usr/libexec/ccache-wrappers/cc && \
    ln -s /usr/bin/ccache /usr/libexec/ccache-wrappers/$(basename /usr/bin/gcc)

RUN pip3 install \
         meson==0.54.0

ENV LANG "en_US.UTF-8"
ENV MAKE "/usr/bin/make"
ENV NINJA "/usr/bin/ninja-build"
ENV CCACHE_WRAPPERSDIR "/usr/libexec/ccache-wrappers"
