# Customize Makefile.maint.                           -*- makefile -*-
# Copyright (C) 2008-2011 Red Hat, Inc.
# Copyright (C) 2001-2011 Free Software Foundation, Inc.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# This is reported not to work with make-3.79.1
# ME := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))

_build-aux := build-aux
ME := $(_build-aux)/syntax-check.mk

GIT = git
VC = $(GIT)

VC_LIST = $(srcdir)/$(_build-aux)/vc-list-files -C $(srcdir)

# You can override this variable in syntax-check.mk to set your own regexp
# matching files to ignore.
VC_LIST_ALWAYS_EXCLUDE_REGEX ?= ^$$

# This is to preprocess robustly the output of $(VC_LIST), so that even
# when $(srcdir) is a pathological name like "....", the leading sed command
# removes only the intended prefix.
_dot_escaped_srcdir = $(subst .,\.,$(srcdir))

# Post-process $(VC_LIST) output, prepending $(srcdir)/, but only
# when $(srcdir) is not ".".
ifeq ($(srcdir),.)
_prepend_srcdir_prefix =
else
_prepend_srcdir_prefix = | sed 's|^|$(srcdir)/|'
endif

# In order to be able to consistently filter "."-relative names,
# (i.e., with no $(srcdir) prefix), this definition is careful to
# remove any $(srcdir) prefix, and to restore what it removes.
_sc_excl = \
  $(if $(exclude_file_name_regexp--$@),$(exclude_file_name_regexp--$@),^$$)
VC_LIST_EXCEPT = \
  $(VC_LIST) | sed 's|^$(_dot_escaped_srcdir)/||' \
	| if test -f $(srcdir)/.x-$@; then grep -vEf $(srcdir)/.x-$@; \
	  else grep -Ev -e "$${VC_LIST_EXCEPT_DEFAULT-ChangeLog}"; fi \
	| grep -Ev -e '($(VC_LIST_ALWAYS_EXCLUDE_REGEX)|$(_sc_excl))' \
	$(_prepend_srcdir_prefix)

ifeq ($(origin prev_version_file), undefined)
  prev_version_file = $(srcdir)/.prev-version
endif

PREV_VERSION := $(shell cat $(prev_version_file) 2>/dev/null)
VERSION_REGEXP = $(subst .,\.,$(VERSION))
PREV_VERSION_REGEXP = $(subst .,\.,$(PREV_VERSION))

ifeq ($(VC),$(GIT))
this-vc-tag = v$(VERSION)
this-vc-tag-regexp = v$(VERSION_REGEXP)
else
tag-package = $(shell echo "$(PACKAGE)" | tr '[:lower:]' '[:upper:]')
tag-this-version = $(subst .,_,$(VERSION))
this-vc-tag = $(tag-package)-$(tag-this-version)
this-vc-tag-regexp = $(this-vc-tag)
endif
my_distdir = $(PACKAGE)-$(VERSION)

# Prevent programs like 'sort' from considering distinct strings to be equal.
# Doing it here saves us from having to set LC_ALL elsewhere in this file.
export LC_ALL = C

## --------------- ##
## Sanity checks.  ##
## --------------- ##

# Collect the names of rules starting with `sc_'.
syntax-check-rules := $(sort $(shell sed -n 's/^\(sc_[a-zA-Z0-9_-]*\):.*/\1/p' \
			$(srcdir)/$(ME)))
.PHONY: $(syntax-check-rules)

ifeq ($(shell $(VC_LIST) >/dev/null 2>&1; echo $$?),0)
local-checks-available += $(syntax-check-rules)
else
local-checks-available += no-vc-detected
no-vc-detected:
	@echo "No version control files detected; skipping syntax check"
endif
.PHONY: $(local-checks-available)

# Arrange to print the name of each syntax-checking rule just before running it.
$(syntax-check-rules): %: %.m
sc_m_rules_ = $(patsubst %, %.m, $(syntax-check-rules))
.PHONY: $(sc_m_rules_)
$(sc_m_rules_):
	@echo $(patsubst sc_%.m, %, $@)
	@date +%s.%N > .sc-start-$(basename $@)

# Compute and print the elapsed time for each syntax-check rule.
sc_z_rules_ = $(patsubst %, %.z, $(syntax-check-rules))
.PHONY: $(sc_z_rules_)
$(sc_z_rules_): %.z: %
	@end=$$(date +%s.%N);						\
	start=$$(cat .sc-start-$*);					\
	rm -f .sc-start-$*;						\
	awk -v s=$$start -v e=$$end					\
	  'END {printf "%.2f $(patsubst sc_%,%,$*)\n", e - s}' < /dev/null

# The patsubst here is to replace each sc_% rule with its sc_%.z wrapper
# that computes and prints elapsed time.
local-check :=								\
  $(patsubst sc_%, sc_%.z,$(local-checks-available))

syntax-check: $(local-check)

