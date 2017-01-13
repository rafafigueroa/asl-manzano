**Manzano**
=======================

### Manzano is a library to communicate with hardware used in seismological stations. Currently:

 + [Q330 Digitizer](http://www.kinemetrics.com/uploads/PDFs/q330.pdf)
 + [E300 STS1 sensor electronics](http://www.metrozet.com/legacy/STS1-E300_Manual_V1_20.pdf)
 + [Falcon facilities monitoring system](http://rletech.com/our-products/facility-monitoring-systems/server-room-facility-monitoring/fms/)
 
### Key components:
 + Custom serialization library, that allows for unusual data types such as a 24 bit int.
 + Custom auto-generation process for bitmap data structures with a common internal representation.
 + Udp, Tcp and Serial connections and message handling. Serial connections uses William Woodall [C++ serial library](https://github.com/wjwwood/serial)

----------------------------------------------------------------------------
* Read the [Manzano Wiki](https://github.com/usgs/asl-manzano/wiki) for details.
* The MAN page (green_manzano.1) includes basic usage.
* The doxyfile (Doxyfile) can be used to generate automatic documentation.

## Manzano Applications

----------------------------------------------------------------------------
### Green Manzano 
Command line tool to use Manzano either interactively or through a script.

----------------------------------------------------------------------------
### Yellow Manzano 
Interacts with an [E300](http://www.metrozet.com/legacy/STS1-E300_Manual_V1_20.pdf) electronics through an udp server. Intended to run on a station's data_processor or another linux device connected serially with the E300.



----------------------------------------------------------------------------
### Author / Maintainer
Rafael Figueroa (rfigueroa@usgs.gov)

---------------------------------------------------------
DISCLAIMER:

This software is preliminary or provisional and is subject to revision. It is being provided to meet the need for timely best science. The software has not received final approval by the U.S. Geological Survey (USGS). No warranty, expressed or implied, is made by the USGS or the U.S. Government as to the functionality of the software and related material nor shall the fact of release constitute any such warranty. The software is provided on the condition that neither the USGS nor the U.S. Government shall be held liable for any damages resulting from the authorized or unauthorized use of the software.

---------------------------------------------------------
