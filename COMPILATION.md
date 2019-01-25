# Compilation instructions for developers

Please see file INSTALLATION.md for installing pre-compiled binary packages on
Windows, macOS, and Linux.  It is not necessary to compile jack_playrec yourself unless
you want to make changes to jack_playrec itself.

This guide describes how to compile jack_playrec from sources for developers.

## I. Compiling from source on Linux

### Prerequisites
64-bit version of Ubuntu 18.04 or later,
or a Beaglebone Black running Debian Stretch.

... with the following software packages installed:
- g++
- make
- libsndfile1-dev
- libjack-jackd2-dev
- jackd2

### Compilation

Clone jack_playrec from github, compile jack_playrec by typing in a terminal
```
git clone https://github.com/HoerTech-gGmbH/jack_playrec
cd jack_playrec
./configure && make
```

### Installation of self-compiled jack_playrec:

A very simple installation routine is provided together with the
source code.  To collect the relevant binaries and libraries execute
```
make install
```

You can set the make variable PREFIX to point to the desired installation
location. The default installation location is ".", the current directory.

## II. Compiling from source on macOS

### Prerequisites
- macOS 10.10 or later.
- XCode 7.2 or later (available from App Store)
- Jack2 for OS X http://jackaudio.org (also available from MacPorts)
- MacPorts https://www.macports.org

The following packages should be installed via MacPorts:
- libsndfile
- pkgconfig

### Compilation

Clone jack_playrec from github, compile jack_playrec by typing in a terminal
```
git clone https://github.com/HoerTech-gGmbH/jack_playrec
cd jack_playrec
./configure && make
```

### Installation of self-compiled jack_playrec:

A very simple installation routine is provided together with the
source code.  To collect the relevant binaries and libraries execute
```
make install
```

You can set the make variable PREFIX to point to the desired installation
location.  The default installation location is ".", the current directory.

## III. Compilation on 64-bit Windows (advanced)

### Prerequisites

- msys2 installation with MinGW64 C++ compiler
- Jack Audio Connection Kit (Use the 64-bit installer for windows) (http://jackaudio.org)

### Preparation

- With the msys2 package manager pacman, install the following packages:
mingw-w64-x86_64-libsndfile and git.
- Copy the contents of the includes folder in the JACK directory into your mingw
include directory (default is c:\msys64\mingw64\include).  There should now be a
directory c:\msys64\mingw64\include\jack containing some files.
- Copy libjack64.lib from the JACK installation to the lib directory of your mingw64
directory and rename it to libjack.a afterwards.  Windows may warn that the
file may become unusable -- ignore this warning.

### Compilation

Start a mingw64 bash shell from the Windows start menu.
Clone jack_playrec from github, compile jack_playrec by typing in a terminal
```
git clone https://github.com/HoerTech-gGmbH/jack_playrec
cd jack_playrec
./configure && make
```
