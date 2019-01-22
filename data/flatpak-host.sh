#!/usr/bin/bash

case $1 in
    build-*)
        exec -a $0 /usr/bin/flatpak.real "$@"
        ;;
    *)
        exec flatpak-spawn --host /usr/bin/flatpak "$@"
        ;;
esac
