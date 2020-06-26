/************************************************************************
** File: hk_msgdefs.h 
**
** NASA Docket No. GSC-16,127-1, and identified as "Core Flight Software System
** (CFS) Housekeeping Application Version 2” 
**
** Copyright © 2007-2014 United States Government as represented by the
** Administrator of the National Aeronautics and Space Administration. All Rights
** Reserved. 
**
** Licensed under the Apache License, Version 2.0 (the "License"); 
** you may not use this file except in compliance with the License. 
** You may obtain a copy of the License at 
** http://www.apache.org/licenses/LICENSE-2.0 
** 
** Unless required by applicable law or agreed to in writing, software 
** distributed under the License is distributed on an "AS IS" BASIS, 
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
** See the License for the specific language governing permissions and 
** limitations under the License. 
**
** Purpose: 
**  The CFS Housekeeping (HK) Application header file
**
** Notes:
**
**
*************************************************************************/
#ifndef _hk_msgdefs_h_
#define _hk_msgdefs_h_


/*************************************************************************
** Includes
**************************************************************************/
#include "cfe.h"


/****************************************
** HK app command packet command codes
****************************************/

/** \hkcmd Housekeeping No-Op
**
**  \par Description
**       This command will increment the command execution counter and send an
**       event containing the version number of the application
**
**  \hkcmdmnemonic \HK_NOOP
**
**  \par Command Structure
**       #CFE_SB_CmdHdr_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with the
**       following telemetry:
**       - \b \c \HK_CMDPC - command execution counter will
**         increment
**       - The #HK_NOOP_CMD_EID informational event message will
**         be generated
**
**  \par Error Conditions
**       There are no error conditions for this command. If the Housekeeping
**       app receives the command, the event is sent (although it
**       may be filtered by EVS) and the counter is incremented
**       unconditionally.
**
**  \par Criticality
**       None
**
**  \sa
*/
#define HK_NOOP_CC                          0


/** \hkcmd Housekeeping Reset Counters
**
**  \par Description
**       This command resets the following counters within the HK
**        housekeeping telemetry:
**       - Command Execution Counter (\HK_CMDPC)
**       - Command Error Counter (\HK_CMDEC)
**       - Combined Packets Sent Counter (\HK_CMBPKTSSENT)
**       - Missing Data Counter (\HK_MISSDATACTR)
**
**  \hkcmdmnemonic \HK_RESETCTRS
**
**  \par Command Structure
**       #CFE_SB_CmdHdr_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with the
**       following telemetry:
**       - \b \c \HK_CMDPC - command execution counter will be reset
**       - \b \c \HK_CMDEC - command error counter will be reset
**       - \b \c \HK_CMBPKTSSENT - combined packets sent counter will be reset
**       - \b \c \HK_MISSDATACTR - missing data counter will be reset
**       - The #HK_RESET_CNTRS_CMD_EID informational event message will
**         be generated
**
**  \par Error Conditions
**       There are no error conditions for this command. If the Housekeeping
**       App receives the command, the event is sent (although it
**       may be filtered by EVS) and the counter is incremented
**       unconditionally.
**
**  \par Criticality
**       This command is not inherently dangerous.  However, it is
**       possible for ground systems and on-board safing procedures
**       to be designed such that they react to changes in the counter
**       values that are reset by this command.
**
**  \sa
*/
#define HK_RESET_CC                         1

      
#endif /* _hk_msgdefs_h_ */

/************************/
/*  End of File Comment */
/************************/
