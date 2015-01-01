#!/usr/bin/perl

my $vBytes = {};
my $v404 = {};
my $vHits = {};
my $vClients = {};
my $vReferrers = {};

while (<>) {
	next unless m{
		^
		(\S+)\s
		(\S+)\s
		(\S+)\s
		\[([^]]+)\]\s
		"(\S+)\s
		((?:\\.|[^\s\\])+)\s
		([^"]+)"\s
		(\S+)\s
		(\S+)\s
		"([^"]*)"\s
		"([^"]*)"
	}x;
	
	my ($remote,$uri,$status,$size,$ref) = ($1,$6,$8,$9,$10);
	
	$$vBytes{$uri} += $size unless "-" eq $size;
	$$v404{$uri}++ if "404" eq $status;

	next unless m{"GET /weblog/2};
#	next unless m{"GET /ongoing/When/};

	$$vHits{$uri}++;
	$$vClients{$remote}++;
	$$vReferrers{$ref}++ unless "-" eq $ref || $ref =~ m{^http://bannister.us/} || "" eq $ref;
#	$$vReferrers{$ref}++ unless "-" eq $ref || $ref =~ m{^http://www.tbray.org/ongoing/} || "" eq $ref;
}

sub outBy {
	my ($prefix,$kv) = @_;
	$prefix = "\n" . $prefix . " ";
	while (my ($k, $v) = each %$kv) {
		print $prefix . $v . " " . $k;
	}
	print "\n";
}

outBy("B",$vBytes); 
outBy("N",$v404); 
outBy("H",$vHits); 
outBy("I",$vClients); 
outBy("R",$vReferrers); 
