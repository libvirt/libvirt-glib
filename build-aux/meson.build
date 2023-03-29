scripts = [
    'check-symfile.py',
    'check-symsorting.py',
    'dist.py',
    'gen-authors.py',
]

foreach name : scripts
  sname = name.split('.')[0].underscorify()
  set_variable('@0@_prog'.format(sname), find_program(name))
endforeach

syntax_check_conf = configuration_data()
syntax_check_conf.set('srcdir', meson.project_source_root())

configure_file(
  input: 'Makefile.in',
  output: '@BASENAME@',
  configuration: syntax_check_conf,
)


if host_machine.system() == 'freebsd'
  make_prog = find_program('gmake')
else
  make_prog = find_program('make')
endif

rc = run_command(
  'sed', '-n',
  's/^\\(sc_[a-zA-Z0-9_-]*\\):.*/\\1/p',
  meson.current_source_dir() / 'syntax-check.mk',
  check: true,
)

sc_tests = rc.stdout().strip().split()

# Skip syntax-check if not building from git because we get the list of files
# to check using git commands and it fails if we are not in git repository.
if git
  foreach target : sc_tests
    test(
      target,
      make_prog,
      args: [
        '-C', meson.current_build_dir(),
        target
      ],
      suite: 'syntax-check',
    )
  endforeach
endif
