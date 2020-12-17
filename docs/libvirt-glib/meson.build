version_xml = configure_file(
  input: 'version.xml.in',
  output: 'version.xml',
  configuration: conf_data
)

libvirt_glib_doc_path = join_paths(datadir, 'gtk-doc', 'html', 'Libvirt-glib')

gnome.gtkdoc(
  'Libvirt-glib',
  main_sgml: 'Libvirt-glib-docs.xml',
  src_dir: [join_paths(meson.source_root(), 'libvirt-glib')],
  dependencies: [
    libvirt_glib_dep, gobject_dep,
  ],
  scan_args: [
    '--rebuild-sections',
  ],
  mkdb_args: [
    '--output-format=xml',
  ],
  install: true,
  install_dir: libvirt_glib_doc_path
)
