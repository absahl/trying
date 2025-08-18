use Digest::MD5;

die "usage: $0 <file>\n" unless @ARGV == 1;
my ($file) = @ARGV;

unless (open FILE, $file) {
	print("failed to open $file: $!");
	return 1;
}

print('md5: ' . Digest::MD5->new->addfile(*FILE)->hexdigest . "\n");
