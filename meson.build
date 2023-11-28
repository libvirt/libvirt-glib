project(
  'libvirt-glib', 'c',
  version: '5.0.0',
  license: 'LGPLv2+',
  meson_version: '>= 0.56.0',
  default_options: [
    'buildtype=debugoptimized',
    'c_std=gnu99',
    'warning_level=2',
  ],
)

python3 = find_program('python3', required: true)
# Python3 < 3.7 treats the C locale as 7-bit only. We must force env vars so
# it treats it as UTF-8 regardless of the user's locale.
runutf8 = [ 'LC_ALL=', 'LANG=C', 'LC_CTYPE=en_US.UTF-8' ]

cc = meson.get_compiler('c')
git = run_command('test', '-d', '.git', check: false).returncode() == 0

version_info = meson.project_version().split('.')
version_major = version_info[0].to_int()
version_minor = version_info[1].to_int()
version_micro = version_info[2].to_int()

libvirt_glib_interface_age = 0
libvirt_glib_binary_age = 1000 * version_major + version_minor

# Maintain version scheme with libtool
libvirt_glib_soversion = 0
libvirt_glib_version = '@0@.@1@.@2@'.format(
    libvirt_glib_soversion,
    (libvirt_glib_binary_age - libvirt_glib_interface_age),
    libvirt_glib_interface_age)
osx_current = libvirt_glib_binary_age - libvirt_glib_interface_age + 1
libvirt_glib_darwin_versions = [
  osx_current,
  '@0@.@1@.0'.format(osx_current, libvirt_glib_interface_age)
]

# Keep these two definitions in agreement.
glib_version = '2.48.0'
glib_min_version_symbol='GLIB_VERSION_2_48'
glib_dep = dependency('glib-2.0', version: '>=' + glib_version)
gobject_dep = dependency('gobject-2.0', version: '>=' + glib_version)
gio_dep = dependency('gio-2.0', version: '>=' + glib_version)

libvirt_version = '2.3.0'
libvirt_dep = dependency('libvirt', version: '>=' + libvirt_version)

libxml_min_version='>=2.9.1'
libxml_dep = dependency('libxml-2.0', version: libxml_min_version)

prefix = get_option('prefix')
localedir = join_paths(prefix, get_option('localedir'))
datadir = join_paths(prefix, get_option('datadir'))
libdir = join_paths(prefix, get_option('libdir'))
includedir = join_paths(prefix, get_option('includedir'))

subdir('build-aux')

# gobject introspection
gir = find_program('g-ir-scanner', required: get_option('introspection'))
enable_introspection = gir.found() and not meson.is_cross_build()

# vala
vapi_opt = get_option('vapi')
enable_vapi = add_languages('vala', native: false, required: vapi_opt)
if enable_vapi and not enable_introspection
    enable_vapi = false
    if vapi_opt.enabled()
        error('vala support was requested, but introspection support is mandatory.')
    endif
endif

# gtk-doc
if not get_option('docs').disabled()
  gtk_doc = find_program('gtkdoc-scan', required: get_option('docs'))
  enable_doc = gtk_doc.found() and not meson.is_cross_build()
else
  enable_doc = false
endif

# Add RPATH information when building for a non-standard prefix, or
# when explicitly requested to do so

if prefix == '/usr' and not get_option('rpath').enabled()
  rpathdir = ''
else
  rpathdir = libdir
endif

common_link_args = []
if meson.get_compiler('c').has_link_argument('-Wl,--no-undefined')
  common_link_args += ['-Wl,--no-undefined']
endif


build_tests = not get_option('tests').disabled()

girdir = join_paths(datadir, 'gir-1.0')
typelibdir = join_paths(libdir, 'girepository-1.0')
pkgconfdir = join_paths(libdir, 'pkgconfig')

conf_data = configuration_data()
conf_data.set('VERSION', meson.project_version())
conf_data.set('prefix', prefix)
conf_data.set('includedir', includedir)
conf_data.set('exec_prefix', prefix)
conf_data.set('libdir', libdir)

configure_file(
  input: 'meson-config.h.in',
  output: 'config.h',
  configuration: conf_data
)

if git
  configure_file(
    input: 'libvirt-glib.spec.in',
    output: 'libvirt-glib.spec',
    configuration: conf_data
  )
endif

configure_file(
  input: 'libvirt-glib-1.0.pc.in',
  output: 'libvirt-glib-1.0.pc',
  configuration: conf_data,
  install: true,
  install_dir: pkgconfdir
)

configure_file(
  input: 'libvirt-gconfig-1.0.pc.in',
  output: 'libvirt-gconfig-1.0.pc',
  configuration: conf_data,
  install: true,
  install_dir: pkgconfdir
)

configure_file(
  input: 'libvirt-gobject-1.0.pc.in',
  output: 'libvirt-gobject-1.0.pc',
  configuration: conf_data,
  install: true,
  install_dir: pkgconfdir
)

