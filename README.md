<img width="500" src="https://raw.githubusercontent.com/picrin-scheme/picrin/master/etc/picrin-logo-fin01-02.png"></img>
# Picscheme
Picscheme is a direct fork of the [picrin](https://github.com/picrin-scheme/picrin)
project with the initial goal of reviving and maintaining the project.
The fork was created after discussion with picrin's maintrainers to create
a clean break with maintenance and support moving to the new project.

See the [picrin README](README.picrin.md) for more information on picrin

## Build

Just type `make` in the project root directory. You will find an executable binary newly created in the top
level directory.
```
make
```

When you are building picrin on x86\_64 system, PIC\_NAN\_BOXING flag is automatically turned on
(see include/picrin/config.h for detail).

To build the c89 version use `make c89` instead, PIC\_NAN\_BOXING will not be enabled by default.

## Install

`make install` target is provided. By default it installs picrin binary into `/usr/local/bin/`.
```
make install
```
Since picscheme does not use autoconf, if you want to specify the install directory, pass the
custom path to `make` via command line argument.
```
make install prefix=/path/to/dir
```
## Requirements

To build picscheme Scheme from source code, some external libraries and tools are required:

- perl
- regex.h of POSIX.1
- libedit (optional, requires pkg-config support, see contrib/30.readline/nitro.mk)
- gmake or compatible make command

In c89 mode there are a couple of additional dependencies. These are standard in c99 and later.

- getaddrinfo
- posix_memalign

The make command automatically turns on optional libraries if available.
Picscheme is mainly developed on Mac OS X and only tested on OS X, Ubuntu 14.04+ and Arch Linux
as of 1-August-2025.

When you try to run picscheme on other platforms and find issues please raise an issue.

## Libraries
The Makefile supports both static and dynamic, run `make slib` to build the static library
and `make dlib` to build the dynamic .so verion.  Note that the installation of these
is currently manual

To test the static library build tiny-scheme with
```
make clean
make slib
gcc -o tiny-scheme -I lib/include src/tiny-main.c -L. -lpicrin -lm
./tiny-scheme
```

To test the dymamic library build tiny-scheme with
```
make clean
make dlib
ln -s libpicrin.1.0.0 libpicrin.1.0
ln -s libpicrin.1.0.0 libpicrin.1
gcc -o tiny-scheme -I lib/include src/tiny-main.c -L. -lpicrin -lm
LD_LIBRARY_PATH=$PWD ./tiny-scheme
```

Library installation depends on the OS and release. For static libraries copying the libpicrin.a to a
location the linker looks in will suffice. For dynamic libraries libpicrin.so.1.0.0 will need copying
to a directory searched by the loader and ldconfig, at least on linux, run to update the caches and
configure the names

## Authors

See `AUTHORS`

# Lisp for microcontollers
While picscheme is small it does not currently fit on nor compile for microcontrollers.
If you are interested in lisp/scheme for microcontrollers head over
to [ulist](http://www.ulisp.com)


## C89 mode
The project is written in c89 c and compiles cleanly with gcc -std=c89.
However, there a few library references that are not part of the c89
standard:
* lib/gc.c uses posix\_memalign
* srfi-106 requires getaddrinfo (see contrib/40.srfi/src/106.c)

These are generally available in modern OSes. If this is not the case
* For gc.c define 'PIC\_BITMAP\_GC=0' in the Makefile CFLAGS
* Disable srfi-106: This will need source changes in the contrib tree

In general unless a c89 compiler is the only one available a more
recent one should be used.

## Changes

### August 2025
* Tidy up of compiler warnings in non-c89 mode
  * Moved from array of strings to char array in boot.c and load\_piclib.c
  * Tagged case statements with 'fall through' comments where needed
* Fixed c89 compatibility issues
  * Replaced c++ style single line comments with c style ones
  * Added maths functions missing from c89
  * Handled eqv? of NAN in c89 non-boxed nan mode as NAN != NAN
* Support for static and dynamic versions of libpicrin added
