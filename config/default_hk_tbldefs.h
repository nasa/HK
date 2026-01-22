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
 *  The CFS Housekeeping (HK) Application Table Structure Definition
 *
 * This is a compatibility header for the "hk_tbldefs.h" file that has
 * traditionally provided the table definitions for cFS apps.
 *
 * @note This file may be overridden/superceded by mission-provided defintions
 * either by overriding this header or by generating definitions from a command/data
 * dictionary tool.
 */
#ifndef DEFAULT_HK_TBLDEFS_H
#define DEFAULT_HK_TBLDEFS_H

/************************************************************************
** Includes
*************************************************************************/
#include "common_types.h"
#include "hk_mission_cfg.h"
#include "cfe_sb_extern_typedefs.h"
#include "cfe_es_extern_typedefs.h"

/**  \brief HK Copy Table Entry Format
 */
typedef struct
{
    CFE_SB_MsgId_t InputMid;     /**< \brief MsgId of the input packet */
    uint16         InputOffset;  /**< \brief ByteOffset into the input pkt where copy will begin */
    CFE_SB_MsgId_t OutputMid;    /**< \brief MsgId of the output packet */
    uint16         OutputOffset; /**< \brief ByteOffset into the output pkt where data will be placed */
    uint16         NumBytes;     /**< \brief Number of data bytes to copy from input to output pkt */
} HK_CopyTableEntry_t;

/**  \brief HK Run-time Table Entry Format
 */
typedef struct
{
    CFE_ES_MemAddress_t OutputPktAddr;      /**< \brief Addr of output packet */
    uint8               InputMidSubscribed; /**< \brief Indicates if input MID has been subscribed to */
    uint8               DataPresent;        /**< \brief Indicates if the data associated with the entry is present */
} HK_RuntimeTableEntry_t;

#endif
