Manzano
=======================

Manzano is a library to communicate with the Quanterra Q330 family of digitizers, and E300 electronics for STS1 sensors.

green\_manzano is the command line interactive application.

* This README file includes instructions for green\_manzano compilation and instalation.

* The MAN page (green\_manzano.1) includes basic usage.

* The doxyfile (Doxyfile) can be used to generate automatic documentation.

---------------------------------------------------------
Basic instructions to install green\_manzano (linux):

The only external depencies are CMake, clang++-3.5.

Google test is needed to run the tests, but is not needed to install green\_manzano.

* Install CMake

* Run this:

$ sudo apt-get update

$ sudo apt-get install build-essential

$ sudo apt-get install clang-3.5 (or newer)

$ export CXX=/usr/bin/clang++-3.5

* Clone repository (for example at home directory), then:

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
