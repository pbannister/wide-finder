#!/usr/bin/perl

my %v = ();

while (<>) {
    m{^(\d+)\t(.*)$};
    $v{$2} += $1;
}

foreach $k (sort keys %v) {
    print "$v{$k}\t$k\n"; 
}

