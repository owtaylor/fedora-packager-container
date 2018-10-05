FROM fedora:28

RUN dnf -y update && \
    dnf -y --enablerepo=updates-testing install \
        coreutils-common \
        fedmod \
        fedpkg \
        flatpak-module-tools \
        less \
        python3-koji-containerbuild-cli \
        python3-service-identity \
        sudo

COPY bashrc /usr/share/fedora-packager-container/

COPY mbs-mock.cfg /etc/module-build-service/
RUN cat /etc/module-build-service/mbs-mock.cfg >> /etc/module-build-service/mock.cfg && \
    rm /etc/module-build-service/mbs-mock.cfg

RUN sed -i "s@NUM_CONCURRENT_BUILDS = 5@NUM_CONCURRENT_BUILDS = 1@" /etc/module-build-service/config.py

RUN echo "config_opts['use_nspawn'] = False" >> /usr/lib/python3.6/site-packages/flatpak_module_tools/templates/mock.cfg.j2

RUN echo /home/otaylor/Source/fedpkg/conf/etc/rpkg/fedpkg.conf