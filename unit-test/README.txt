##############################################################################
## File: README.txt
##
## Purpose: CFS HK application unit test instructions, results, and coverage
##############################################################################

-------------------------
HK Unit Test Instructions
-------------------------
This unit test was run in a virtual machine running Ubuntu 18.04 and uses the
ut-assert stubs and default hooks for the cFE, OSAL and PSP.

To run the unit test enter the following commands at the command line prompt in
the top-level cFS directory (after ensuring that HK is listed as a target). Note
that in order to successfully compile the unit tests the "-Werror" compilation
flag must be disabled.

make distclean
make SIMULATION=native ENABLE_UNIT_TESTS=true prep
make
make test
make lcov

HK 2.5.0 Unit Test Results:

Tests Executed:    87
Assert Pass Count: 441
Assert Fail Count: 0

==========================================================================
hk_app.c - Line Coverage:     100.0%
           Function Coverage: 100.0%
           Branch Coverage:   100.0%

==========================================================================
hk_utils.c - Line Coverage:     100.0%
             Function Coverage: 100.0%
             Branch Coverage:   100.0%

==========================================================================
