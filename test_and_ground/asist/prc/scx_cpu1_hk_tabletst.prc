PROC scx_cpu1_hk_tabletst
;*******************************************************************************
;  Test Name:  hk_copytable3
;  Test Level: Build Verification
;  Test Type:  Functional
;
;  Test Description
;	The purpose of this procedure is to generate the files for the copy
;       tables used during BVT.  Note that the message ids used are
;       borrowed from the other CFS applications (MM, FM, MD, and SCH). 
;
;  Table 3:   ;      2 input messages, 6 output messages (Table will have 128 entries);      Used for StressHousekeeping, StressMissingData, StressTableLoad;
;  Requirements Tested
;       None
;
;  Prerequisite Conditions
;	The cFE is up and running and ready to accept commands. ;	The HK commands and TLM items exist in the GSE database. 
;	A display page exists for the HK Housekeeping telemetry packet. ;	HK Test application loaded and running;
;  Assumptions and Constraints
;	None.
;
;  Change History
;
;	Date		   Name		Description
;	5/19/08 	Barbie Medina	Original Procedure.
;
;  Arguments
;	None.
;
;  Procedures Called
;	Name			 Description
;       create_tbl_file_from_cvt Procedure that creates a load file from
;                                the specified arguments and cvt
;                                
;
;  Expected Test Results and Analysis
;
;**********************************************************************

#include "ut_statusdefs.h"
#include "ut_cfe_info.h"
#include "cfe_platform_cfg.h"
#include "cfe_evs_events.h"
#include "cfe_es_events.h"
#include "to_lab_events.h"
#include "hk_platform_cfg.h"
#include "hk_events.h"
#include "tst_hk_events.h"

write ";*********************************************************************"
write ";  define local variables "
write ";*********************************************************************"

LOCAL entry
LOCAL appid
LOCAL OutputPacket1
LOCAL OutputPacket2
LOCAl OutputPacket3
LOCAl OutputPacket4
LOCAl OutputPacket5
LOCAl OutputPacket6
LOCAL InputPacket1
LOCAL InputPacket2

;; CPU1 is the default
appid = 0xfa6
OutputPacket1 = 0x89c  
OutputPacket2 = 0x89d
OutputPacket3 = 0x89e
OutputPacket4 = 0x89f
OutputPacket5 = 0x8a0
OutputPacket6 = 0x8a1
InputPacket1 = 0x887
InputPacket2 = 0x89a

if ("CPU1" = "CPU2") then
   appid = 0xfc4
   OutputPacket1 = 0x99c  
   OutputPacket2 = 0x99d
   OutputPacket3 = 0x99e
   OutputPacket4 = 0x99f
   OutputPacket5 = 0x9a0
   OutputPacket6 = 0x9a1
   InputPacket1 = 0x987
   InputPacket2 = 0x99a
elseif ("CPU1" = "CPU3") then
   appid = 0xfe4
   OutputPacket1 = 0xa9c  
   OutputPacket2 = 0xa9d
   OutputPacket3 = 0xa9e
   OutputPacket4 = 0xa9f
   OutputPacket5 = 0xaa0
   OutputPacket6 = 0x8a1
   InputPacket1 = 0xa87
   InputPacket2 = 0xa9a
endif 


write ";*********************************************************************"
write ";  Step 1.0:  Define Copy Table 3. "
write ";*********************************************************************"


