converter_sources = [
	'main.c',
]

converter_deps = [
	dependency('libadwaita-1', version: '>= 1.1'),
	dependency('vte-2.91-gtk4', version: '>= 0.69.0'),
]

config_h = configuration_data()
config_h.set_quoted('PACKAGE_VERSION', meson.project_version())
config_h.set_quoted('GETTEXT_PACKAGE', meson.project_name())
config_h.set_quoted('LOCALEDIR', join_paths(get_option('prefix'), get_option('localedir')))

configure_file(
  output: 'config.h',
  configuration: config_h,
)

add_project_arguments([
  '-I' + meson.project_build_root(),
], language: 'c')

converter_sources += gnome.compile_resources(
	'converter-resources',
	'converter.gresource.xml',
	c_name: 'converter'
)

executable(
	'converter',
	converter_sources,
	dependencies: converter_deps,
	install: true
)