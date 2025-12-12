### File locking and FD inheritance (Perl)

This project demonstrates how file locks behave across `fork`/`exec` boundaries in Perl, and how marking a file descriptor as close-on-exec (`FD_CLOEXEC`) prevents the lock and descriptor from being inherited by a subsequently `exec`'d program.

At a high level, `main.pl`:
- Opens `file.lock`
- Acquires a non-blocking exclusive lock with `flock`
- Sets the close-on-exec flag (`FD_CLOEXEC`) on the file descriptor
- Forks a child process; the child unlocks and closes the handle, then `exec`s `sleep 30`
- The parent logs progress and exits

This illustrates the best practice of setting `FD_CLOEXEC` to avoid leaking open descriptors (and any associated locks) into a new program image after `exec`.

### Requirements
- Perl 5
- CPAN modules:
  - `Log::Log4perl`
  - `Time::HiRes`

Dependencies are declared in `cpanfile`. A `cpanfile.snapshot` is included for reproducible installs (Carton).

### Installation

Using Carton (recommended if you have it):

```bash
cpanm Carton  # if you don't have Carton
cd perl/file_locking
carton install
```

Using cpanm directly into a local vendor dir:

```bash
cd perl/file_locking
cpanm --local-lib=local --installdeps .
```

### Running

With Carton:

```bash
cd perl/file_locking
carton exec perl main.pl
```

With a locally vendored lib path:

```bash
cd perl/file_locking
PERL5LIB=local/lib/perl5 perl main.pl
# or: perl -Ilocal/lib/perl5 main.pl
```

You should see structured logs (via `Log::Log4perl`) showing file open, lock acquisition, `FD_CLOEXEC` being set, the fork, and then the child `exec`.

### Experiment: with and without descriptor inheritance
- Current behavior sets `FD_CLOEXEC` (prevents inheritance across `exec`).
- To observe inheritance, comment out the call to `disable_inheritance_on_file_handle($lock_fh)` in `main.pl` and rerun. Without `FD_CLOEXEC`, if the descriptor remains open across `exec`, the new program can inherit it and the lock may persist unexpectedly.

Note: The script explicitly unlocks and closes the handle in the child prior to `exec` for clarity; you can experiment by removing those lines to study behavior on your platform.

### Cleanup
```bash
rm -f file.lock
```

### License
This project follows the license in the repository’s `LICENSE`.


