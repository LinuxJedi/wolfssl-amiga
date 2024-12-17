# Amiga wolfSSL port

This is a port of wolfSSL designed to run on a 68030 or higher Amiga. With some
minor tweaking this should work on lower CPUs as well.

## Requirements

You need [Bebbo's GCC](https://github.com/bebbo/amiga-gcc) compiled and installed.

## Compiling

First of all run `git submodule update --init --recursive` to get the wolfSSL source code.
Then make sure `m68k-amigaos-gcc` is in your path, then run `make`.
