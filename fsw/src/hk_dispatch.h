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
 *  The CFS Housekeeping (HK) Dispatch header file
 */

#ifndef HK_DISPATCH_H
#define HK_DISPATCH_H

/************************************************************************
 * Includes
 ************************************************************************/

#include "cfe.h"

/************************************************************************
 * Exported Functions
 ************************************************************************/

/**
 * \brief  Verify length of HK commands
 *
 *  \par Description
 *       Function called when an HK command is received.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] BufPtr         Pointer to the Software Buss Buffer
 *  \param[in] ExpectedLength The expected lenght of the command
 *
 *  \return Command Length Status
 *  \retval #HK_SUCCESS           Length Valid
 *  \retval #HK_BAD_MSG_LENGTH_RC \copydoc HK_BAD_MSG_LENGTH_RC
 */
int32 HK_VerifyCmdLength(const CFE_SB_Buffer_t *BufPtr, size_t ExpectedLength);

/**
 * \brief Process a command pipe message
 *
 *  \par Description
 *       Processes a single software bus command pipe message. Checks
 *       the message and command IDs and calls the appropriate routine
 *       to handle the command.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param [in]  BufPtr Pointer to Software Bus buffer
 */
void HK_AppPipe(const CFE_SB_Buffer_t *BufPtr);

#endif
