hacksoundserver_namespace = 'HackSound'
hacksoundserver_version = '0'
hacksoundserver_library_name = 'hacksoundserver'
hacksoundserver_export_package_name = 'hack-sound-server'
hacksoundserver_api_name = '@0@-@1@'.format(hacksoundserver_library_name,
                                            hacksoundserver_version)

hacksoundserver_dbus_sources = gnome.gdbus_codegen(
    hacksoundserver_export_package_name,
    sources: 'data/com.endlessm.HackSoundServer.xml',
    interface_prefix : 'com.endlessm.',
    autocleanup: 'all',
    install_header: true,
    install_dir: join_paths(get_option('includedir'), meson.project_name())
)

hacksoundserverlib = shared_library(
    hacksoundserver_api_name,
    hacksoundserver_dbus_sources,
    version: meson.project_version(),
    dependencies: [gio_dep, gio_unix],
    install: true,
    soversion: hacksoundserver_version
)

pkg.generate(
    hacksoundserverlib,
    name: meson.project_name(),
    filebase: hacksoundserver_api_name,
    description: 'A HackSoundServer Client',
    requires: ['gio-2.0', 'gio-unix-2.0']
)

gnome.generate_gir(
    hacksoundserverlib,
    sources : hacksoundserver_dbus_sources,
    namespace : hacksoundserver_namespace,
    nsversion : hacksoundserver_version,
    includes: ['Gio-2.0'],
    install : true,
    dependencies : [
        gio_dep,
        gio_unix
    ]
)
