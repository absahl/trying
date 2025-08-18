use Mac::SysProfile;

my $profile = Mac::SysProfile->new();
       my $data = $profile->gettype('SPSoftwareDataType');
       return(undef) unless(ref($data) eq 'ARRAY');

       my $h = $data->[0];

       my $SystemVersion = $h->{'os_version'};
       if ($SystemVersion =~ /^(.*?)\s+(\d+.*)/) {
           $OSName=$1;
           $OSVersion=$2;
print("osname: $OSName\n");
print("osver: $OSVersion\n");
}