# _sc_search_regexp
#
# This macro searches for a given construct in the selected files and
# then takes some action.
#
# Parameters (shell variables):
#
#  prohibit | require
#
#     Regular expression (ERE) denoting either a forbidden construct
#     or a required construct.  Those arguments are exclusive.
#
#  in_vc_files | in_files
#
#     grep-E-style regexp denoting the files to check.  If no files
#     are specified the default are all the files that are under
#     version control.
#
#  containing | non_containing
#
#     Select the files (non) containing strings matching this regexp.
#     If both arguments are specified then CONTAINING takes
#     precedence.
#
#  with_grep_options
#
#     Extra options for grep.
#
#  ignore_case
#
#     Ignore case.
#
#  halt
#
#     Message to display before to halting execution.
#
# Finally, you may exempt files based on an ERE matching file names.
# For example, to exempt from the sc_space_tab check all files with the
# .diff suffix, set this Make variable:
#
# exclude_file_name_regexp--sc_space_tab = \.diff$
#
# Note that while this functionality is mostly inherited via VC_LIST_EXCEPT,
# when filtering by name via in_files, we explicitly filter out matching
# names here as well.

# By default, _sc_search_regexp does not ignore case.
export ignore_case =
_ignore_case = $$(test -n "$$ignore_case" && printf %s -i || :)

define _sc_say_and_exit
   dummy=; : so we do not need a semicolon before each use;		\
   { printf '%s\n' "$(ME): $$msg" 1>&2; exit 1; };
endef

# _sc_search_regexp used to be named _prohibit_regexp.  However,
# upgrading to the new definition and leaving the old name undefined
# would usually convert each custom rule using $(_prohibit_regexp)
# (usually defined in syntax-check.mk) into a no-op.  This definition ensures
# that people know right away if they're still using the old name.
# FIXME: remove in 2012.
_prohibit_regexp = \
  $(error '*** you need to s/_prohibit_regexp/_sc_search_regexp/, and adapt')

define _sc_search_regexp
   dummy=; : so we do not need a semicolon before each use;		\
									\
   : Check arguments;							\
   test -n "$$prohibit" && test -n "$$require"				\
     && { msg='Cannot specify both prohibit and require'		\
          $(_sc_say_and_exit) } || :;					\
   test -z "$$prohibit" && test -z "$$require"				\
     && { msg='Should specify either prohibit or require'		\
          $(_sc_say_and_exit) } || :;					\
   test -n "$$in_vc_files" && test -n "$$in_files"			\
     && { msg='Cannot specify both in_vc_files and in_files'		\
          $(_sc_say_and_exit) } || :;					\
   test "x$$halt" != x							\
     || { msg='halt not defined' $(_sc_say_and_exit) };			\
									\
   : Filter by file name;						\
   if test -n "$$in_files"; then					\
     files=$$(find $(srcdir) | grep -E "$$in_files"			\
              | grep -Ev '$(exclude_file_name_regexp--$@)');		\
   else									\
     files=$$($(VC_LIST_EXCEPT));					\
     if test -n "$$in_vc_files"; then					\
       files=$$(echo "$$files" | grep -E "$$in_vc_files");		\
     fi;								\
   fi;									\
									\
   : Filter by content;							\
   test -n "$$files" && test -n "$$containing"				\
     && { files=$$(grep -l "$$containing" $$files); } || :;		\
   test -n "$$files" && test -n "$$non_containing"			\
     && { files=$$(grep -vl "$$non_containing" $$files); } || :;	\
									\
   : Check for the construct;						\
   if test -n "$$files"; then						\
     if test -n "$$prohibit"; then					\
       grep $$with_grep_options $(_ignore_case) -nE "$$prohibit" $$files \
         && { msg="$$halt" $(_sc_say_and_exit) } || :;			\
     else								\
       grep $$with_grep_options $(_ignore_case) -LE "$$require" $$files \
           | grep .							\
         && { msg="$$halt" $(_sc_say_and_exit) } || :;			\
     fi									\
   else :;								\
   fi || :;
endef

sc_avoid_if_before_free:
	@$(srcdir)/$(_build-aux)/useless-if-before-free			\
		$(useless_free_options)					\
	    $$($(VC_LIST_EXCEPT) | grep -v useless-if-before-free) &&	\
	  { echo '$(ME): found useless "if" before "free" above' 1>&2;	\
	    exit 1; } || :

sc_cast_of_argument_to_free:
	@prohibit='\<free *\( *\(' halt='don'\''t cast free argument'	\
	  $(_sc_search_regexp)

sc_cast_of_x_alloc_return_value:
	@prohibit='\*\) *x(m|c|re)alloc\>'				\
	halt='don'\''t cast x*alloc return value'			\
	  $(_sc_search_regexp)

# Use STREQ rather than comparing strcmp == 0, or != 0.
sc_prohibit_strcmp:
	@grep -nE '! *str''cmp *\(|\<str''cmp *\(.+\) *[!=]='	\
	    $$($(VC_LIST_EXCEPT))					\
	  | grep -vE ':# *define STRN?EQ\(' &&				\
	  { echo '$(ME): replace str''cmp calls above with STREQ/STRNEQ' \
		1>&2; exit 1; } || :

