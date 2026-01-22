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
 *  The CFS Housekeeping (HK) Application header file
 */
#ifndef HK_APP_H
#define HK_APP_H

/************************************************************************
 * Includes
 ************************************************************************/

#include "cfe.h"
#include "hk_msg.h"
#include "hk_tbl.h"
#include "hk_utils.h"
#include "hk_platform_cfg.h"

/*************************************************************************
 * Macro definitions
 *************************************************************************/
#define HK_PIPE_NAME "HK_CMD_PIPE" /**< \brief Application Pipe Name  */

#define HK_SUCCESS           (0)  /**< \brief HK return code for success */
#define HK_ERROR             (-1) /**< \brief HK return code for general error */
#define HK_BAD_MSG_LENGTH_RC (-2) /**< \brief HK return code for unexpected cmd length */

/**
 * \brief Wakeup for HK
 *
 *  \par Description
 *      Wakes up HK every 1 second for routine maintenance whether a
 *      message was received or not.
 */
#define HK_SB_TIMEOUT 1000

/************************************************************************
 * Type Definitions
 ************************************************************************/

/**
 * \brief HK global data structure
 */
typedef struct
{
    HK_HkPacket_t HkPacket; /**< \brief HK Housekeeping Packet */

    CFE_SB_PipeId_t CmdPipe;    /**< \brief Pipe Id for HK command pipe */
    uint8           CmdCounter; /**< \brief Number of valid commands received */
    uint8           ErrCounter; /**< \brief Number of invalid commands received */

    uint16 MissingDataCtr;      /**< \brief Number of times missing data was detected */
    uint16 CombinedPacketsSent; /**< \brief Count of combined output msgs sent */

    CFE_ES_MemHandle_t MemPoolHandle; /**< \brief HK mempool handle for output pkts */
    uint32             RunStatus;     /**< \brief HK App run status */

    CFE_TBL_Handle_t CopyTableHandle;    /**< \brief Copy Table handle */
    CFE_TBL_Handle_t RuntimeTableHandle; /**< \brief Run-time table handle */

    HK_CopyTableEntry_t *   CopyTablePtr;    /**< \brief Ptr to copy table entry */
    HK_RuntimeTableEntry_t *RuntimeTablePtr; /**< \brief Ptr to run-time table entry */

    uint8 MemPoolBuffer[HK_NUM_BYTES_IN_MEM_POOL]; /**< \brief HK mempool buffer */
} HK_AppData_t;

/*************************************************************************
 * Exported data
 *************************************************************************/

extern HK_AppData_t HK_AppData; /**< \brief HK Housekeeping Packet */

/************************************************************************
 * Exported Functions
 ************************************************************************/

/**
 * \brief CFS Housekeeping (HK) application entry point
 *
 *  \par Description
 *       Housekeeping application entry point and main process loop.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 */
void HK_AppMain(void);

/**
 * \brief Initialize the housekeeping application
 *
 *  \par Description
 *       Housekeeping application initialization routine. This
 *       function performs all the required startup steps to
 *       get the application registered with the cFE services so
 *       it can begin to receive command messages.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t HK_AppInit(void);

/**
 * \brief Initialize the Copy Table and the Runtime Table
 *
 *  \par Description
 *       Registers the Copy table and Runtime table with cFE Table
 *       Services. Also processes the copy table.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \sa #HK_AppInit
 */
CFE_Status_t HK_TableInit(void);

/**
 * \brief Reset housekeeping data
 *
 *  \par Description
 *       Function called in response to a Reset Counters Command. This
 *       function will reset the HK housekeeping data.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \sa #HK_RESET_COUNTERS_CC
 */
void HK_ResetHkData(void);

#endif
