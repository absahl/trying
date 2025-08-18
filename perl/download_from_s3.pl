use LWP::UserAgent;
use Mozilla::CA;

my $ua = LWP::UserAgent->new;
my $url = "https://software-packages-executables.s3.eu-north-1.amazonaws.com/535/94e58865409b7fe1189a1dfe485590c9?response-content-disposition=attachment%3B%20filename%3Deverything.exe&X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIASKYEEDXZJ4RXVJ44%2F20250707%2Feu-north-1%2Fs3%2Faws4_request&X-Amz-Date=20250707T055726Z&X-Amz-Expires=172800&X-Amz-SignedHeaders=host&X-Amz-Signature=ffd710688a25835904fa52e48d76164904d79a609dc0bd9288000f0799fcdae8";
my $res = $ua->get($url, ':content_file' => 'everything.exe');

die $res->status_line unless $res->is_success;
print "May Allah help me download file successfully from S3";
