/************************************************************************
 * NASA Docket No. GSC-18,919-1, and identified as “Core Flight
 * System (cFS) Housekeeping (HK) Application version 2.5.1”
 *
 * Copyright (c) 2021 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * @file
 *  The CFS Housekeeping (HK) Application header file
 */
#ifndef HK_MSGDEFS_H
#define HK_MSGDEFS_H

#include <cfe.h>

/**
 * \defgroup cfshkcmdcodes CFS Housekeeping Command Codes
 * \{
 */

/**
 * \brief Housekeeping No-Op
 *
 *  \par Description
 *       This command will increment the command execution counter and send an
 *       event containing the version number of the application
 *
 *  \par Command Structure
 *       #CFE_MSG_CommandHeader_t
 *
 *  \par Command Verification
 *       Successful execution of this command may be verified with the
 *       following telemetry:
 *       - #HK_HkTlm_Payload_t.CmdCounter will increment
 *       - The #HK_NOOP_INF_EID informational event message will be generated
 *
 *  \par Error Conditions
 *       There are no error conditions for this command. If the Housekeeping
 *       app receives the command, the event is sent (although it
 *       may be filtered by EVS) and the counter is incremented
 *       unconditionally.
 *
 *  \par Criticality
 *       None
 */
#define HK_NOOP_CC 0

/**
 * \brief Housekeeping Reset Counters
 *
 *  \par Description
 *       This command resets the counters within the HK housekeeping telemetry
 *
 *  \par Command Structure
 *       #CFE_MSG_CommandHeader_t
 *
 *  \par Command Verification
 *       Successful execution of this command may be verified with the
 *       following telemetry:
 *       - #HK_HkTlm_Payload_t.CmdCounter will be reset
 *       - The #HK_RESET_INF_EID informational event message will
 *         be generated
 *
 *  \par Error Conditions
 *       There are no error conditions for this command. If the Housekeeping
 *       App receives the command, the event is sent (although it
 *       may be filtered by EVS) and the counter is incremented
 *       unconditionally.
 *
 *  \par Criticality
 *       This command is not inherently dangerous.  However, it is
 *       possible for ground systems and on-board safing procedures
 *       to be designed such that they react to changes in the counter
 *       values that are reset by this command.
 */
#define HK_RESET_COUNTERS_CC 1

/**\}*/

#endif
