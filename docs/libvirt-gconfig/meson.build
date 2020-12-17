libvirt_gconfig_doc_path = join_paths(datadir, 'gtk-doc', 'html', 'Libvirt-gconfig')

gnome.gtkdoc(
  'Libvirt-gconfig',
  main_sgml: 'Libvirt-gconfig-docs.xml',
  src_dir: [join_paths(meson.source_root(), 'libvirt-gconfig')],
  dependencies: [
    libvirt_gconfig_dep,
  ],
  scan_args: [
    '--rebuild-sections',
    '--rebuild-types',
  ],
  mkdb_args: [
    '--output-format=xml',
  ],
  install: true,
  install_dir: libvirt_gconfig_doc_path
)
