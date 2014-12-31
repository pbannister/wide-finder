#!/usr/bin/perl
#
#	Report the top 10 URIs of any kind by aggregate byte count downloaded
#	Report the top 10 URIs of any kind that returned 404 Not Found
#	Report the top 10 URIs, by hits, which are articles
#	Report the top 10 client addresses that fetched hit-counted articles
#	Report the top 10 referrers for hit-counted articles
#

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

	#REMOTE_ADDR      : $1
	#REMOTE_USER      : $2
	#AUTH_USER        : $3
	#DATE             : $4
	#REQUEST_METHOD   : $5
	#REQUEST_URL      : $6
	#REQUEST_PROTOCOL : $7
	#STATUS           : $8
	#BYTES            : $9 
	#REFERRER         : $10
	#AGENT            : $11
	
	my ($remote,$uri,$status,$size,$ref) = ($1,$6,$8,$9,$10);
	
	$$vBytes{$uri} += $size unless "-" eq $size;
	$$v404{$uri}++ if "404" eq $status;

#	next unless m{"GET /weblog};
	next unless m{"GET /ongoing/When/};

	$$vHits{$uri}++;
	$$vClients{$remote}++;
#	$$vReferrers{$ref}++ unless "-" eq $ref || $ref =~ m{^http://bannister.us/};
	$$vReferrers{$ref}++ unless "-" eq $ref || $ref =~ m{^http://www.tbray.org/ongoing/};
}

sub reportByN {
	my ($title, $kv) = @_;
	my @keys = sort { $$kv{$b} <=> $$kv{$a} or $a cmp $b } keys %$kv;
	print "\n" . $title . "\n\t" . (join "\n\t", map { $$kv{$_} . ": " . $_ } @keys[0..9]) . "\n\n";  
}

reportByN("Top 10 URIs by total response bytes",$vBytes); 
reportByN("Top 10 URIs returning 404 (Not Found)",$v404); 
reportByN("Top 10 URIs by hits on articles",$vHits); 
reportByN("Top 10 client IPs by hits on articles",$vClients); 
reportByN("Top 10 referrers by hits on articles",$vReferrers); 
