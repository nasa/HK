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
 *  The CFS Housekeeping (HK) Application Message IDs header file
 */
#ifndef HK_MSGIDS_H
#define HK_MSGIDS_H

/**
 * \defgroup cfshkcmdmid CFS Housekeeping Command Message IDs
 * \{
 */

#define HK_CMD_MID     0x189A /**< \brief HK Ground Commands Message ID */
#define HK_SEND_HK_MID 0x189B /**< \brief HK Send Housekeeping Data Cmd Message ID */

#define HK_SEND_COMBINED_PKT_MID 0x189C /**< \brief HK Send Combined Pkt Cmd Message ID */

/**\}*/

/**
 * \defgroup cfshktlmmid CFS Housekeeping Telemetry Message IDs
 * \{
 */

#define HK_HK_TLM_MID 0x089B /**< \brief HK Housekeeping Telemetry Message ID */

#define HK_COMBINED_PKT1_MID 0x089C /**< \brief HK Combined Packet 1 Message ID */
#define HK_COMBINED_PKT2_MID 0x089D /**< \brief HK Combined Packet 2 Message ID */
#define HK_COMBINED_PKT3_MID 0x089E /**< \brief HK Combined Packet 3 Message ID */
#define HK_COMBINED_PKT4_MID 0x089F /**< \brief HK Combined Packet 4 Message ID */

/**\}*/

#endif
