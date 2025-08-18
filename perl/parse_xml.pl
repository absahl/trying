use XML::Simple;

$xml = q{
<DOWNLOAD id="123" name="SampleDownload">
</DOWNLOAD>
};

$data = XMLin($xml, ForceArray => ['FRAGMENT']);
print "download - id: " . $data->{'id'} . ", name: " . $data->{'name'}. "\n";

for my $frag (@{$data->{'FRAGMENT'}}) {
	print "frag" . $frag->{'INDEX'} . ": " . $frag->{'URL'} . "\n";
}
