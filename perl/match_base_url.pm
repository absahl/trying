my $base_url = 'https://mycdn.com';
my @urls = ('https://mycdn.com/1', 'https://mycdn1.com/1');
for my $url (@urls) {
	unless ($url =~ /^\Q$base_url\E/) {
		print("$url is invalid\n");
	}
}

