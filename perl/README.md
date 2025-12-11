## Perl projects (Carton-based)

This directory contains small Perl projects. Each subdirectory (for example, `file_locking`) is an independent project that can be built and run using [Carton](https://metacpan.org/pod/Carton).

### Build and run

1) Ensure Carton is installed (one-time):

```bash
cpanm Carton
```

2) Move into a project directory (example: `file_locking`) and install dependencies:

```bash
cd perl/file_locking
carton install
```

3) Run the main script with the project's dependencies:

```bash
carton exec perl main.pl
```

Notes:
- Each project should include a `cpanfile` and/or `cpanfile.snapshot` that Carton uses to resolve and lock dependencies.
- `carton install` will create a `local/` directory in the project folder containing the installed modules.

