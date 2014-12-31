#!/usr/bin/perl

my $vBytes     = {};
my $v404       = {};
my $vHits      = {};
my $vClients   = {};
my $vReferrers = {};

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
	$v{$1}->{$3} += $2;
}

sub sampleTop {
	my ( $n, $kv ) = @_;
	my $iLast  = $n - 1;
	my $nLimit = 100 + $n;
	my $min    = 0;
	my @sample = ();
	while ( my ( $k, $v ) = each(%$kv) ) {
		next if $v < $min;
		push( @sample, [ $v, $k ] );
		next if ( scalar @sample ) < $nLimit;
		@sample = sort { $b->[0] <=> $a->[0] } @sample;
		$#sample = $iLast;
		$min = $sample[$#sample]->[0];
	}
	@sample = sort { $b->[0] <=> $a->[0] } @sample;
	$#sample = $iLast;
	return @sample;
}

sub reportByN {
	  my ( $title, $kv ) = @_;
	  my @top = sampleTop( 10, $kv );
	  my $got = join "\n\t", map { $$_[0] . ": " . $$_[1] } @top; 
	  print "\n$title\n\t$got\n\n";
}

reportByN( "Top 10 URIs by total response bytes",   $vBytes );
reportByN( "Top 10 URIs returning 404 (Not Found)", $v404 );
reportByN( "Top 10 URIs by hits on articles",       $vHits );
reportByN( "Top 10 client IPs by hits on articles", $vClients );
reportByN( "Top 10 referrers by hits on articles",  $vReferrers );

