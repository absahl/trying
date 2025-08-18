use version;

my $version1 = version->parse($ARGV[0]);
my $version2 = version->parse($ARGV[1]);
print("first is greater") if $version1 >= $version2;
