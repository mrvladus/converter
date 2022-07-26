project('converter', 'c',
	version: '1.0',
	meson_version: '>= 0.60.0',
	default_options: [
	'warning_level=2',
	'c_std=gnu11',
	],
)

gnome = import('gnome')

i18n = import('i18n')
add_project_arguments('-DGETTEXT_PACKAGE="@0@"'.format(meson.project_name()), language: 'c')

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


subdir('po')
subdir('data')
subdir('src')

gnome.post_install(
	glib_compile_schemas: true,
	gtk_update_icon_cache: true,
	update_desktop_database: true,
)