# Pass EXIT_*, not number, to usage, exit, and error (when exiting)
# Convert all uses automatically, via these two commands:
# git grep -l '\<exit *(1)' \
#  | grep -vEf .x-sc_prohibit_magic_number_exit \
#  | xargs --no-run-if-empty \
#      perl -pi -e 's/(^|[^.])\b(exit ?)\(1\)/$1$2(EXIT_FAILURE)/'
# git grep -l '\<exit *(0)' \
#  | grep -vEf .x-sc_prohibit_magic_number_exit \
#  | xargs --no-run-if-empty \
#      perl -pi -e 's/(^|[^.])\b(exit ?)\(0\)/$1$2(EXIT_SUCCESS)/'
sc_prohibit_magic_number_exit:
	@prohibit='(^|[^.])\<(usage|exit) ?\([0-9]|\<error ?\([1-9][0-9]*,'	\
	halt='use EXIT_* values rather than magic number'			\
	  $(_sc_search_regexp)

# `FATAL:' should be fully upper-cased in error messages
# `WARNING:' should be fully upper-cased, or fully lower-cased
sc_error_message_warn_fatal:
	@grep -nEA2 '[^rp]error *\(' $$($(VC_LIST_EXCEPT))		\
	    | grep -E '"Warning|"Fatal|"fatal' &&			\
	  { echo '$(ME): use FATAL, WARNING or warning'	1>&2;		\
	    exit 1; } || :

# Error messages should not end with a period
sc_error_message_period:
	@grep -nEA2 '[^rp]error *\(' $$($(VC_LIST_EXCEPT))		\
	    | grep -E '[^."]\."' &&					\
	  { echo '$(ME): found error message ending in period' 1>&2;	\
	    exit 1; } || :

# Don't use cpp tests of this symbol.  All code assumes config.h is included.
sc_prohibit_have_config_h:
	@prohibit='^# *if.*HAVE''_CONFIG_H'				\
	halt='found use of HAVE''_CONFIG_H; remove'			\
	  $(_sc_search_regexp)

# Nearly all .c files must include <config.h>.  However, we also permit this
# via inclusion of a package-specific header, if syntax-check.mk specified one.
# config_h_header must be suitable for grep -E.
config_h_header ?= <config\.h>
sc_require_config_h:
	@require='^# *include $(config_h_header)'			\
	in_vc_files='\.c$$'						\
	halt='the above files do not include <config.h>'		\
	  $(_sc_search_regexp)

# You must include <config.h> before including any other header file.
# This can possibly be via a package-specific header, if given by syntax-check.mk.
sc_require_config_h_first:
	@if $(VC_LIST_EXCEPT) | grep -l '\.c$$' > /dev/null; then	\
	  fail=0;							\
	  for i in $$($(VC_LIST_EXCEPT) | grep '\.c$$'); do		\
	    grep '^# *include\>' $$i | sed 1q				\
		| grep -E '^# *include $(config_h_header)' > /dev/null	\
	      || { echo $$i; fail=1; };					\
	  done;								\
	  test $$fail = 1 &&						\
	    { echo '$(ME): the above files include some other header'	\
		'before <config.h>' 1>&2; exit 1; } || :;		\
	else :;								\
	fi

sc_prohibit_HAVE_MBRTOWC:
	@prohibit='\bHAVE_MBRTOWC\b'					\
	halt="do not use $$prohibit; it is always defined"		\
	  $(_sc_search_regexp)

# To use this "command" macro, you must first define two shell variables:
# h: the header name, with no enclosing <> or ""
# re: a regular expression that matches IFF something provided by $h is used.
define _sc_header_without_use
  dummy=; : so we do not need a semicolon before each use;		\
  h_esc=`echo '[<"]'"$$h"'[">]'|sed 's/\./\\\\./g'`;			\
  if $(VC_LIST_EXCEPT) | grep -l '\.c$$' > /dev/null; then		\
    files=$$(grep -l '^# *include '"$$h_esc"				\
	     $$($(VC_LIST_EXCEPT) | grep '\.c$$')) &&			\
    grep -LE "$$re" $$files | grep . &&					\
      { echo "$(ME): the above files include $$h but don't use it"	\
	1>&2; exit 1; } || :;						\
  else :;								\
  fi
endef

# Prohibit the inclusion of assert.h without an actual use of assert.
sc_prohibit_assert_without_use:
	@h='assert.h' re='\<assert *\(' $(_sc_header_without_use)

# Prohibit the inclusion of close-stream.h without an actual use.
sc_prohibit_close_stream_without_use:
	@h='close-stream.h' re='\<close_stream *\(' $(_sc_header_without_use)

# Prohibit the inclusion of getopt.h without an actual use.
sc_prohibit_getopt_without_use:
	@h='getopt.h' re='\<getopt(_long)? *\(' $(_sc_header_without_use)

# Don't include this header unless you use one of its functions.
sc_prohibit_long_options_without_use:
	@h='long-options.h' re='\<parse_long_options *\(' \
	  $(_sc_header_without_use)

