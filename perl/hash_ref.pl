my $hash_ref = {name => 'Saad', age => 29};
print 'name: ' . $hash_ref->{name} . ', age: ' . $hash_ref->{age} . "\n";

my %hash = %$hash_ref;
print 'name: ' . $hash{name} . ', age: ' . $hash{age} . "\n";
