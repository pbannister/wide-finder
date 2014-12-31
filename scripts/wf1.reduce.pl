#!/usr/bin/perl

my %v = ();

while (<>) {
    $v{$1}++ if m{"GET /(weblog/\d\d\d\d/[^ ]*) };
}

foreach $k (keys %v) {
    print $v{$k} . "\t$k\n";
}

