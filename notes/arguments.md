# Command Line Arguments for bad_zip (b_zip)

To compress:
`Usage: ./build/debug/bad_zip [options] [archive_name] [file1] [file2] ... [directory1] ...`

To decompress:
`Usage: ./build/debug/bad_zip [options] [archive_name] [target_directory_name]`

Available options:

`-q`: quiet, does not print output messages
`-h`: help, prints the help messages
`-c`: compress
`-d`: decompress

`-d` and `-c` are mutually exclusive, only one may be present

Behavior:

- The compressed file name does not need to include the `.bzip` at the end, since that will automatically be appended
- There is no recursive flag, since the tool assumes all directories are to be processed recursively

Future note: maybe support the `-cq` for multiple flags