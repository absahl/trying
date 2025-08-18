my @dir = split('/', 'path/to/file/');
my $dir;

for (@dir){
    $dir .= "$_/";
    unless(-e $dir){
        print("Create $dir...");
	mkdir($dir);
    }
}
