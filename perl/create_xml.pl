use strict;
use warnings;
use XML::Simple;

my $data = {
    UPDATE => {
        URL_SUFFIX   => '', 
        VERSION      => '10.10.10',
        ARGS         => '/S',
        FORCE_UPDATE => '0',
    }
};

my $xml = XMLout(
    $data,
    RootName   => undef,   # prevent wrapping in extra root element
    XMLDecl    => '<?xml version="1.0" encoding="UTF-8"?>',
);

print $xml;

