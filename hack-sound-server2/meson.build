namespace = 'HackServices'
library_name = 'hacksoundserver2'
export_package_name = 'hack-sound-server2'

api_version = '0'
api_name = '@0@-@0@'.format(library_name, api_version)
libtool_version = '0.0.0'

hacksoundserver2_dbus_sources = gnome.gdbus_codegen(
    export_package_name,
    sources: 'data/com.endlessm.HackSoundServer2.xml',
    interface_prefix : 'com.endlessm.',
    namespace: namespace,
    autocleanup: 'all',
    install_header: true,
    install_dir: join_paths(get_option('includedir'), meson.project_name()),
    annotations: [
        ['com.endlessm.HackSoundServer2', 'org.gtk.GDBus.C.Name', 'Sound2'],
        ['com.endlessm.HackSoundServer2.Player', 'org.gtk.GDBus.C.Name', 'Sound2Player'],
        ['com.endlessm.HackSoundServer2.Player.Sound', 'org.gtk.GDBus.C.Name', 'Sound2PlayerSound']
    ]
)

hacksoundserver2lib = shared_library(
    api_name,
    hacksoundserver2_dbus_sources,
    version: meson.project_version(),
    dependencies: [gio_dep, gio_unix],
    install: true,
    soversion: api_version,
    version: libtool_version
)

pkg.generate(
    libraries: hacksoundserver2lib,
    version: meson.project_version(),
    name: namespace,
    filebase: namespace,
    description: 'Clients for Hack DBus-based services',
    requires: ['gio-2.0', 'gio-unix-2.0']
)

hack_services_gen_sources = gnome.generate_gir(
    hacksoundserver2lib,
    sources : hacksoundserver2_dbus_sources,
    namespace : namespace,
    nsversion : api_version,
    includes: ['Gio-2.0'],
    install : true,
    dependencies : [
        gio_dep,
        gio_unix
    ]
)
