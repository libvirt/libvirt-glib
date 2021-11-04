testenv = environment()
testenv.append('G_TEST_SRCDIR', meson.current_source_dir())
testenv.append('G_TEST_BUILDDIR', meson.current_build_dir())

test_gconfig = executable(
  'test-gconfig',
  c_args: common_cflags + warning_cflags,
  sources: ['test-gconfig.c'],
  include_directories: top_include_dir,
  dependencies: [libvirt_gconfig_dep]
)
test('gconfig', test_gconfig, env: testenv)

test_events = executable(
  'test-events',
  c_args: common_cflags + warning_cflags,
  sources: ['test-events.c'],
  include_directories: top_include_dir,
  dependencies: [libvirt_glib_dep]
)

test('events', test_events, env: testenv)
