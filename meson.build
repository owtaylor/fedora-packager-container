project('fedora-packager-container', 'c')

executable('run-host',
           'src/host-command.c',
           'src/host-command.h',
           'src/run-host.c',
           dependencies: dependency('gio-unix-2.0'),
           install : true,
           install_dir : join_paths(get_option('libexecdir'), 'fedora-packager-container'))

imagedir=join_paths(get_option('datadir'), 'fedora-packager-container', 'image')
install_data('data/Dockerfile', 'data/bashrc',  'data/mbs-mock.cfg',
             install_dir : imagedir,
             install_mode : 'rw-r--r--')
install_data('data/flatpak-host.sh',
             install_dir : imagedir,
             install_mode : 'rwxr-xr-x')

cdata = configuration_data()
cdata.set('DATADIR', join_paths(get_option('prefix'), get_option('datadir')))

configure_file(input : 'scripts/fedpkg-c.in',
               output : 'fedpkg-c',
               configuration : cdata,
               install_dir : get_option('bindir'),
               install_mode : 'rwxr-xr-x')
