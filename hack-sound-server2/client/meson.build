hacksoundclient2sources = [
  'hack-sound-client.c'
]

hacksoundclient2headers = [
  'hack-sound-client.h'
]

hacksoundclient2lib = library(
	'hacksoundclient-@0@'.format(hacksoundserver2_version),
	hacksoundclient2sources + hacksoundclient2headers,
	version : hacksoundserver2_version,
	soversion : hacksoundserver2_version,
	install : true,
	dependencies : hacksoundserver2_dep,
	include_directories: include_directories('.')
)

install_headers(hacksoundclient2headers)

hacksoundclient2_dep = declare_dependency(
	link_with : hacksoundclient2lib,
	sources: hacksoundclient2sources + hacksoundclient2headers,
	dependencies : [
	    gio_dep,
	    gio_unix
	],
	include_directories: include_directories('.')
)

subdir('examples')
