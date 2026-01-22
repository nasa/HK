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
 *  The CFS Housekeeping (HK) Application function codes
 *
 */
#ifndef DEFAULT_HK_MSGDEFS_H
#define DEFAULT_HK_MSGDEFS_H

#include "common_types.h"
#include "cfe_es_extern_typedefs.h"

/**
 *  \brief HK Application housekeeping Payload
 */
typedef struct
{
    uint8              CmdCounter;          /**< \brief Count of valid commands received */
    uint8              ErrCounter;          /**< \brief Count of invalid commands received */
    uint16             Padding;             /**< \brief Padding to force 32 bit alignment */
    uint16             CombinedPacketsSent; /**< \brief Count of combined tlm pkts sent */
    uint16             MissingDataCtr;      /**< \brief Number of times missing data was detected */
    CFE_ES_MemHandle_t MemPoolHandle;       /**< \brief Memory pool handle used to get mempool diags */
} HK_HkTlm_Payload_t;

#endif
