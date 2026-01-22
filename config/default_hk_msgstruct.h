/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
 *  The CFS Housekeeping (HK) Application Message Structure Definition
 *
 * Provides default definitions for message structures
 *
 * @note This file may be overridden/superceded by mission-provided defintions
 * either by overriding this header or by generating definitions from a command/data
 * dictionary tool.
 */
#ifndef DEFAULT_HK_MSGSTRUCT_H
#define DEFAULT_HK_MSGSTRUCT_H

#include "cfe_es_extern_typedefs.h"
#include "cfe_sb_extern_typedefs.h"
#include "cfe_msg_hdr.h"

/**
 * \defgroup cfshkcmdstructs CFS Housekeeping Command Structures
 * \{
 */

/**
 *  \brief Send Combined Output Payload
 *
 *  This structure contains the format of the command used to inform HK to send
 *  the specified combined output message
 */
typedef struct
{
    CFE_SB_MsgId_t OutMsgToSend; /**< \brief MsgId of combined tlm pkt to send  */
} HK_SendCombinedPkt_Payload_t;

/**
 *  \brief Send Combined Output Message Command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command Message Header */

    HK_SendCombinedPkt_Payload_t Payload;
} HK_SendCombinedPktCmd_t;

/**
 *  \brief Send HK command packet structure
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command Message Header */
} HK_SendHkCmd_t;

/**
 *  \brief No-Operation command packet structure
 *
 *  For command details see #HK_NOOP_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command Message Header */
} HK_NoopCmd_t;

/**
 *  \brief No-Operation command packet structure
 *
 *  For command details see #HK_RESET_COUNTERS_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command Message Header */
} HK_ResetCountersCmd_t;

/**\}*/

/**
 * \defgroup cfshktlm CFS Housekeeping Telemetry
 * \{
 */

/**
 *  \brief HK Application housekeeping Packet
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t TelemetryHeader; /**< \brief Telemetry Message Header */

    HK_HkTlm_Payload_t Payload;
} HK_HkPacket_t;

/**\}*/

#endif
