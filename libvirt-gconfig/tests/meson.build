test_domain_create = executable(
  'test-domain-create',
  c_args: common_cflags + warning_cflags,
  sources: ['test-domain-create.c'],
  include_directories: top_include_dir,
  dependencies: [libvirt_gconfig_dep]
)

test_domain_parse = executable(
  'test-domain-parse',
  c_args: common_cflags + warning_cflags,
  sources: ['test-domain-parse.c'],
  include_directories: top_include_dir,
  dependencies: [libvirt_gconfig_dep]
)

test_capabilities_parse = executable(
  'test-capabilities-parse',
  c_args: common_cflags + warning_cflags,
  sources: ['test-capabilities-parse.c'],
  include_directories: top_include_dir,
  dependencies: [libvirt_gconfig_dep]
)
