# NESPAK

A collection of NES compression tools.

## Usage

```bash
# compress .txt file into .bin file using RLE compression
ruby nespak.rb -p pack -a rle -i examples/example.txt -o tmp/out.bin

# decompress .bin file back into .txt file using RLE decompression
ruby nespak.rb -p unpack -a rle -i examples/example.bin -o tmp/out.txt
```