# Don't include this header unless you use one of its functions.
sc_prohibit_inttostr_without_use:
	@h='inttostr.h' re='\<(off|[iu]max|uint)tostr *\(' \
	  $(_sc_header_without_use)

# Don't include this header unless you use one of its functions.
sc_prohibit_ignore_value_without_use:
	@h='ignore-value.h' re='\<ignore_(value|ptr) *\(' \
	  $(_sc_header_without_use)

# Don't include this header unless you use one of its functions.
sc_prohibit_error_without_use:
	@h='error.h' \
	re='\<error(_at_line|_print_progname|_one_per_line|_message_count)? *\('\
	  $(_sc_header_without_use)

# Don't include xalloc.h unless you use one of its functions.
# Consider these symbols:
# perl -lne '/^# *define (\w+)\(/ and print $1' lib/xalloc.h|grep -v '^__';
# perl -lne '/^(?:extern )?(?:void|char) \*?(\w+) *\(/ and print $1' lib/xalloc.h
# Divide into two sets on case, and filter each through this:
# | sort | perl -MRegexp::Assemble -le \
#  'print Regexp::Assemble->new(file => "/dev/stdin")->as_string'|sed 's/\?://g'
# Note this was produced by the above:
# _xa1 = \
#x(((2n?)?re|c(har)?|n(re|m)|z)alloc|alloc_(oversized|die)|m(alloc|emdup)|strdup)
# But we can do better, in at least two ways:
# 1) take advantage of two "dup"-suffixed strings:
# x(((2n?)?re|c(har)?|n(re|m)|[mz])alloc|alloc_(oversized|die)|(mem|str)dup)
# 2) notice that "c(har)?|[mz]" is equivalent to the shorter and more readable
# "char|[cmz]"
# x(((2n?)?re|char|n(re|m)|[cmz])alloc|alloc_(oversized|die)|(mem|str)dup)
_xa1 = x(((2n?)?re|char|n(re|m)|[cmz])alloc|alloc_(oversized|die)|(mem|str)dup)
_xa2 = X([CZ]|N?M)ALLOC
sc_prohibit_xalloc_without_use:
	@h='xalloc.h' \
	re='\<($(_xa1)|$(_xa2)) *\('\
	  $(_sc_header_without_use)

sc_prohibit_cloexec_without_use:
	@h='cloexec.h' re='\<(set_cloexec_flag|dup_cloexec) *\(' \
	  $(_sc_header_without_use)

sc_prohibit_posixver_without_use:
	@h='posixver.h' re='\<posix2_version *\(' $(_sc_header_without_use)

sc_prohibit_same_without_use:
	@h='same.h' re='\<same_name *\(' $(_sc_header_without_use)

sc_prohibit_hash_pjw_without_use:
	@h='hash-pjw.h' \
	re='\<hash_pjw *\(' \
	  $(_sc_header_without_use)

sc_prohibit_safe_read_without_use:
	@h='safe-read.h' re='(\<SAFE_READ_ERROR\>|\<safe_read *\()' \
	  $(_sc_header_without_use)

sc_prohibit_argmatch_without_use:
	@h='argmatch.h' \
	re='(\<(ARRAY_CARDINALITY|X?ARGMATCH(|_TO_ARGUMENT|_VERIFY))\>|\<(invalid_arg|argmatch(_exit_fn|_(in)?valid)?) *\()' \
	  $(_sc_header_without_use)

sc_prohibit_canonicalize_without_use:
	@h='canonicalize.h' \
	re='CAN_(EXISTING|ALL_BUT_LAST|MISSING)|canonicalize_(mode_t|filename_mode)' \
	  $(_sc_header_without_use)

sc_prohibit_root_dev_ino_without_use:
	@h='root-dev-ino.h' \
	re='(\<ROOT_DEV_INO_(CHECK|WARN)\>|\<get_root_dev_ino *\()' \
	  $(_sc_header_without_use)

sc_prohibit_openat_without_use:
	@h='openat.h' \
	re='\<(openat_(permissive|needs_fchdir|(save|restore)_fail)|l?(stat|ch(own|mod))at|(euid)?accessat)\>' \
	  $(_sc_header_without_use)

# Prohibit the inclusion of c-ctype.h without an actual use.
ctype_re = isalnum|isalpha|isascii|isblank|iscntrl|isdigit|isgraph|islower\
|isprint|ispunct|isspace|isupper|isxdigit|tolower|toupper
sc_prohibit_c_ctype_without_use:
	@h='c-ctype.h' re='\<c_($(ctype_re)) *\(' \
	  $(_sc_header_without_use)

_empty =
_sp = $(_empty) $(_empty)
# The following list was generated by running:
# man signal.h|col -b|perl -ne '/bsd_signal.*;/.../sigwaitinfo.*;/ and print' \
#   | perl -lne '/^\s+(?:int|void).*?(\w+).*/ and print $1' | fmt
_sig_functions = \
  bsd_signal kill killpg pthread_kill pthread_sigmask raise sigaction \
  sigaddset sigaltstack sigdelset sigemptyset sigfillset sighold sigignore \
  siginterrupt sigismember signal sigpause sigpending sigprocmask sigqueue \
  sigrelse sigset sigsuspend sigtimedwait sigwait sigwaitinfo
