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

libvirt_glib_link_args = common_link_args
if host_machine.system() == 'linux'
  libvirt_glib_link_args += [
    '-Wl,--version-script,@0@/@1@'.format(
      meson.current_source_dir(),
      'libvirt-glib.sym'
    )
  ]
endif

libvirt_glib = shared_library(
  'virt-glib-1.0',
  sources: libvirt_glib_sources,
  c_args: common_cflags,
  dependencies: libvirt_glib_deps,
  include_directories: top_include_dir,
  link_args: libvirt_glib_link_args,
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

test('check-glib-symfile',
     python3,
     args: [check_symfile,
            join_paths(meson.current_source_dir(), 'libvirt-glib.sym'),
            join_paths(meson.current_build_dir(), 'libvirt-glib-1.0.so')])

test('check-glib-symsorting',
     python3,
     args: [check_symsorting,
            join_paths(meson.current_source_dir(), 'libvirt-glib.sym')])
