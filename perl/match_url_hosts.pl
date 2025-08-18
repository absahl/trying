use URI;

die "usage: $0 <url1> <url1>\n" unless @ARGV == 2;
my ($url1, $url2) = @ARGV;

my $u1 = URI->new($url1);
my $u2 = URI->new($url2);

if ($URI->new($url1)->host eq $u2->host) {
	print('host match <u1:' . $u1->host . '> <u2:' . $u2->host . ">\n");
} else {
	print('host does not match <u1:' . $u1->host . '> <u2:' . $u2->host . ">\n");
}
