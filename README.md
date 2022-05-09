# core Flight System (cFS) Housekeeping Application (HK)

## Introduction

The Housekeeping application (HK) is a core Flight System (cFS) application
that is a plug in to the Core Flight Executive (cFE) component of the cFS.

The HK application is used for building and sending combined telemetry messages
(from individual system applications) to the software bus for routing. Combining
messages is performed in order to minimize downlink telemetry bandwidth.
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

An integrated bundle including the cFE, OSAL, and PSP can
be obtained at https://github.com/nasa/cfs

## About cFS

The cFS is a platform and project independent reusable software framework and
set of reusable applications developed by NASA Goddard Space Flight Center.
This framework is used as the basis for the flight software for satellite data
systems and instruments, but can be used on other embedded systems.  More
information on the cFS can be found at http://cfs.gsfc.nasa.gov
