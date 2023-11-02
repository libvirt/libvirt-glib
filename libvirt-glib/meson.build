libvirt_glib_sources = [
  'libvirt-glib-error.c',
  'libvirt-glib-event.c',
  'libvirt-glib-main.c',
]

libvirt_glib_headers = [
  'libvirt-glib.h',
  'libvirt-glib-error.h',
  'libvirt-glib-event.h',
  'libvirt-glib-main.h',
]

libvirt_glib_deps = [
  libvirt_dep, glib_dep,
]

libvirt_glib_sym_file = meson.current_source_dir() / 'libvirt-glib.sym'
libvirt_glib_link_args = common_link_args
libvirt_glib_link_depends = []
if version_script_flags != ''
  libvirt_glib_link_args += '@0@@1@'.format(
    version_script_flags,
    libvirt_glib_sym_file,
  )
  libvirt_glib_link_depends += libvirt_glib_sym_file
endif

libvirt_glib = shared_library(
  'virt-glib-1.0',
  sources: libvirt_glib_sources,
  c_args: common_cflags + warning_cflags,
  dependencies: libvirt_glib_deps,
  include_directories: top_include_dir,
  link_args: libvirt_glib_link_args,
  link_depends: libvirt_glib_link_depends,
  version: libvirt_glib_version,
  darwin_versions: libvirt_glib_darwin_versions,
  soversion: libvirt_glib_soversion,
  install: true,
  install_rpath: rpathdir,
)

libvirt_glib_dep = declare_dependency(
  link_with: libvirt_glib,
  dependencies: libvirt_glib_deps,
)

install_data(
  libvirt_glib_headers,
  install_dir: join_paths(includedir, 'libvirt-glib-1.0', 'libvirt-glib'),
)

if enable_introspection
  libvirt_glib_gir_sources = [
    libvirt_glib_headers,
    libvirt_glib_sources,
  ]

  libvirt_glib_gir = gnome.generate_gir(
    libvirt_glib,
    sources: libvirt_glib_gir_sources,
    nsversion: '1.0',
    namespace: 'LibvirtGLib',
    export_packages: 'libvirt-glib-1.0',
    includes: [
      'GLib-2.0',
    ],
    dependencies: libvirt_glib_deps,
    header: 'libvirt-glib/libvirt-glib.h',
    identifier_prefix: 'GVir',
    symbol_prefix: 'gvir',
    install: true,
    install_dir_gir: girdir,
    install_dir_typelib: typelibdir,
  )

  libvirt_glib_gir_dep = declare_dependency(
    sources: libvirt_glib_gir,
  )

  if enable_vapi
    libvirt_glib_vapi = gnome.generate_vapi(
      'libvirt-glib-1.0',
      install: true,
      packages: [
        'glib-2.0',
      ],
      sources: libvirt_glib_gir[0],
    )
  endif
endif

if host_machine.system() != 'windows' and host_machine.system() != 'darwin'
  test('check-glib-symfile',
    python3,
    args: [check_symfile_prog.full_path(),
      libvirt_glib_sym_file,
      join_paths(meson.current_build_dir(), 'libvirt-glib-1.0.so')])
endif

test('check-glib-symsorting',
     python3,
     args: [check_symsorting_prog.full_path(),
            libvirt_glib_sym_file])
