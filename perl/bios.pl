use Mac::SysProfile;

my $profile = Mac::SysProfile->new();
my $data = $profile->gettype('SPHardwareDataType');

print("length of data: " . scalar(@$data) . "\n");
print("$_\n") for @$data;

my $h = $data->[0];
print("$_: $h->{$_}\n") for keys %$h;
