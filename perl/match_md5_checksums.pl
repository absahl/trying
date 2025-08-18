use Digest::MD5;

die "usage: $0 <file> <checksum>\n" unless @ARGV == 2;
my ($file, $checksum) = @ARGV;

unless (open FILE, $file) {
	print("failed to open $file: $!");
	return 1;
}

my $digest = Digest::MD5->new->addfile(*FILE)->hexdigest;

print('md5 <' . $digest . ">\n");
print('checksum <' . $checksum . ">\n");
print('again <' . Digest::MD5->new->addfile(*FILE)->hexdigest . ">\n");

if ($digest ne $checksum) {
	print("digest does not match the checksum\n");
	return 1;
} else {
	print("digest is OK\n");
}