cc_flags = []

git_werror = get_option('git_werror')
if git_werror.enabled() or git_werror.auto() and git
  cc_flags += [ '-Werror' ]
endif

# gcc --help=warnings outputs
ptrdiff_max = cc.sizeof('ptrdiff_t', prefix: '#include <stddef.h>')
size_max = cc.sizeof('size_t', prefix: '#include <stdint.h>')
# Compute max safe object size by checking ptrdiff_t and size_t sizes.
# Ideally we would get PTRDIFF_MAX and SIZE_MAX values but it would
# give us (2147483647L) and we would have to remove the () and the suffix
# in order to convert it to numbers to be able to pick the smaller one.
alloc_max = run_command(
  python3.full_path(), '-c',
  'print(min(2**(@0@ * 8 - 1) - 1, 2**(@1@ * 8) - 1))'.format(ptrdiff_max, size_max),
  check: true,
)

cc_flags += [
  '-fasynchronous-unwind-tables',
  '-fexceptions',
  '-fipa-pure-const',
  '-fno-common',
  '-Wabsolute-value',
  '-Waddress',
  '-Waddress-of-packed-member',
  '-Waggressive-loop-optimizations',
  '-Walloc-size-larger-than=@0@'.format(alloc_max.stdout().strip()),
  '-Warray-bounds=2',
  '-Wattribute-alias=2',
  '-Wattribute-warning',
  '-Wattributes',
  '-Wbool-compare',
  '-Wbool-operation',
  '-Wbuiltin-declaration-mismatch',
  '-Wbuiltin-macro-redefined',
  '-Wcannot-profile',
  # We do "bad" function casts all the time for event callbacks
  '-Wno-cast-function-type',
  '-Wchar-subscripts',
  '-Wclobbered',
  '-Wcomment',
  '-Wcomments',
  '-Wcoverage-mismatch',
  '-Wcpp',
  '-Wdangling-else',
  '-Wdate-time',
  '-Wdeprecated-declarations',
  '-Wdesignated-init',
  '-Wdiscarded-array-qualifiers',
  '-Wdiscarded-qualifiers',
  '-Wdiv-by-zero',
  '-Wduplicated-cond',
  '-Wduplicate-decl-specifier',
  '-Wempty-body',
  '-Wendif-labels',
  '-Wexpansion-to-defined',
  '-Wformat-contains-nul',
  '-Wformat-extra-args',
  '-Wno-format-nonliteral',
  '-Wformat-overflow=2',
  '-Wformat-security',
  # -Wformat enables this by default, and we should keep it,
  # but need to rewrite various areas of code first
  '-Wno-format-truncation',
  '-Wformat-y2k',
  '-Wformat-zero-length',
  '-Wframe-address',
  '-Wframe-larger-than=256',
  '-Wfree-nonheap-object',
  '-Whsa',
  '-Wif-not-aligned',
  '-Wignored-attributes',
  '-Wignored-qualifiers',
  '-Wimplicit',
  '-Wimplicit-fallthrough=5',
  '-Wimplicit-function-declaration',
  '-Wimplicit-int',
  '-Wincompatible-pointer-types',
  '-Winit-self',
  '-Winline',
  '-Wint-conversion',
  '-Wint-in-bool-context',
  '-Wint-to-pointer-cast',
  '-Winvalid-memory-model',
  '-Winvalid-pch',
  '-Wjump-misses-init',
  '-Wlogical-not-parentheses',
  '-Wlogical-op',
  '-Wmain',
  '-Wmaybe-uninitialized',
  '-Wmemset-elt-size',
  '-Wmemset-transposed-args',
  '-Wmisleading-indentation',
  '-Wmissing-attributes',
  '-Wmissing-braces',
  '-Wmissing-declarations',
  '-Wmissing-field-initializers',
  '-Wmissing-include-dirs',
  '-Wmissing-parameter-type',
  '-Wmissing-profile',
  '-Wmissing-prototypes',
  '-Wmultichar',
  '-Wmultistatement-macros',
  '-Wnarrowing',
  '-Wnested-externs',
  '-Wnonnull',
  '-Wnonnull-compare',
  '-Wnormalized=nfc',
  '-Wnull-dereference',
  '-Wodr',
  '-Wold-style-declaration',
  '-Wold-style-definition',
  '-Wopenmp-simd',
  '-Woverflow',
  '-Woverride-init',
  '-Wpacked-bitfield-compat',
  '-Wpacked-not-aligned',
  '-Wparentheses',
  '-Wpointer-arith',
  '-Wpointer-compare',
  '-Wpointer-sign',
  '-Wpointer-to-int-cast',
  '-Wpragmas',
  '-Wpsabi',
  '-Wrestrict',
  '-Wreturn-local-addr',
  '-Wreturn-type',
  '-Wscalar-storage-order',
  '-Wsequence-point',
  '-Wshadow',
  '-Wshift-count-negative',
  '-Wshift-count-overflow',
  '-Wshift-negative-value',
  '-Wshift-overflow=2',
  # So we have -W enabled, and then have to explicitly turn off...
  '-Wno-sign-compare',
  '-Wsizeof-array-argument',
  '-Wsizeof-pointer-div',
  '-Wsizeof-pointer-memaccess',
  '-Wstrict-aliasing',
  '-Wstrict-prototypes',
  '-Wstringop-overflow=2',
  '-Wstringop-truncation',
  '-Wsuggest-attribute=cold',
  '-Wno-suggest-attribute=const',
  '-Wsuggest-attribute=format',
  '-Wsuggest-attribute=noreturn',
  '-Wno-suggest-attribute=pure',
  '-Wsuggest-final-methods',
  '-Wsuggest-final-types',
  '-Wswitch',
  '-Wswitch-bool',
  '-Wswitch-enum',
  '-Wswitch-unreachable',
  '-Wsync-nand',
  '-Wtautological-compare',
  '-Wtrampolines',
  '-Wtrigraphs',
  '-Wtype-limits',
  # Clang incorrectly complains about dup typedefs win gnu99 mode
  # so use this Clang-specific arg to keep it quiet
  '-Wno-typedef-redefinition',
  '-Wuninitialized',
  '-Wunknown-pragmas',
  '-Wunused',
  '-Wunused-but-set-parameter',
  '-Wunused-but-set-variable',
  '-Wunused-const-variable=2',
  '-Wunused-function',
  '-Wunused-label',
  '-Wunused-local-typedefs',
  '-Wunused-result',
  '-Wunused-value',
  '-Wunused-variable',
  '-Wvarargs',
  '-Wvariadic-macros',
  '-Wvector-operation-performance',
  '-Wvla',
  '-Wvolatile-register-var',
  '-Wwrite-strings',
]


