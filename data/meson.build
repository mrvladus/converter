desktop_file = i18n.merge_file(
	input: 'io.github.mrvladus.Converter.desktop.in',
	output: 'io.github.mrvladus.Converter.desktop',
	type: 'desktop',
	po_dir: '../po',
	install: true,
	install_dir: join_paths(get_option('datadir'), 'applications')
)

appstream_file = i18n.merge_file(
	input: 'io.github.mrvladus.Converter.appdata.xml.in',
	output: 'io.github.mrvladus.Converter.appdata.xml',
	po_dir: '../po',
	install: true,
	install_dir: join_paths(get_option('datadir'), 'appdata')
)

subdir('icons')