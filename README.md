# Housekeeping
NASA core Flight System Housekeeping Application

## Description
The Housekeeping application (HK) is a core Flight System (cFS) application that is a plug in to the Core Flight Executive (cFE) component of the cFS.

The cFS is a platform and project independent reusable software framework and set of reusable applications developed by NASA Goddard Space Flight Center. This framework is used as the basis for the flight software for satellite data systems and instruments, but can be used on other embedded systems. More information on the cFS can be found at [http://cfs.gsfc.nasa.gov](http://cfs.gsfc.nasa.gov)

The HK application is used for building and sending combined telemetry messages (from individual system applications) to the software bus for routing. Combining messages is performed in order to minimize downlink telemetry bandwidth. Combined messages are also useful for organizing certain types of data packets together. HK provides the capability to generate multiple combined packets so that data can be sent at different rates.
