converter_sources = [
	'main.c',
]

converter_deps = [
	dependency('libadwaita-1', version: '>= 1.1'),
	dependency('vte-2.91-gtk4', version: '>= 0.69.0'),
]

gnome = import('gnome')

converter_sources += gnome.compile_resources(
	'converter-resources',
	'converter.gresource.xml',
	c_name: 'converter'
)

executable(
	'converter',
	converter_sources,
	dependencies: converter_deps,
	install: true,
)