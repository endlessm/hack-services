gamestateservice_namespace = 'GameStateService'
gamestateservice_library_name = 'gamestateservice'
gamestateservice_export_package_name = 'game-state-service'
gamestateservice_api_version = '0'
gamestateservice_api_name = '@0@-@1@'.format(gamestateservice_library_name,
                                             gamestateservice_api_version)


gamestateservice_dbus_sources = gnome.gdbus_codegen(
    gamestateservice_export_package_name,
    sources: 'data/com.endlessm.GameStateService.xml',
    interface_prefix : 'com.endlessm.',
    autocleanup: 'all',
    install_header: true,
    install_dir: join_paths(get_option('includedir'), meson.project_name())
)

gamestateservicelib = shared_library(
    gamestateservice_api_name,
    gamestateservice_dbus_sources,
    version: meson.project_version(),
    dependencies: [gio_dep, gio_unix],
    install: true,
    soversion: gamestateservice_api_version
)

pkg.generate(
    libraries: gamestateservicelib,
    name: meson.project_name(),
    filebase: gamestateservice_api_name,
    description: 'A GameStateService Client',
    requires: ['gio-2.0', 'gio-unix-2.0']
)

gnome.generate_gir(
    gamestateservicelib,
    sources : gamestateservice_dbus_sources,
    namespace : gamestateservice_namespace,
    nsversion : gamestateservice_api_version,
    includes: ['Gio-2.0'],
    install : true,
    dependencies : [
        gio_dep,
        gio_unix
    ]
)
