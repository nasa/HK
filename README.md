# core Flight System (cFS) Housekeeping Application (HK)

## Introduction

The Housekeeping application (HK) is a core Flight System (cFS) application
that is a plug in to the Core Flight Executive (cFE) component of the cFS.

The HK application is used for building and sending combined telemetry messages
(from individual system applications) to the software bus for routing. Combining messages is performed to minimize downlink telemetry bandwidth.
Combining certain data from multiple messages into one message eliminates the
message headers that would be required if each message was sent individually.
Combined messages are also useful for organizing certain types of data. This
application may be used for data types other than housekeeping telemetry. HK
provides the capability to generate multiple combined packets (a.k.a. output
packets) so that data can be sent at different rates (e.g. a fast, medium and
slow packet).

The HK application is written in C and depends on the cFS Operating System
Abstraction Layer (OSAL) and cFE components.  There is additional HK application
specific configuration information contained in the application user's guide.
  
User's guide information can be generated using Doxygen (from top mission directory):
```
  make prep
  make -C build/docs/hk-usersguide hk-usersguide
```
 
## Software Required

cFS Framework (cFE, OSAL, PSP)

A demonstration bundle of the Core Flight System including the cFE, OSAL, and PSP can be obtained at https://github.com/nasa/cfs

For information about a mission ready cFS bundle, see: https://github.com/nasa/cFS#cfs-gov-mission-ready-version

## Known issues

See all [open issues](https://github.com/nasa/CS/issues) and closed to milestones later than this version.

## Getting Help

For best results, submit issues:questions or issues:help wanted requests at <https://github.com/nasa/cFS>.

Official cFS page: <http://cfs.gsfc.nasa.gov>