_sig_function_re = $(subst $(_sp),|,$(strip $(_sig_functions)))
# The following were extracted from "man signal.h" manually.
_sig_types_and_consts =							\
  MINSIGSTKSZ SA_NOCLDSTOP SA_NOCLDWAIT SA_NODEFER SA_ONSTACK		\
  SA_RESETHAND SA_RESTART SA_SIGINFO SIGEV_NONE SIGEV_SIGNAL		\
  SIGEV_THREAD SIGSTKSZ SIG_BLOCK SIG_SETMASK SIG_UNBLOCK SS_DISABLE	\
  SS_ONSTACK mcontext_t pid_t sig_atomic_t sigevent siginfo_t sigset_t	\
  sigstack sigval stack_t ucontext_t
# generated via this:
# perl -lne '/^#ifdef (SIG\w+)/ and print $1' lib/sig2str.c|sort -u|fmt -70
_sig_names =								\
  SIGABRT SIGALRM SIGALRM1 SIGBUS SIGCANCEL SIGCHLD SIGCLD SIGCONT	\
  SIGDANGER SIGDIL SIGEMT SIGFPE SIGFREEZE SIGGRANT SIGHUP SIGILL	\
  SIGINFO SIGINT SIGIO SIGIOT SIGKAP SIGKILL SIGKILLTHR SIGLOST SIGLWP	\
  SIGMIGRATE SIGMSG SIGPHONE SIGPIPE SIGPOLL SIGPRE SIGPROF SIGPWR	\
  SIGQUIT SIGRETRACT SIGSAK SIGSEGV SIGSOUND SIGSTKFLT SIGSTOP SIGSYS	\
  SIGTERM SIGTHAW SIGTRAP SIGTSTP SIGTTIN SIGTTOU SIGURG SIGUSR1	\
  SIGUSR2 SIGVIRT SIGVTALRM SIGWAITING SIGWINCH SIGWIND SIGWINDOW	\
  SIGXCPU SIGXFSZ
_sig_syms_re = $(subst $(_sp),|,$(strip $(_sig_names) $(_sig_types_and_consts)))

# Prohibit the inclusion of signal.h without an actual use.
sc_prohibit_signal_without_use:
	@h='signal.h'							\
	re='\<($(_sig_function_re)) *\(|\<($(_sig_syms_re))\>'		\
	  $(_sc_header_without_use)

# Don't include stdio--.h unless you use one of its functions.
sc_prohibit_stdio--_without_use:
	@h='stdio--.h' re='\<((f(re)?|p)open|tmpfile) *\('		\
	  $(_sc_header_without_use)

# Don't include stdio-safer.h unless you use one of its functions.
sc_prohibit_stdio-safer_without_use:
	@h='stdio-safer.h' re='\<((f(re)?|p)open|tmpfile)_safer *\('	\
	  $(_sc_header_without_use)

# Prohibit the inclusion of strings.h without a sensible use.
# Using the likes of bcmp, bcopy, bzero, index or rindex is not sensible.
sc_prohibit_strings_without_use:
	@h='strings.h'							\
	re='\<(strn?casecmp|ffs(ll)?)\>'				\
	  $(_sc_header_without_use)

# Get the list of symbol names with this:
# perl -lne '/^# *define ([A-Z]\w+)\(/ and print $1' lib/intprops.h|fmt
_intprops_names =							\
  TYPE_IS_INTEGER TYPE_TWOS_COMPLEMENT TYPE_ONES_COMPLEMENT		\
  TYPE_SIGNED_MAGNITUDE TYPE_SIGNED TYPE_MINIMUM TYPE_MAXIMUM		\
  INT_BITS_STRLEN_BOUND INT_STRLEN_BOUND INT_BUFSIZE_BOUND		\
  INT_ADD_RANGE_OVERFLOW INT_SUBTRACT_RANGE_OVERFLOW			\
  INT_NEGATE_RANGE_OVERFLOW INT_MULTIPLY_RANGE_OVERFLOW			\
  INT_DIVIDE_RANGE_OVERFLOW INT_REMAINDER_RANGE_OVERFLOW		\
  INT_LEFT_SHIFT_RANGE_OVERFLOW INT_ADD_OVERFLOW INT_SUBTRACT_OVERFLOW	\
  INT_NEGATE_OVERFLOW INT_MULTIPLY_OVERFLOW INT_DIVIDE_OVERFLOW		\
  INT_REMAINDER_OVERFLOW INT_LEFT_SHIFT_OVERFLOW
_intprops_syms_re = $(subst $(_sp),|,$(strip $(_intprops_names)))
# Prohibit the inclusion of intprops.h without an actual use.
sc_prohibit_intprops_without_use:
	@h='intprops.h'							\
	re='\<($(_intprops_syms_re)) *\('				\
	  $(_sc_header_without_use)