; Entry 1
entry = 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket1
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 12
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 12
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 2
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket2
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 12
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 13
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 3
entry = entry +1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket3
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 12
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 14
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 4
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket4
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 12
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 15
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 5
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket5
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 12
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 16
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 6
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket6
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 12
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 17
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 7
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket1
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 14
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 18
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 8
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket2
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 14
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 19
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 9
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket3
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 14
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 20
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 10
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket4
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 14
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 21
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 11
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket5
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 14
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 22
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 12
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket6
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 14
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 23
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 13
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket1
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 16
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 24
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 14
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket2
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 16
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 25
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 15
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket3
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 16
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 26
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 16
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket4
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 16
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 27
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 17
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket5
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 16
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 28
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 18
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket6
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 18
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 29
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 19
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket1
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 18
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 30
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 20
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket2 
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 18
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 31
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 21
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket3
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 18
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 32
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 22
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket4
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 18
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 33
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 23
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket5
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 18
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 34
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 24
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket6
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 18
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 35
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 25
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket1
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 20
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 36
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 26
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket2
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 20
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 37
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 27
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket3
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 20
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 38
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 28
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket4
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 20
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 39
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 29
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket5
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 20
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 40
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 30
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket6
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 20
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 41
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 31
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket1
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 22
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 42
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 32
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket2
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 22
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 43
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 33
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket3
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 22
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 12
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 34
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket4
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 22
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 13
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 35
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket5
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 22
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 14
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 36
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket6
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 22
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 15
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 37
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket1
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 24
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 16
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 38
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket2
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 24
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 17
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 39
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket3
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 24
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 18
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 40
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket4
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 24
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 19
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 41
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket5
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 24
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 20
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 42
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket6
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 24
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 21
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 43
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket1
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 26
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 22
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 44
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket2
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 26
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 23
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 45
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket3
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 26
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 24
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 46
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket4
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 26
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 25
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 47
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket5
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 26
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 26
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 48
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket6
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 26
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 27
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 49
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket1
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 28
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 28
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 50
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket2
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 28
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 29
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 51
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket3
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 28
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 30
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 52
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket4
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 28
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 31
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 53
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket5
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 28
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 32
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 54
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket6
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 28
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 33
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 55
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket1
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 30
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 34
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 56
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket2
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 30
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 35
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 57
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket3
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 30
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 36
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 58
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket4
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 30
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 37
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 59
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket5
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 30
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 38
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 60
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket6
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 30
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 39
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 61
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket1
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 32
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 40
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 62
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket2
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 32
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 41
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 63
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket3
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 32
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 42
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 64
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket4
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 32
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket1
SCX_CPU1_HK_CopyTable[entry].InputOffset = 43
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1


; Entry 65
entry = 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket1
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 13
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 43
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 66
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket2
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 13
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 42
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 67
entry = entry +1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket3
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 13
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 41
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 68
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket4
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 13
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 40
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 69
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket5
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 13
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 39
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 70
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket6
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 13
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 38
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 71
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket1
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 15
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 37
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 72
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket2
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 15
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 36
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 73
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket3
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 15
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 35
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 74
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket4
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 15
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 34
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 75
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket5
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 15
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 33
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 76
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket6
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 15
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 32
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 77
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket1
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 17
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 31
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 78
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket2
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 17
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 30
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 79
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket3
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 17
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 29
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 80
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket4
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 17
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 28
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 81
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket5
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 17
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 27
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 82
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket6
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 17
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 26
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 83
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket1
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 19
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 25
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 84
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket2 
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 19
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 24
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 85
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket3
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 19
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 23
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 86
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket4
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 19
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 22
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 87
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket5
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 19
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 21
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 88
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket6
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 19
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 20
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 89
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket1
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 21
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 19
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 90
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket2
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 21
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 18
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 91
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket3
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 21
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 17
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 92
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket4
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 21
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 16
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 93
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket5
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 21
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 15
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 94
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket6
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 21
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 14
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 95
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket1
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 23
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 13
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 96
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket2
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 23
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 12
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 97
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket3
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 23
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 43
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 98
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket4
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 23
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 42
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 99
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket5
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 23
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 41
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

; Entry 100
entry = entry + 1
SCX_CPU1_HK_CopyTable[entry].OutputMid = OutputPacket6
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 23
SCX_CPU1_HK_CopyTable[entry].InputMid = InputPacket2
SCX_CPU1_HK_CopyTable[entry].InputOffset = 40
SCX_CPU1_HK_CopyTable[entry].NumBytes = 1

for entry = 101 to 128 do
SCX_CPU1_HK_CopyTable[entry].OutputMid = 0
SCX_CPU1_HK_CopyTable[entry].OutputOffset = 0
SCX_CPU1_HK_CopyTable[entry].InputMid = 0
SCX_CPU1_HK_CopyTable[entry].InputOffset = 0 
SCX_CPU1_HK_CopyTable[entry].NumBytes = 0
enddo


 
s create_tbl_file_from_cvt("CPU1", appid, "Copy Table 3", "hk_cpy_tbl.tbl", "HK.CopyTable", "SCX_CPU1_HK_CopyTable[1]", "SCX_CPU1_HK_CopyTable[128]")


write ";*********************************************************************"
write ";  End procedure SCX_CPU1_hk_copytable3                               "
write ";*********************************************************************"
ENDPROC
