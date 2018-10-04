fedora-packager-container
=========================
This project contains a command line client `fedpkg-c` that runs a container set up for Fedora contributor development - RPMs, Flatpaks, Containers, etc.

Installing via flatpak
======================
You can build and install a Flatpak that wraps fedpkg-c by running `flatpak.sh` in this directory. (This is meant for development - eventually the Flatpak would be distributed pre-built.)

podman and sudo
===============
Hopefully we'll eventually be able use the rootless mode of podman together with mock, but at that moment, the container needs to be executed as root to map  users correctly and get the necessary permissions to create a chroot, so `fedpkg-c` runs podman as 'sudo podman'.

Command line usage
==================
`fedpkg-c configure`: prompts for some basic configuration parameters. (Automatically is run the first time)
`fedpkg-c build`: builds the container
`fedpkg-c shell`: run a shell in the container - starting the container in the background
`fedpkg-c destroy`: stop the background container, and remove it.

License and Copyright
=====================
fedora-packager-container is copyright Owen Taylor <otaylor@fishsoup.net> and
Red Hat, Inc., and licensed under the terms of the GNU General Public License,
version 2 or later.