_stddef_syms_re = NULL|offsetof|ptrdiff_t|size_t|wchar_t
# Prohibit the inclusion of stddef.h without an actual use.
sc_prohibit_stddef_without_use:
	@h='stddef.h'							\
	re='\<($(_stddef_syms_re))\>'					\
	  $(_sc_header_without_use)

_de1 = dirfd|(close|(fd)?open|read|rewind|seek|tell)dir(64)?(_r)?
_de2 = (versionsort|struct dirent|getdirentries|alphasort|scandir(at)?)(64)?
_de3 = MAXNAMLEN|DIR|ino_t|d_ino|d_fileno|d_namlen
_dirent_syms_re = $(_de1)|$(_de2)|$(_de3)
# Prohibit the inclusion of dirent.h without an actual use.
sc_prohibit_dirent_without_use:
	@h='dirent.h'							\
	re='\<($(_dirent_syms_re))\>'					\
	  $(_sc_header_without_use)

# Prohibit the inclusion of verify.h without an actual use.
sc_prohibit_verify_without_use:
	@h='verify.h'							\
	re='\<(verify(true|expr)?|static_assert) *\('			\
	  $(_sc_header_without_use)

# Don't include xfreopen.h unless you use one of its functions.
sc_prohibit_xfreopen_without_use:
	@h='xfreopen.h' re='\<xfreopen *\(' $(_sc_header_without_use)

# Each nonempty ChangeLog line must start with a year number, or a TAB.
sc_changelog:
	@prohibit='^[^12	]'					\
	in_vc_files='^ChangeLog$$'					\
	halt='found unexpected prefix in a ChangeLog'			\
	  $(_sc_search_regexp)

# Ensure that each .c file containing a "main" function also
# calls bindtextdomain.
sc_bindtextdomain:
	@require='bindtextdomain *\('					\
	in_vc_files='\.c$$'						\
	containing='\<main *('						\
	halt='the above files do not call bindtextdomain'		\
	  $(_sc_search_regexp)

sc_trailing_blank:
	@prohibit='[	 ]$$'						\
	halt='found trailing blank(s)'					\
	  $(_sc_search_regexp)

# A regexp matching function names like "error" that may be used
# to emit translatable messages.
_gl_translatable_diag_func_re ?= error

# Look for diagnostics that aren't marked for translation.
# This won't find any for which error's format string is on a separate line.
sc_unmarked_diagnostics:
	@grep -nE							\
	    '\<$(_gl_translatable_diag_func_re) *\([^"]*"[^"]*[a-z]{3}' \
		$$($(VC_LIST_EXCEPT))					\
	  | grep -Ev '(_|ngettext ?)\(' &&				\
	  { echo '$(ME): found unmarked diagnostic(s)' 1>&2;		\
	    exit 1; } || :

# Prohibit checked in backup files.
sc_prohibit_backup_files:
	@$(VC_LIST) | grep '~$$' &&				\
	  { echo '$(ME): found version controlled backup file' 1>&2;	\
	    exit 1; } || :

# Require the latest GFDL.  Two regexp, since some .texi files end up
# line wrapping between 'Free Documentation License,' and 'Version'.
_GFDL_regexp = (Free ''Documentation.*Version 1\.[^3]|Version 1\.[^3] or any)
sc_GFDL_version:
	@prohibit='$(_GFDL_regexp)'					\
	halt='GFDL vN, N!=3'						\
	  $(_sc_search_regexp)

# This Perl code is slightly obfuscated.  Not only is each "$" doubled
# because it's in a Makefile, but the $$c's are comments;  we cannot
# use "#" due to the way the script ends up concatenated onto one line.
# It would be much more concise, and would produce better output (including
# counts) if written as:
#   perl -ln -0777 -e '/\n(\n+)$/ and print "$ARGV: ".length $1' ...
# but that would be far less efficient, reading the entire contents
# of each file, rather than just the last two bytes of each.
# In addition, while the code below detects both blank lines and a missing
# newline at EOF, the above detects only the former.
#
# This is a perl script that is expected to be the single-quoted argument
# to a command-line "-le".  The remaining arguments are file names.
# Print the name of each file that ends in exactly one newline byte.
# I.e., warn if there are blank lines (2 or more newlines), or if the
# last byte is not a newline.  However, currently we don't complain
# about any file that contains exactly one byte.
# Exit nonzero if at least one such file is found, otherwise, exit 0.
# Warn about, but otherwise ignore open failure.  Ignore seek/read failure.
#
# Use this if you want to remove trailing empty lines from selected files:
#   perl -pi -0777 -e 's/\n\n+$/\n/' files...
#
require_exactly_one_NL_at_EOF_ =					\
  foreach my $$f (@ARGV)						\
    {									\
      open F, "<", $$f or (warn "failed to open $$f: $$!\n"), next;	\
      my $$p = sysseek (F, -2, 2);					\
      my $$c = "seek failure probably means file has < 2 bytes; ignore"; \
      my $$last_two_bytes;						\
      defined $$p and $$p = sysread F, $$last_two_bytes, 2;		\
      close F;								\
      $$c = "ignore read failure";					\
      $$p && ($$last_two_bytes eq "\n\n"				\
              || substr ($$last_two_bytes,1) ne "\n")			\
          and (print $$f), $$fail=1;					\
    }									\
  END { exit defined $$fail }