if get_option('warning_level') == '2'
  # on aarch64 error: -fstack-protector not supported for this target
  if host_machine.cpu_family() != 'aarch64'
    if host_machine.system() in [ 'linux', 'freebsd', 'windows' ]
      # we prefer -fstack-protector-strong but fallback to -fstack-protector-all
      fstack_cflags = cc.first_supported_argument([
        '-fstack-protector-strong',
        '-fstack-protector-all',
      ])
      cc_flags += fstack_cflags

      # When building with mingw using -fstack-protector requires libssp library
      # which is included by using -fstack-protector with linker.
      if fstack_cflags.length() == 1 and host_machine.system() == 'windows'
        add_project_link_arguments(fstack_cflags, language: 'c')
      endif
    endif
  endif

  # Clang complains about unused static inline functions which are common
  # with G_DEFINE_AUTOPTR_CLEANUP_FUNC.
  w_unused_function_args = ['-Wunused-function', '-Werror']
  w_unused_function_code = '''
    static inline void foo(void) {}

    int main(void) { return 0; }
  '''
  # -Wunused-function is implied by -Wall, we must turn it off explicitly.
  if not cc.compiles(w_unused_function_code, args: w_unused_function_args)
    cc_flags += ['-Wno-unused-function']
  endif

  warning_cflags = cc.get_supported_arguments(cc_flags)
else
  warning_cflags = []
endif

if host_machine.system() == 'windows'
  # no machinery for generating .def files for now
  version_script_flags = ''
elif host_machine.system() == 'darwin'
  # macOS libraries don't support symbol versioning
  version_script_flags = ''
else
  version_script_flags = '-Wl,--version-script='
endif



if git
  authors = run_command(python3.full_path(), gen_authors_prog.full_path(),
                        env: runutf8, check: true)
  authors_file = 'AUTHORS.in'

  authors_conf = configuration_data()
  authors_conf.set('authorslist', authors.stdout())

  configure_file(
    input: authors_file,
    output: '@BASENAME@',
    configuration: authors_conf,
  )

  meson.add_dist_script(python3.full_path(), dist_prog.full_path(),
                        meson.project_build_root(), 'AUTHORS')
  meson.add_dist_script(python3.full_path(), dist_prog.full_path(),
                        meson.project_build_root(), 'libvirt-glib.spec')
endif

common_cflags = [
  '-DLOCALEDIR="@0@"'.format(localedir),
  '-DDATADIR="@0@"'.format(datadir),
  '-DLIBVIRT_GLIB_BUILD',
  '-DGLIB_VERSION_MIN_REQUIRED=@0@'.format(glib_min_version_symbol),
  '-DGLIB_VERSION_MAX_ALLOWED=@0@'.format(glib_min_version_symbol),
]

gnome = import('gnome')
i18n = import('i18n')

top_include_dir = [include_directories('.')]

subdir('libvirt-glib')
subdir('libvirt-gconfig')
subdir('libvirt-gobject')
subdir('po')
subdir('examples')
subdir('docs')
if build_tests
  subdir('tests')
endif
