#!/usr/bin/perl

my $vBytes = [];
my $v404 = [];
my $vHits = [];
my $vClients = [];
my $vReferrers = [];

my %v = ();
$v{"B"} = $vBytes;
$v{"N"} = $v404;
$v{"H"} = $vHits;
$v{"I"} = $vClients;
$v{"R"} = $vReferrers;

while (<>) {
	next unless m{
		^
		(\S+)\s
		(\S+)\s
		(.*)
		$
	}x;
	push(@{$v{$1}},$2 . ": " . $3);
}

sub report {
	my ($title, $a) = @_;
	print "\n" . $title . "\n\t" . (join "\n\t", @{$a}) . "\n\n";  
}

report("Top 10 URIs by total response bytes",$vBytes); 
report("Top 10 URIs returning 404 (Not Found)",$v404); 
report("Top 10 URIs by hits on articles",$vHits); 
report("Top 10 client IPs by hits on articles",$vClients); 
report("Top 10 referrers by hits on articles",$vReferrers); 
