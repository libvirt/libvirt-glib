Libvirt-Glib Message Translation
================================

Libvirt-Glib translatable messages are maintained using the GNU Gettext tools
and file formats, in combination with the Weblate web service.

Source repository
=================

The libvirt-glib GIT repository does NOT store the master "libvirt-glib.pot"
file, nor does it store full "po" files for translations. The master
"libvirt-glib.pot" file can be generated at any time using

   make libvirt-glib.pot

The translations are kept in minimized files that are the same file format
as normal po files but with all redundant information stripped and messages
re-ordered. The key differences between the ".mini.po" files in GIT and the
full ".po" files are

  - msgids with no current translation are omitted
  - msgids are sorted in alphabetical order not source file order
  - msgids with a msgstr marked "fuzzy" are discarded
  - source file locations are omitted

The full po files can be created at any time using

   make update-po

This merges the "libvirt-glib.pot" with the "$LANG.mini.po" for each language,
to create the "$LANG.po" files. These are included in the release archives
created by "make dist".

When a full po file is updated, changes can be propagated back into the
minimized po files using

   make update-mini-po

Note, however, that this is generally not something that should be run by
developers normally, as it is triggered by 'make pull-po' when refreshing
content from Weblate.


Weblate web service
===================

The translation of libvirt-glib messages has been outsourced to the Fedora
translation team using the Weblate web service:

  https://translate.stg.fedoraproject.org/projects/libvirt/libvirt-glib

As such, changes to translations will generally NOT be accepted as patches
directly to libvirt-glib GIT. Any changes made to "$LANG.mini.po" files in
libvirt-glib GIT will be overwritten and lost the next time content is
imported from Weblate.

The master "libvirt-glib.pot" file is periodically pushed to Weblate to provide
the translation team with content changes, using

  make push-pot

New translated text is then periodically pulled down from Weblate to update the
minimized po files, using

  make pull-po

Sometimes the translators make mistakes, most commonly with handling printf
format specifiers. The "pull-po" command re-generates the .gmo files to try to
identify such mistakes. If a mistake is made, the broken msgstr should be
deleted in the local "$LANG.mini.po" file, and the Weblate web interface used
to reject the translation so that the broken msgstr isn't pulled down next time.

After pulling down new content the diff should be examined to look for any
obvious mistakes that are not caught automatically.