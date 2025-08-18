# $@: used with evals
eval { die "Killers of children!"; };
print $@ if $@;

# $!: used with system call like open and unlink
open my $fh, '<', 'nofile.txt' or print "Error: $!\n";

# $?: used to get exit status of last command
