#!/usr/bin/env perl

# Copyright (C) 2012-2013 Red Hat, Inc.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library.  If not, see
# <http://www.gnu.org/licenses/>.

use strict;
use warnings;

die "syntax: $0 SYMFILE..." unless int(@ARGV) >= 1;

my $ret = 0;
my $incomment = 0;
my $name;
foreach my $symfile (@ARGV) {
    open SYMFILE, $symfile or die "cannot read $symfile: $!";

    my $line = 0;
    my @group;

    while (<SYMFILE>) {
        chomp;

	if ($incomment) {
	    if (m,\*/,) {
		$incomment = 0;
	    } else {
		# skip
	    }
	} else {
	    if (m,/\*,) {
		$incomment = 1;
	    } elsif (/^(.*)\s*{\s*$/) {
		@group = ();
		$line = $.;
		$name = $1;
	    } elsif (/^\s*}(.*);$/) {
		&check_sorting(\@group, $symfile, $line, $name);
	    } elsif (/^\s*(global|local):/) {
		# skip
	    } elsif (/^\s*\*;\s*$/) {
		# skip
	    } elsif (/^\s*$/) {
		# skip
	    } else {
		$_ =~ s/;//;
		push @group, $_;
	    }
	}
    }

    close SYMFILE;
}

sub check_sorting {
    my $group = shift;
    my $symfile = shift;
    my $line = shift;
    my $name = shift;

    my @group = @{$group};
    my @sorted = sort { lc $a cmp lc $b } @group;
    my $sorted = 1;
    my $first;
    my $last;

    # Check that symbols within a group are in order
    for (my $i = 0 ; $i <= $#sorted ; $i++) {
        if ($sorted[$i] ne $group[$i]) {
            $first = $i unless defined $first;
            $last = $i;
            $sorted = 0;
        }
    }
    if (!$sorted) {
        @group = splice @group, $first, ($last-$first+1);
        @sorted = splice @sorted, $first, ($last-$first+1);
        print "Symbol block $name at $symfile:$line: symbols not sorted\n";
        print map { "  " . $_ . "\n" } @group;
        print "Correct ordering\n";
        print map { "  " . $_ . "\n" } @sorted;
        print "\n";
        $ret = 1;
    }
}

exit $ret;
