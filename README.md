**Manzano**
=======================

Manzano is a library to communicate with the Quanterra Q330 family of digitizers, and E300 electronics for STS1 sensors.

green_manzano is the command line interactive application.

* This README file includes instructions for green_manzano compilation and instalation.

* The MAN page (green_manzano.1) includes basic usage.

* The doxyfile (Doxyfile) can be used to generate automatic documentation.

---------------------------------------------------------
Basic instructions to install green_manzano (linux):

The only external depencies are CMake, clang++-3.5.

Google test is needed to run the tests, but is not needed to install green_manzano.

* Install CMake

* Install build tools:
```sh
$ sudo apt-get update
$ sudo apt-get install build-essential
$ sudo apt-get install clang-3.5 # or newer
$ export CXX=/usr/bin/clang++-3.5 # or add to .bashrc
```

* Clone repository (for example at home directory)

* Move to green_manzano directory:
```sh
$ cd ~/manzano/code/green_manzano
```
* Create and move to build directory:
```sh
~/manzano/code/green_manzano$ mkdir build
~/manzano/code/green_manzano$ cd build
```
* Build:
```sh
~/manzano/code/green_manzano/build$ cmake ..
~/manzano/code/green_manzano/build$ make
~/manzano/code/green_manzano/build$ sudo make install
```
* Edit configuration file (see example_config.json):
```sh
$ vim ~/manzano/runtime_config/config.json
```

---------------------------------------------------------
Mac OS:

* Install command line tools (and clang++), run and click install when prompted:
```sh
$ xcode-select --install
```
* Download latest stable version of CMake, and follow these instructions
(CMake part only):

http://tudat.tudelft.nl/projects/tudat/wiki/Install_on_Mac_OS_X


---------------------------------------------------------
DISCLAIMER:

This software is preliminary or provisional and is subject to revision. It is being provided to meet the need for timely best science. The software has not received final approval by the U.S. Geological Survey (USGS). No warranty, expressed or implied, is made by the USGS or the U.S. Government as to the functionality of the software and related material nor shall the fact of release constitute any such warranty. The software is provided on the condition that neither the USGS nor the U.S. Government shall be held liable for any damages resulting from the authorized or unauthorized use of the software.

---------------------------------------------------------
