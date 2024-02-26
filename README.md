# pennyunpacker
Unpacks all data packs based off of `Pack.db` (converted to plain text).

## Instructions
Compile, copy `files.txt`, `filenames.txt` and the `Pack` folder from game files next to the executable.

Args:
- `--extract`: Extract files without the file header
- `--extractheader`: Extract files with the file header

## Tools
`decompress_gzip.sh` - Run from inside `Pack/` to decompress all gzipped files (PVR texture files).

## TODO
- Finish figuring out hashing so we can correctly match hashes to files
- Implement data file packing

## Thanks
Thanks to:
- [smb123w64gb](https://github.com/smb123w64gb) for providing the initial file list