libvirt_gobject_public_headers = [
  'libvirt-gobject.h',
  'libvirt-gobject-main.h',
  'libvirt-gobject-domain-snapshot.h',
  'libvirt-gobject-domain-device.h',
  'libvirt-gobject-domain-disk.h',
  'libvirt-gobject-domain-interface.h',
  'libvirt-gobject-domain.h',
  'libvirt-gobject-interface.h',
  'libvirt-gobject-network.h',
  'libvirt-gobject-network-dhcp-lease.h',
  'libvirt-gobject-network-filter.h',
  'libvirt-gobject-node-device.h',
  'libvirt-gobject-secret.h',
  'libvirt-gobject-storage-vol.h',
  'libvirt-gobject-storage-pool.h',
  'libvirt-gobject-stream.h',
  'libvirt-gobject-connection.h',
  'libvirt-gobject-manager.h',
]

libvirt_gobject_public_sources = [
  'libvirt-gobject-main.c',
  'libvirt-gobject-domain-snapshot.c',
  'libvirt-gobject-domain-device.c',
  'libvirt-gobject-domain-disk.c',
  'libvirt-gobject-domain-interface.c',
  'libvirt-gobject-domain.c',
  'libvirt-gobject-interface.c',
  'libvirt-gobject-network.c',
  'libvirt-gobject-network-dhcp-lease.c',
  'libvirt-gobject-network-filter.c',
  'libvirt-gobject-node-device.c',
  'libvirt-gobject-secret.c',
  'libvirt-gobject-storage-vol.c',
  'libvirt-gobject-storage-pool.c',
  'libvirt-gobject-stream.c',
  'libvirt-gobject-connection.c',
  'libvirt-gobject-manager.c',
]

libvirt_gobject_sources = libvirt_gobject_public_sources + [
  'libvirt-gobject-compat.c',
  'libvirt-gobject-input-stream.c',
  'libvirt-gobject-output-stream.c',
]

libvirt_gobject_includedir = join_paths(includedir, 'libvirt-gobject-1.0', 'libvirt-gobject')

libvirt_gobject_enum_headers = gnome.mkenums(
  'libvirt-gobject-enums.h',
  sources: libvirt_gobject_public_headers,
  h_template: 'libvirt-gobject-enums.h.template',
  symbol_prefix: 'gvir',
  identifier_prefix: 'GVir',
  install_header: true,
  install_dir: libvirt_gobject_includedir,
)

libvirt_gobject_enum_sources = gnome.mkenums(
  'libvirt-gobject-enums.c',
  sources: libvirt_gobject_public_headers,
  c_template: 'libvirt-gobject-enums.c.template',
  symbol_prefix: 'gvir',
  identifier_prefix: 'GVir',
  # workaround for https://github.com/mesonbuild/meson/issues/9472
  install_header: false,
  install_dir: libvirt_gobject_includedir,
  # end workaround
)

libvirt_gobject_sources += [libvirt_gobject_enum_sources, libvirt_gobject_enum_headers]

libvirt_gobject_deps = [
  libvirt_dep, gio_dep, libvirt_glib_dep, libvirt_gconfig_dep,
]

libvirt_gobject_sym_file = meson.current_source_dir() / 'libvirt-gobject.sym'
libvirt_gobject_link_args = common_link_args
libvirt_gobject_link_depends = []
if version_script_flags != ''
  libvirt_gobject_link_args += '@0@@1@'.format(
    version_script_flags,
    libvirt_gobject_sym_file,
  )
  libvirt_gobject_link_depends += libvirt_gobject_sym_file
endif

libvirt_gobject = shared_library(
  'virt-gobject-1.0',
  sources: libvirt_gobject_sources,
  c_args: common_cflags + warning_cflags,
  dependencies: libvirt_gobject_deps,
  include_directories: top_include_dir,
  link_with: [libvirt_glib, libvirt_gconfig],
  link_args: libvirt_gobject_link_args,
  link_depends: libvirt_gobject_link_depends,
  version: libvirt_glib_version,
  darwin_versions: libvirt_glib_darwin_versions,
  soversion: libvirt_glib_soversion,
  install: true,
  install_rpath: rpathdir,
)

libvirt_gobject_dep = declare_dependency(
  sources: [
    libvirt_gobject_enum_headers,
  ],
  link_with: libvirt_gobject,
  dependencies: libvirt_gobject_deps,
)

install_data(
  libvirt_gobject_public_headers,
  install_dir: libvirt_gobject_includedir,
)

if enable_introspection
  libvirt_gobject_gir_sources = [
    libvirt_gobject_public_headers,
    libvirt_gobject_public_sources,
    libvirt_gobject_enum_sources,
    libvirt_gobject_enum_headers,
  ]

  libvirt_gobject_gir = gnome.generate_gir(
    libvirt_gobject,
    sources: libvirt_gobject_gir_sources,
    nsversion: '1.0',
    namespace: 'LibvirtGObject',
    export_packages: 'libvirt-gobject-1.0',
    includes: [
      'GObject-2.0',
      'Gio-2.0',
      libvirt_glib_gir[0],
      libvirt_gconfig_gir[0],
    ],
    dependencies: libvirt_gobject_deps + [],
    header: 'libvirt-gobject/libvirt-gobject.h',
    identifier_prefix: 'GVir',
    symbol_prefix: 'gvir',
    install: true,
    install_dir_gir: girdir,
    install_dir_typelib: typelibdir,
  )

  libvirt_gobject_gir_dep = declare_dependency(
    sources: libvirt_gobject_gir,
  )

  if enable_vapi
    libvirt_gobject_vapi = gnome.generate_vapi(
      'libvirt-gobject-1.0',
      install: true,
      packages: [
        'gobject-2.0',
        'gio-2.0',
        libvirt_glib_vapi,
        libvirt_gconfig_vapi,
      ],
      sources: libvirt_gobject_gir[0],
    )
  endif
endif

if host_machine.system() != 'windows' and host_machine.system() != 'darwin'
  test('check-gobject-symfile',
    python3,
    args: [check_symfile_prog.full_path(),
      libvirt_gobject_sym_file,
      join_paths(meson.current_build_dir(), 'libvirt-gobject-1.0.so')])
endif

test('check-gobject-symsorting',
     python3,
     args: [check_symsorting_prog.full_path(),
            libvirt_gobject_sym_file])
