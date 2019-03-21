project('hack-services', 'c', version: '0.0.0')

gnome = import('gnome')
pkg = import('pkgconfig')

gio_dep = dependency('gio-2.0')
gio_unix = dependency ('gio-unix-2.0')

if get_option('enable-hss')
    subdir('hack-sound-server')
endif

if get_option('enable-gss')
    subdir('game-state-service')
endif
