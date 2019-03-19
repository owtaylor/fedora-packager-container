FROM fedora:29

RUN dnf -y update && \
    dnf -y --enablerepo=updates-testing install \
        bash-completion \
        coreutils-common \
        fedmod \
        fedpkg \
        flatpak-module-tools \
        flatpak-xdg-utils \
        less \
        python3-koji-containerbuild-cli \
        python3-service-identity \
        python3-pungi \
        sudo

COPY bashrc /usr/share/fedora-packager-container/

RUN mv /usr/bin/flatpak /usr/bin/flatpak.real
COPY flatpak-host.sh /usr/bin/flatpak

COPY mbs-mock.cfg /etc/module-build-service/
RUN cat /etc/module-build-service/mbs-mock.cfg >> /etc/module-build-service/mock.cfg && \
    rm /etc/module-build-service/mbs-mock.cfg

RUN sed -i "s@NUM_CONCURRENT_BUILDS = 5@NUM_CONCURRENT_BUILDS = 1@" /etc/module-build-service/config.py
RUN sed -i "s@RPMS_ALLOW_REPOSITORY = False@RPMS_ALLOW_REPOSITORY = True@" /etc/module-build-service/config.py
RUN sed -i "s@git://pkgs.fedoraproject.org@https://src.fedoraproject.org@" /etc/module-build-service/config.py
RUN sed -i "s@# config_opts\['use_nspawn'\] = True@config_opts['use_nspawn'] = False@" /etc/mock/site-defaults.cfg

RUN echo "config_opts['use_nspawn'] = False" >> /usr/lib/python3.7/site-packages/flatpak_module_tools/templates/mock.cfg.j2

RUN echo /home/otaylor/Source/fedpkg/conf/etc/rpkg/fedpkg.conf
