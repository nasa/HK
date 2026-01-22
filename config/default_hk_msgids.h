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
#ifndef DEFAULT_HK_MSGIDS_H
#define DEFAULT_HK_MSGIDS_H

#include "hk_msgid_values.h"

/**
 * \defgroup cfshkcmdmid CFS Housekeeping Command Message IDs
 * \{
 */

#define HK_CMD_MID               CFE_PLATFORM_HK_CMD_MIDVAL(CMD)
#define HK_SEND_HK_MID           CFE_PLATFORM_HK_CMD_MIDVAL(SEND_HK)
#define HK_SEND_COMBINED_PKT_MID CFE_PLATFORM_HK_CMD_MIDVAL(SEND_COMBINED_PKT)

/**\}*/

/**
 * \defgroup cfshktlmmid CFS Housekeeping Telemetry Message IDs
 * \{
 */

#define HK_HK_TLM_MID CFE_PLATFORM_HK_TLM_MIDVAL(HK_TLM) /**< \brief HK Housekeeping Telemetry Message ID */

#define HK_COMBINED_PKT1_MID \
    CFE_PLATFORM_HK_TLM_MIDVAL(COMBINED_PKT1_TLM) /**< \brief HK Combined Packet 1 Message ID */
#define HK_COMBINED_PKT2_MID \
    CFE_PLATFORM_HK_TLM_MIDVAL(COMBINED_PKT2_TLM) /**< \brief HK Combined Packet 2 Message ID */
#define HK_COMBINED_PKT3_MID \
    CFE_PLATFORM_HK_TLM_MIDVAL(COMBINED_PKT3_TLM) /**< \brief HK Combined Packet 3 Message ID */
#define HK_COMBINED_PKT4_MID \
    CFE_PLATFORM_HK_TLM_MIDVAL(COMBINED_PKT4_TLM) /**< \brief HK Combined Packet 4 Message ID */

/**\}*/

#endif
