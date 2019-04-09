hacksoundserver2_namespace = 'HackSound'
hacksoundserver2_version = '2'
hacksoundserver2_library_name = 'hacksoundserver2'
hacksoundserver2_export_package_name = 'hack-sound-server2'
hacksoundserver2_api_name = '@0@-@1@'.format(hacksoundserver2_library_name,
                                             hacksoundserver2_version)

hacksoundserver2_dbus_sources = gnome.gdbus_codegen(
    hacksoundserver2_export_package_name,
    sources: 'data/com.endlessm.HackSoundServer2.xml',
    interface_prefix : 'com.endlessm.',
    autocleanup: 'all',
    install_header: true,
    install_dir: join_paths(get_option('includedir'), meson.project_name())
)

hacksoundserver2lib = shared_library(
    hacksoundserver2_api_name,
    hacksoundserver2_dbus_sources,
    version: meson.project_version(),
    dependencies: [gio_dep, gio_unix],
    install: true,
    soversion: hacksoundserver2_version
)

pkg.generate(
    hacksoundserver2lib,
    name: meson.project_name(),
    filebase: hacksoundserver2_api_name,
    description: 'A HackSoundServer2 Client',
    requires: ['gio-2.0', 'gio-unix-2.0']
)

gnome.generate_gir(
    hacksoundserver2lib,
    sources : hacksoundserver2_dbus_sources,
    namespace : hacksoundserver2_namespace,
    nsversion : hacksoundserver2_version,
    includes: ['Gio-2.0'],
    install : true,
    dependencies : [
        gio_dep,
        gio_unix
    ]
)
