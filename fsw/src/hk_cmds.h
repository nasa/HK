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
#ifndef HK_CMDS_H
#define HK_CMDS_H

/************************************************************************
 * Includes
 ************************************************************************/

#include "cfe_sb_api_typedefs.h"
#include "hk_msg.h"

/************************************************************************
 * Exported Functions
 ************************************************************************/

/**
 * \brief Send Combined Housekeeping Message
 *
 *  \par Description
 *       Processes the command to send a combined housekeeping message
 *
 *  \par Assumptions, External Events, and Notes:
 *       This command does not affect the command execution counter, but
 *       this command will increment the cmd error counter if an invalid cmd
 *       length is detected.
 *
 *  \param [in]  BufPtr Pointer to Software Bus buffer
 */
CFE_Status_t HK_SendCombinedPktCmd(const HK_SendCombinedPktCmd_t *BufPtr);

/**
 * \brief Process housekeeping request
 *
 *  \par Description
 *       Processes an on-board housekeeping request message.
 *
 *  \par Assumptions, External Events, and Notes:
 *       This command does not affect the command execution counter, but
 *       this command will increment the cmd error counter if an invalid cmd
 *       length is detected.
 *
 *  \param [in] BufPtr Pointer to command message header
 */
CFE_Status_t HK_SendHkCmd(const HK_SendHkCmd_t *BufPtr);

/**
 * \brief Process noop command
 *
 *  \par Description
 *       Processes a noop ground command.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param [in]  BufPtr Pointer to Software Bus buffer
 *
 *  \sa #HK_NOOP_CC
 */
CFE_Status_t HK_NoopCmd(const HK_NoopCmd_t *BufPtr);

/**
 * \brief Process reset counters command
 *
 *  \par Description
 *       Processes a reset counters ground command which will reset
 *       the memory manager commmand error and command execution counters
 *       to zero.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param [in]   BufPtr Pointer to Software Bus buffer
 *
 *  \sa #HK_RESET_COUNTERS_CC
 */
CFE_Status_t HK_ResetCountersCmd(const HK_ResetCountersCmd_t *BufPtr);

#endif
