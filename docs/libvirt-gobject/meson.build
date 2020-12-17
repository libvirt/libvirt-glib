libvirt_gobject_doc_path = join_paths(datadir, 'gtk-doc', 'html', 'Libvirt-gobject')

gnome.gtkdoc(
  'Libvirt-gobject',
  main_sgml: 'Libvirt-gobject-docs.xml',
  src_dir: [join_paths(meson.source_root(), 'libvirt-gobject')],
  dependencies: [
    libvirt_gobject_dep,
  ],
  scan_args: [
    '--rebuild-sections',
    '--rebuild-types',
  ],
  mkdb_args: [
    '--output-format=xml',
  ],
  install: true,
  install_dir: libvirt_gobject_doc_path
)
