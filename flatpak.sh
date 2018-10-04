#!/bin/sh

set -e -x

[ -e _flatpak ] || mkdir _flatpak
cd _flatpak
manifest=../org.fedoraproject.PackagerContainer.json
stamp=install-base-stamp
if [ -e $stamp -a $stamp -nt $manifest ] ; then
    :
else
    rm -rf install-base && mkdir -p install-base
    flatpak-builder --stop-at=fedora-packager-container install-base $manifest
    touch install-base-stamp
fi

rm -rf install
cp -al install-base install
mkdir -p build
flatpak build --build-dir=$(pwd)/build install sh -c '( [ -e build.ninja ] || meson --prefix=/app ../.. ) && ninja && meson install'
flatpak-builder --repo=repo --finish-only --disable-cache install ../org.fedoraproject.PackagerContainer.json
flatpak build-update-repo repo

flatpak --user uninstall -y org.fedoraproject.PackagerContainer > /dev/null || true
flatpak --user remote-add --if-not-exists --no-gpg-verify PackagerContainer-devel $(pwd)/repo
flatpak --user install -y PackagerContainer-devel org.fedoraproject.PackagerContainer
