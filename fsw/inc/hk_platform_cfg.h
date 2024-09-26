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
 *  The CFS Housekeeping (HK) Application platform configuration header file
 */
#ifndef HK_PLATFORM_CFG_H
#define HK_PLATFORM_CFG_H

/**
 * \defgroup cfshkplatformcfg CFS Housekeeping Platform Configuration
 * \{
 */

/**
 *  \brief Application Pipe Depth
 *
 *  \par Description:
 *       Dictates the pipe depth of the hk command pipe.
 *
 *  \par Limits
 *       The minimum size of this parameter is 1
 *       The maximum size dictated by cFE platform configuration
 *       parameter  OS_QUEUE_MAX_DEPTH
 */
#define HK_PIPE_DEPTH 40

/**
 *  \brief Discard Incomplete Combo Packets
 *
 *  \par Description:
 *       Dictates whether combo packets that have not had all data contents
 *       updated since last requested will be discarded (YES = 1) or sent
 *       anyway (NO = 0).
 *
 *  \par Limits
 *       This parameter can be set to 0 or 1 only.
 */
#define HK_DISCARD_INCOMPLETE_COMBO 0

/**
 *  \brief Number of bytes in the HK Memory Pool
 *
 *  \par Description:
 *       The HK memory pool contains the memory needed for the output packets.
 *       The output packets are dynamically allocated from this pool when the
 *       HK copy table is initially processed or loaded with new data.
 *
 *  \par Limits
 *       The Housekeeping app does not place a limit on this parameter, but there is
 *       an overhead cost in the memory pool.  The value must be larger than what is
 *       needed.
 */
#define HK_NUM_BYTES_IN_MEM_POOL (6 * 1024)

/**
 *  \brief Name of the HK Copy Table
 *
 *  \par Description:
 *       This parameter defines the name of the HK Copy Table.
 *
 *  \par Limits
 *       The Housekeeping app does not place a limit on this parameter
 */
#define HK_COPY_TABLE_NAME "CopyTable"

/**
 *  \brief Name of the HK Run-time Table
 *
 *  \par Description:
 *       This parameter defines the name of the HK Run-time Table.
 *
 *  \par Limits
 *       The Housekeeping app does not place a limit on this parameter
 */
#define HK_RUNTIME_TABLE_NAME "RuntimeTable"

/**
 *  \brief HK Copy Table Filename
 *
 *  \par Description:
 *       The value of this constant defines the filename of the HK Copy Table
 *
 *  \par Limits
 *       The length of each string, including the NULL terminator cannot exceed
 *       the #OS_MAX_PATH_LEN value.
 */

#define HK_COPY_TABLE_FILENAME "/cf/hk_cpy_tbl.tbl"

/**
 * \brief Mission specific version number for HK application
 *
 *  \par Description:
 *       An application version number consists of four parts:
 *       major version number, minor version number, revision
 *       number and mission specific revision number. The mission
 *       specific revision number is defined here and the other
 *       parts are defined in "hk_version.h".
 *
 *  \par Limits:
 *       Must be defined as a numeric value that is greater than
 *       or equal to zero.
 */
#define HK_MISSION_REV 0

/**
 * \brief Maximum size allowed for HK combined packet
 *
 *  \par Description:
 *       Defines the maximum allowed size (in bytes) of a combined packet.
 *
 *  \par Limits:
 *       Must be defined as a numeric value that is greater than
 *       or equal to zero.
 */
#define HK_MAX_COMBINED_PACKET_SIZE 1500

/**\}*/

#endif
