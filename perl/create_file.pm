use strict;
use warnings;
use Fcntl qw(:DEFAULT :mode);

my $fd;
sysopen($fd, "test.file", O_CREAT | O_WRONLY, 0600) or die "Can't open file: $!";
syswrite($fd, "Allah Hu\n");
close($fd);
