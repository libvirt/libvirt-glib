executable(
  'event-test',
  sources: ['event-test.c'],
  dependencies: libvirt_glib_dep,
  include_directories: top_include_dir,
)

executable(
  'conn-test',
  sources: ['conn-test.c'],
  dependencies: [libvirt_glib_dep, libvirt_gconfig_dep, libvirt_gobject_dep],
  include_directories: top_include_dir,
)
