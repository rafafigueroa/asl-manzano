Manzano 
=======================

This software communicates with the Quanterra Q330 family of digitizers. It performs functionaly similar to the open source Lib330 library.

Basic instructions (linux):

* Install CMake

* Run this:

$ sudo apt-get update

$ sudo apt-get install build-essential

$ sudo apt-get install clang-3.5 (or newer)

$ export CXX=/usr/bin/clang++-3.5

* Clone repository (for example at home directory), then:

~/manzano$ git submodule init

~/manzano$ git submodule update

* Move to green\_manzano directory:

$ cd ~/manzano/code/green\_manzano

* Create and move to build directory:

~/manzano/code/green\_manzano$ mkdir build

~/manzano/code/green\_manzano$ cd build

* Build:

~/manzano/code/green\_manzano/build$ cmake ..

~/manzano/code/green\_manzano/build$ make

~/manzano/code/green\_manzano/build$ sudo make install

* Edit configuration file:

$ vim ~/manzano/runtime\_config/config.json


---------------------------------------------------------
Mac OS:

to install command line tools (and clang++) and then click install:
xcode-select --install

Download latest stable version of CMake, and follow these instructions
(CMake part only):

http://tudat.tudelft.nl/projects/tudat/wiki/Install\_on\_Mac\_OS\_X


---------------------------------------------------------
DISCLAIMER:

This software is preliminary or provisional and is subject to revision. It is being provided to meet the need for timely best science. The software has not received final approval by the U.S. Geological Survey (USGS). No warranty, expressed or implied, is made by the USGS or the U.S. Government as to the functionality of the software and related material nor shall the fact of release constitute any such warranty. The software is provided on the condition that neither the USGS nor the U.S. Government shall be held liable for any damages resulting from the authorized or unauthorized use of the software.
