# THIS FILE WAS AUTO-GENERATED
#
#  $ lcitool manifest ci/manifest.yml
#
# https://gitlab.com/libvirt/libvirt-ci

FROM docker.io/library/alpine:edge

RUN apk update && \
    apk upgrade && \
    apk add \
        ca-certificates \
        ccache \
        gcc \
        gettext \
        git \
        glib-dev \
        gobject-introspection-dev \
        gtk-doc \
        libvirt-dev \
        libxml2-dev \
        make \
        meson \
        musl-dev \
        pkgconf \
        samurai \
        vala && \
    apk list --installed | sort > /packages.txt && \
    mkdir -p /usr/libexec/ccache-wrappers && \
    ln -s /usr/bin/ccache /usr/libexec/ccache-wrappers/cc && \
    ln -s /usr/bin/ccache /usr/libexec/ccache-wrappers/gcc

ENV CCACHE_WRAPPERSDIR="/usr/libexec/ccache-wrappers"
ENV LANG="en_US.UTF-8"
ENV MAKE="/usr/bin/make"
ENV NINJA="/usr/bin/ninja"
