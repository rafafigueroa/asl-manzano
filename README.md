**Manzano**
=======================

Manzano is a library to communicate with the Quanterra Q330 family of digitizers, and E300 electronics for STS1 sensors.

**green_manzano** is the command line interactive application.

* This README file includes instructions for green_manzano compilation and instalation.
* The MAN page (green_manzano.1) includes basic usage.
* The doxyfile (Doxyfile) can be used to generate automatic documentation.

**External Dependencies:**
 * CMake
 * C++14 compiler (clang++-3.5)
 * Linux or Mac OS X
 * Google test (not required for green_manzano installation)

---------------------------------------------------------
## Install dependencies (Linux)

* Install latest CMake (tested with 3.4.1)

* Install build tools:
```sh
$ sudo apt-get update
$ sudo apt-get install build-essential
$ sudo apt-get install clang-3.5 # or newer
$ export CXX=/usr/bin/clang++-3.5 # or add to .bashrc
```

---------------------------------------------------------
## Install dependencies (Mac OS X):

*  Install latest CMake (tested with 3.4.1)

> **Mac OS X CMake install steps:**
> http://tudat.tudelft.nl/projects/tudat/wiki/Install_on_Mac_OS_X

* Install command line tools, click *install* when prompted:
```sh
$ xcode-select --install
```

---------------------------------------------------------
## Install green_manzano

* Clone manzano repository (for example at home directory)

* Move to green_manzano directory:
```sh
$ cd ~/manzano/code/apps/green_manzano
```
* Create and move to build directory:
```sh
~/manzano/code/apps/green_manzano$ mkdir build
~/manzano/code/apps/green_manzano$ cd build
```
* Make/Install:
```sh
~/manzano/code/apps/green_manzano/build$ cmake ..
~/manzano/code/apps/green_manzano/build$ make
~/manzano/code/apps/green_manzano/build$ sudo make install
```
> green_manzano does not require admin priviliges, only the installation. The last step can be skipped, simply run the binary from the build directory.

* Edit configuration file (see example_config.json):
```sh
$ vim ~/manzano/runtime_config/config.json
```

---------------------------------------------------------
DISCLAIMER:

This software is preliminary or provisional and is subject to revision. It is being provided to meet the need for timely best science. The software has not received final approval by the U.S. Geological Survey (USGS). No warranty, expressed or implied, is made by the USGS or the U.S. Government as to the functionality of the software and related material nor shall the fact of release constitute any such warranty. The software is provided on the condition that neither the USGS nor the U.S. Government shall be held liable for any damages resulting from the authorized or unauthorized use of the software.

---------------------------------------------------------
