**Manzano**
=======================

### Manzano is a library to communicate with hardware used in seismological stations. Currently:

 + [Q330 Digitizer](http://www.kinemetrics.com/uploads/PDFs/q330.pdf)
 + [E300 STS1 sensor electronics](http://www.metrozet.com/legacy/STS1-E300_Manual_V1_20.pdf)
 + [Falcon facilities monitoring system](http://rletech.com/our-products/facility-monitoring-systems/server-room-facility-monitoring/fms/)

----------------------------------------------------------------------------
* The [Manzano Wiki](https://github.com/usgs/asl-manzano/wiki) includes installation, usage and developer details.
* The MAN page (green_manzano.1) includes basic green manzano usage.
* The doxyfile (Doxyfile) can be used to generate code automatic documentation.

----------------------------------------------------------------------------
### Key components:
 + Custom serialization library, that allows for unusual data types such as a 24 bit integers and others.
 + Custom auto-generation process for bitmap data structures with a common internal representation.
 + Udp, Tcp and Serial connections. Serial connections uses William Woodall [C++ serial library](https://github.com/wjwwood/serial)
 + Common addressing mechanism for a network of different devices (inclucing multiple stations).
 + Common instruction set for easy use of devices. For example, for the Q330 digitizer:
 
 ```
 DEV:st[0]q[0] » show command


 show
 ├── target
 ├── command
 ├── config
 ├── plan
 └── status

 edit
 ├── target
 └── plan

 get
 ├── poll
 ├── qview
 ├── ctrl
 ├── center
 ├── global
 ├── ping
 ├── dev
 └── stat [:] [:boom] [:gps] [:gpssat] [:power] [:dataport] [:pll]

 set
 ├── ctrl [:save] [:reboot] [:resync] [:gps_on] [:gps_off] [:gps_cold_start]
 ├── reg
 └── dereg

 stop
 └── cal
```

----------------------------------------------------------------------------
## Manzano Applications

### Green Manzano 
Command line tool to use Manzano either interactively or with a script.

### Yellow Manzano 
Interacts with an [E300](http://www.metrozet.com/legacy/STS1-E300_Manual_V1_20.pdf). Intended to run on a station's data_processor or another linux device connected serially with the E300, links the E300 with an udp server for remote interaction.

### Red Manzano (under development)
GUI (Qt) tool to use Manzano interactively.

![Red Manzano beta](https://raw.githubusercontent.com/wiki/usgs/asl-manzano/red_mzn_close.png)

### Falcon
Application to read data from a falcon web service, extract contiguous packets of information and serialize/send packets to external servers.

----------------------------------------------------------------------------
### Author / Maintainer
Rafael Figueroa (rfigueroa@usgs.gov)

---------------------------------------------------------
DISCLAIMER:

This software is preliminary or provisional and is subject to revision. It is being provided to meet the need for timely best science. The software has not received final approval by the U.S. Geological Survey (USGS). No warranty, expressed or implied, is made by the USGS or the U.S. Government as to the functionality of the software and related material nor shall the fact of release constitute any such warranty. The software is provided on the condition that neither the USGS nor the U.S. Government shall be held liable for any damages resulting from the authorized or unauthorized use of the software.

---------------------------------------------------------
