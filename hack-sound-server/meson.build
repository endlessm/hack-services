namespace = 'HackSoundServer'
library_name = 'hacksoundserver'
export_package_name = 'hack-sound-server'

api_version = '0'
api_name = '@0@-@0@'.format(library_name, api_version)
libtool_version = '0.0.0'

gamestateservice_dbus_sources = gnome.gdbus_codegen(
    export_package_name,
    sources: 'data/com.endlessm.HackSoundServer.xml',
    interface_prefix : 'com.endlessm.',
    autocleanup: 'all',
    install_header: true,
    install_dir: join_paths(get_option('includedir'), meson.project_name())
)

gamestateservicelib = shared_library(
    api_name,
    gamestateservice_dbus_sources,
    version: meson.project_version(),
    dependencies: [gio_dep, gio_unix],
    install: true,
    soversion: api_version,
    version: libtool_version
)

pkg.generate(
    libraries: gamestateservicelib,
    version: meson.project_version(),
    name: namespace,
    filebase: namespace,
    description: 'Clients for Hack DBus-based services',
    requires: ['gio-2.0', 'gio-unix-2.0']
)

hack_services_gen_sources = gnome.generate_gir(
    gamestateservicelib,
    sources : gamestateservice_dbus_sources,
    namespace : namespace,
    nsversion : api_version,
    includes: ['Gio-2.0'],
    install : true,
    dependencies : [
        gio_dep,
        gio_unix
    ]
)
