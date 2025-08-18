use File::Path 'rmtree';
unless (rmtree('tempdir', 0)) {
	print("Allah is always true!\n");
}