sc_prohibit_empty_lines_at_EOF:
	@perl -le '$(require_exactly_one_NL_at_EOF_)' $$($(VC_LIST_EXCEPT)) \
	  || { echo '$(ME): empty line(s) or no newline at EOF'		\
		1>&2; exit 1; } || :

# Perl block to convert a match to FILE_NAME:LINENO:TEST,
# that is shared by two definitions below.
perl_filename_lineno_text_ =						\
    -e '  {'								\
    -e '    $$n = ($$` =~ tr/\n/\n/ + 1);'				\
    -e '    ($$v = $$&) =~ s/\n/\\n/g;'					\
    -e '    print "$$ARGV:$$n:$$v\n";'					\
    -e '  }'

prohibit_doubled_word_RE_ ?= \
  /\b(then?|[iao]n|i[fst]|but|f?or|at|and|[dt]o)\s+\1\b/gims
prohibit_doubled_word_ =						\
    -e 'while ($(prohibit_doubled_word_RE_))'				\
    $(perl_filename_lineno_text_)

# Define this to a regular expression that matches
# any filename:dd:match lines you want to ignore.
# The default is to ignore no matches.
ignore_doubled_word_match_RE_ ?= ^$$

sc_prohibit_doubled_word:
	@perl -n -0777 $(prohibit_doubled_word_) $$($(VC_LIST_EXCEPT))	\
	  | grep -vE '$(ignore_doubled_word_match_RE_)'			\
	  | grep . && { echo '$(ME): doubled words' 1>&2; exit 1; } || :

# A regular expression matching undesirable combinations of words like
# "can not"; this matches them even when the two words appear on different
# lines, but not when there is an intervening delimiter like "#" or "*".
prohibit_undesirable_word_seq_RE_ ?=					\
  /\bcan\s+not\b/gims
prohibit_undesirable_word_seq_ =					\
    -e 'while ($(prohibit_undesirable_word_seq_RE_))'			\
    $(perl_filename_lineno_text_)
# Define this to a regular expression that matches
# any filename:dd:match lines you want to ignore.
# The default is to ignore no matches.
ignore_undesirable_word_sequence_RE_ ?= ^$$

sc_prohibit_undesirable_word_seq:
	@perl -n -0777 $(prohibit_undesirable_word_seq_)		\
	     $$($(VC_LIST_EXCEPT))					\
	  | grep -vE '$(ignore_undesirable_word_sequence_RE_)' | grep .	\
	  && { echo '$(ME): undesirable word sequence' >&2; exit 1; } || :

_ptm1 = use "test C1 && test C2", not "test C1 -''a C2"
_ptm2 = use "test C1 || test C2", not "test C1 -''o C2"
# Using test's -a and -o operators is not portable.
# We prefer test over [, since the latter is spelled [[ in configure.ac.
sc_prohibit_test_minus_ao:
	@prohibit='(\<test| \[+) .+ -[ao] '				\
	halt='$(_ptm1); $(_ptm2)'					\
	  $(_sc_search_regexp)

# Avoid a test bashism.
sc_prohibit_test_double_equal:
	@prohibit='(\<test| \[+) .+ == '				\
	containing='#! */bin/[a-z]*sh'					\
	halt='use "test x = x", not "test x =''= x"'			\
	  $(_sc_search_regexp)


# Warn about "c0nst struct Foo const foo[]",
# but not about "char const *const foo" or "#define const const".
sc_redundant_const:
	@prohibit='\bconst\b[[:space:][:alnum:]]{2,}\bconst\b'		\
	halt='redundant "const" in declarations'			\
	  $(_sc_search_regexp)

sc_const_long_option:
	@grep '^ *static.*struct option ' $$($(VC_LIST_EXCEPT))		\
	  | grep -Ev 'const struct option|struct option const' && {	\
	      echo 1>&2 '$(ME): add "const" to the above declarations'; \
	      exit 1; } || :

fix_po_file_diag = \
'you have changed the set of files with translatable diagnostics;\n\
apply the above patch\n'

