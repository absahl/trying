use strict;
use warnings;
use Time::HiRes;
use Log::Log4perl;
use Fcntl qw(:flock FD_CLOEXEC F_GETFD F_SETFD);

my $LOCK_FILE = 'file.lock';

sub disable_inheritance_on_file_handle {
	my $fh = shift;
	my $logger = Log::Log4perl->get_logger();
	$logger->debug("Disabling inheritance on file handle [$fh]");

	unless(defined $fh) {
		$logger->error("File handle is undefined");
		return 0;
	}

	$logger->debug("Getting file descriptor for file handle [$fh]");
	my $fd = fileno($fh);
	unless(defined $fd) {
		$logger->error("Failed to get file descriptor for file handle [$fh]");
		return 0;
	}
	$logger->info("File descriptor obtained for file handle [$fh] [fd:$fd]");

	$logger->debug("Getting flags for file descriptor [$fd]");
	my $flags = fcntl($fh, F_GETFD, 0);
	unless(defined $flags) {
		$logger->error("Failed to get flags for file descriptor [$fd] [err:$!]");
		return 0;
	}
	$logger->info("Flags obtained for file descriptor [$fd] [flags:$flags]");

	my $new_flags = $flags | FD_CLOEXEC;
	$logger->debug("Setting FD_CLOEXEC on file descriptor [$fd] [new_flags:$new_flags]");
	unless (fcntl($fh, F_SETFD, $new_flags)) {
		$logger->error("Failed to set FD_CLOEXEC on file descriptor [$fd] [new_flags:$new_flags] [err:$!]");
		return 0;
	}
	$logger->info("FD_CLOEXEC set on file descriptor [$fd] [new_flags:$new_flags]");
	return 1;
}

my $conf = q(
	log4perl.rootLogger=DEBUG, Screen
	log4perl.appender.Screen=Log::Log4perl::Appender::Screen	
	log4perl.appender.Screen.layout=Log::Log4perl::Layout::PatternLayout
	log4perl.appender.Screen.layout.ConversionPattern=%d{yyyy-MM-dd'T'HH:mm:ss.SSSZ} %p [PID:%P] %m%n
);

Log::Log4perl->init(\$conf);
my $logger = Log::Log4perl->get_logger();

$logger->info("Testing file locking behavior with/without inheritance");

$logger->debug("Opening file for locking [$LOCK_FILE]");
my $lock_fh;
unless (open($lock_fh, '>', $LOCK_FILE)) {
	$logger->error("Failed to open file for locking [$LOCK_FILE] [err:$!]");
	exit 1;
}
$logger->info("File opened [$LOCK_FILE]");

$logger->debug("Obtaining lock on the file [$LOCK_FILE]");
unless (flock($lock_fh, LOCK_EX | LOCK_NB)) {
	$logger->error("Failed to obtain lock on the file [$LOCK_FILE] [err:$!]");
	exit 1;
}
$logger->info("Lock obtained on the file [$LOCK_FILE]");

$logger->debug("Disabling inheritance on file handle");
unless (disable_inheritance_on_file_handle($lock_fh)) {
	$logger->error("Failed to disable inheritance on file handle");
	exit 1;
}
$logger->info("Inheritance disabled on file handle");

$logger->debug("Forking child process");
my $pid = fork();
if ($pid == -1) {
	$logger->error("Failed to fork child process [err:$!]");
	exit 1;
} elsif ($pid == 0) {
	$logger->info("Child process started");
	exec('sleep', '30');
	# while (1) {
	# 	$logger->debug("Child process waiting for 1 second");
	# 	sleep(1);
	# }
}

$logger->info("Parent process exiting");
exit 0;
