<img width="500" src="https://raw.githubusercontent.com/picscheme/picscheme/master/etc/picrin-logo-fin01-02.png"></img>
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
- compiler support for 64bit when compiling for a 32bit system

In c89 mode there are a couple of additional dependencies. These are standard in c99 and later.

- getaddrinfo
- posix\_memalign
- round and trunc 

The make command automatically turns on optional libraries if available.
Picscheme is mainly developed on Mac OS X and only tested on OS X, Ubuntu 14.04+ and Arch Linux
from the 1-August-2025.

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
ln -s libpicrin.so.1.0.0 libpicrin.so.1.0
ln -s libpicrin.so.1.0.0 libpicrin.so.1
gcc -o tiny-scheme -I lib/include src/tiny-main.c -L. -lpicrin -lm
LD_LIBRARY_PATH=$PWD ./tiny-scheme
```

Library installation depends on the OS and release. For static libraries copying the libpicrin.a to a
location the linker looks in will suffice. For dynamic libraries libpicrin.so.1.0.0 will need copying
to a directory searched by the loader and ldconfig, at least on linux, run to update the caches and
configure the names

## Authors

See `AUTHORS`

# Additonal Notes
## C89 mode
The project is written in c89 c and compiles cleanly with gcc -std=c89.
However, there a few library references that are not part of the c89
standard:
* lib/gc.c uses posix\_memalign
* srfi-106 requires getaddrinfo (see contrib/40.srfi/src/106.c)
* contrib/10.math/math.c requires round and trunc 

These are generally available in modern OSes. If this is not the case
* For gc.c define 'PIC\_BITMAP\_GC=0' in the Makefile CFLAGS
* Disable srfi-106: This will need source changes in the contrib tree
* For round and trunc update contrib/10.math/math.c to remove dependencies

In general unless a c89 compiler is the only one available a more
recent one should be used.

## 32 Bit builds
The original picrin had some dependencies on 64bits and sometimes assumed
long was larger than int, these are being identified and resolved. 

### Building 32 bit binaries on 64 bit machines
Some OSes, especially Arch Linux, are missing 32 bit versions of libedit on 64bit machines.
To resolve on systems that use pkg-config, download libedit from [thrysoee.dk](https://thrysoee.dk/editline/),
then,
* unpack the tarball libedit-VERSION.tar.gz
* cd libedit-VERSION
```
INSTALL_DIR=PATH-TO-INSTALL-DIR
./configure --prefix=$INSTALL_DIR --disable-shared CFLAGS="-m32"
make
make install
```
Then in the picrin directory
```
export PKG_CONFIG_PATH=$INSTALL_DIR/lib/pkgconfig
make m32
```

## Lisp for microcontollers
While picscheme is small it does not currently fit on nor compile for microcontrollers.
If you are interested in lisp/scheme for microcontrollers head over
to [ulist](http://www.ulisp.com)

## Changes

### September 2025
* string->number and related functions assumed long was larger than int,
  which is not always the case on 32bit systems. Used int64 instead
* Overflow of strtoll handled via errno to improve reliability
* In setup.h include stdint.h in all cases for int64_t and uint64_t types
  * int64_t and uint64_t now unconditionally defined in stdint.h
* When !PIC_USE_LIBC
  * add strtoll function
  * handle overflow in existing strtol and new strtoll setting value to INT/LONG_MAX and setting errno
* Removed c89trunc and c89round as they had limited range
  * Now use the system libraries
* Export display from scheme base, write exported in previous update

### August 2025
* Tidy up of compiler warnings in non-c89 mode
  * Moved from array of strings to char array in boot.c and load\_piclib.c
  * Tagged case statements with 'fall through' comments where needed
* Fixed c89 compatibility issues
  * Replaced c++ style single line comments with c style ones
  * Added maths functions missing from c89
  * Handled eqv? of NAN in c89 non-boxed nan mode as NAN != NAN
* Support for static and dynamic versions of libpicrin added