# Verify that all source files using _() are listed in po/POTFILES.in.
po_file ?= $(srcdir)/po/POTFILES
generated_files ?= $(srcdir)/lib/*.[ch]
sc_po_check:
	@if test -f $(po_file); then					\
	  grep -E -v '^(#|$$)' $(po_file)				\
	    | grep -v '^src/false\.c$$' | sort > $@-1;			\
	  files=;							\
	  for file in $$($(VC_LIST_EXCEPT)) $(generated_files); do	\
	    test -r $$file || continue;					\
	    case $$file in						\
	      *.m4|*.mk) continue ;;					\
	      *.?|*.??) ;;						\
	      *) continue;;						\
	    esac;							\
	    case $$file in						\
	    *.[ch])							\
	      base=`expr " $$file" : ' \(.*\)\..'`;			\
	      { test -f $$base.l || test -f $$base.y; } && continue;;	\
	    esac;							\
	    files="$$files $$file";					\
	  done;								\
	  grep -E -l '\b(N?_|gettext *)\([^)"]*("|$$)' $$files		\
	    | sed 's|^$(_dot_escaped_srcdir)/||' | sort -u > $@-2;	\
	  diff -u -L $(po_file) -L $(po_file) $@-1 $@-2			\
	    || { printf '$(ME): '$(fix_po_file_diag) 1>&2; exit 1; };	\
	  rm -f $@-1 $@-2;						\
	fi

# #if HAVE_... will evaluate to false for any non numeric string.
# That would be flagged by using -Wundef, however gnulib currently
# tests many undefined macros, and so we can't enable that option.
# So at least preclude common boolean strings as macro values.
sc_Wundef_boolean:
	@prohibit='^#define.*(yes|no|true|false)$$'			\
	in_files='$(CONFIG_INCLUDE)'					\
	halt='Use 0 or 1 for macro values'				\
	  $(_sc_search_regexp)

# Even if you use pathmax.h to guarantee that PATH_MAX is defined, it might
# not be constant, or might overflow a stack.  In general, use PATH_MAX as
# a limit, not an array or alloca size.
sc_prohibit_path_max_allocation:
	@prohibit='(\balloca *\([^)]*|\[[^]]*)PATH_MAX'			\
	halt='Avoid stack allocations of size PATH_MAX'			\
	  $(_sc_search_regexp)

sc_vulnerable_makefile_CVE-2009-4029:
	@prohibit='perm -777 -exec chmod a\+rwx|chmod 777 \$$\(distdir\)' \
	in_files=$$(find $(srcdir) -name Makefile.in)			\
	halt=$$(printf '%s\n'						\
	  'the above files are vulnerable; beware of running'		\
	  '  "make dist*" rules, and upgrade to fixed automake'		\
	  '  see http://bugzilla.redhat.com/542609 for details')	\
	  $(_sc_search_regexp)

# Files that should never cause syntax check failures.
VC_LIST_ALWAYS_EXCLUDE_REGEX = \
  (^HACKING|\.po|build-aux/syntax-check.mk)$$

# Functions like free() that are no-ops on NULL arguments.
useless_free_options =				\
  --name=g_free					\
  --name=xmlBufferFree				\
  --name=xmlFree				\
  --name=xmlFreeDoc				\
  --name=xmlXPathFreeContext			\
  --name=xmlXPathFreeObject

# Ensure that no C source file, docs, or rng schema uses TABs for
# indentation.  Also match *.h.in files, to get libvirt.h.in.  Exclude
# files in gnulib, since they're imported.
space_indent_files=(\.(rng|s?[ch](\.in)?|html.in|py)|(daemon|tools)/.*\.in)
sc_TAB_in_indentation:
	@prohibit='^ *	'						\
	in_vc_files='$(space_indent_files)$$'				\
	halt='indent with space, not TAB, in C, sh, html, py, and RNG schemas' \
	  $(_sc_search_regexp)

# G_GNUC_UNUSED should only be applied in implementations, not
# header declarations
sc_avoid_attribute_unused_in_header:
	@prohibit='^[^#]*G_GNUC_UNUSED([^:]|$$)'			\
	in_vc_files='\.h$$'						\
	halt='use G_GNUC_UNUSED in .c rather than .h files'		\
	  $(_sc_search_regexp)

# Enforce recommended preprocessor indentation style.
sc_preprocessor_indentation:
	@if cppi --version >/dev/null 2>&1; then			\
	  $(VC_LIST_EXCEPT) | grep '\.[ch]$$' | xargs cppi -a -c	\
	    || { echo '$(ME): incorrect preprocessor indentation' 1>&2;	\
		exit 1; };						\
	else								\
	  echo '$(ME): skipping test $@: cppi not installed' 1>&2;	\
	fi

sc_copyright_format:
	@require='Copyright .*Red 'Hat', Inc\.'				\
	containing='Copyright .*Red 'Hat				\
	halt='Red Hat copyright is missing Inc.'			\
	  $(_sc_search_regexp)
	@prohibit='Copyright [^(].*Red 'Hat				\
	halt='consistently use (C) in Red Hat copyright'		\
	  $(_sc_search_regexp)
	@prohibit='\<Red''Hat\>'					\
	halt='spell Red Hat as two words'				\
	  $(_sc_search_regexp)

exclude_file_name_regexp--sc_bindtextdomain = ^(libvirt-gconfig/tests|examples)/|libvirt-glib/libvirt-glib-event.c|libvirt-glib/libvirt-glib-main.c|tests/

exclude_file_name_regexp--sc_preprocessor_indentation = ^*/*.[ch]

exclude_file_name_regexp--sc_prohibit_strcmp = ^*/*.[ch]

exclude_file_name_regexp--sc_unmarked_diagnostics = tap-driver\.sh
