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
 *  The CFS Housekeeping (HK) Application Message IDs header file
 */
#ifndef HK_TOPICIDS_H
#define HK_TOPICIDS_H

#include "hk_topicid_values.h"

/**
 * \defgroup cfshkcmdtid CFS Housekeeping Command Topic IDs
 * \{
 */

#define CFE_MISSION_HK_CMD_TOPICID             CFE_MISSION_HK_TIDVAL(CMD)
#define DEFAULT_CFE_MISSION_HK_CMD_TOPICID     0x9A /**< \brief HK Ground Commands Message ID */
#define CFE_MISSION_HK_SEND_HK_TOPICID         CFE_MISSION_HK_TIDVAL(SEND_HK)
#define DEFAULT_CFE_MISSION_HK_SEND_HK_TOPICID 0x9B /**< \brief HK Send Housekeeping Data Cmd Message ID */

#define CFE_MISSION_HK_SEND_COMBINED_PKT_TOPICID         CFE_MISSION_HK_TIDVAL(SEND_COMBINED_PKT)
#define DEFAULT_CFE_MISSION_HK_SEND_COMBINED_PKT_TOPICID 0x9C /**< \brief HK Send Combined Pkt Cmd Message ID */

/**\}*/

/**
 * \defgroup cfshktlmtid CFS Housekeeping Telemetry Topic IDs
 * \{
 */

#define CFE_MISSION_HK_HK_TLM_TOPICID         CFE_MISSION_HK_TIDVAL(HK_TLM)
#define DEFAULT_CFE_MISSION_HK_HK_TLM_TOPICID 0x9B /**< \brief HK Housekeeping Telemetry Message ID */

#define CFE_MISSION_HK_COMBINED_PKT1_TLM_TOPICID         CFE_MISSION_HK_TIDVAL(COMBINED_PKT1_TLM)
#define DEFAULT_CFE_MISSION_HK_COMBINED_PKT1_TLM_TOPICID 0x9C /**< \brief HK Combined Packet 1 Message ID */
#define CFE_MISSION_HK_COMBINED_PKT2_TLM_TOPICID         CFE_MISSION_HK_TIDVAL(COMBINED_PKT2_TLM)
#define DEFAULT_CFE_MISSION_HK_COMBINED_PKT2_TLM_TOPICID 0x9D /**< \brief HK Combined Packet 2 Message ID */
#define CFE_MISSION_HK_COMBINED_PKT3_TLM_TOPICID         CFE_MISSION_HK_TIDVAL(COMBINED_PKT3_TLM)
#define DEFAULT_CFE_MISSION_HK_COMBINED_PKT3_TLM_TOPICID 0x9E /**< \brief HK Combined Packet 3 Message ID */
#define CFE_MISSION_HK_COMBINED_PKT4_TLM_TOPICID         CFE_MISSION_HK_TIDVAL(COMBINED_PKT4_TLM)
#define DEFAULT_CFE_MISSION_HK_COMBINED_PKT4_TLM_TOPICID 0x9F /**< \brief HK Combined Packet 4 Message ID */

/**\}*/

#endif
