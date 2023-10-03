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
 *  The CFS Housekeeping (HK) Application header file
 */
#ifndef HK_UTILS_H
#define HK_UTILS_H

/************************************************************************
 * Includes
 ************************************************************************/
#include "cfe.h"
#include "hk_tbldefs.h"

/*************************************************************************
 * Macro definitions
 *************************************************************************/
#define HK_INPUTMID_SUBSCRIBED     (0xFF) /**< \brief Input MsgId has been subscribed to */
#define HK_INPUTMID_NOT_SUBSCRIBED (0)    /**< \brief Input MsgId is not subscribed */

#define HK_DATA_NOT_PRESENT (0) /**< \brief Input MsgId present in output msg */
#define HK_DATA_PRESENT     (1) /**< \brief Input MsgId not present */

#define HK_NO_MISSING_DATA       (0) /**< \brief Output Msg has no missing data */
#define HK_MISSING_DATA_DETECTED (1) /**< \brief Output Msg has missing data */

#define HK_NULL_POINTER_DETECTED (-1) /**< \brief An input table pointer was NULL */

/************************************************************************
 * Prototypes for functions defined in hk_utils.c
 ************************************************************************/

/**
 * \brief Process incoming housekeeping data message
 *
 * \par Description
 *        This routine examines each entry in the table and determines whether
 *        its field comprises a legal entry.  If so, a portion of the input
 *        packet is copied to the appropriate combined output packet.
 *
 * \par Assumptions, External Events, and Notes:
 *        Currently the combined telemetry packets are not initialized after
 *        they are sent so values will repeat if no housekeeping update is
 *        received.
 *
 * \param[in] BufPtr A pointer to the input message.
 *
 * \sa #HK_AppPipe
 */
void HK_ProcessIncomingHkData(const CFE_SB_Buffer_t *BufPtr);

/**
 * \brief Validate Housekeeping Copy Table
 *
 * \par Description
 *      This routine is called from CFE_TBL_Register.  It determines whether
 *      the data contained in the new table is acceptable.
 *
 * \par Assumptions, External Events, and Notes:
 *          None
 *
 * \param[in] TblPtr A pointer to the new table data.
 *
 * \return Table Validation Status
 * \retval #HK_SUCCESS Valid table
 * \retval #HK_ERROR   \copydoc HK_ERROR
 *
 * \sa #HK_TableInit
 */
int32 HK_ValidateHkCopyTable(void *TblPtr);

/**
 * \brief Process New Copy Table
 *
 * \par Description
 *        Upon the arrival of a new HK Copy Table, this routine will
 *        handle whatever is necessary to make this new data functional.
 *
 * \par Assumptions, External Events, and Notes:
 *          None
 *
 * \param[in] CpyTblPtr A pointer to the first entry in the new copy table.
 * \param[in] RtTblPtr  A pointer to the first entry in the run-time table.
 *
 * \return Execution Status
 * \retval #CFE_SUCCESS              \copydoc CFE_SUCCESS
 * \retval #HK_NULL_POINTER_DETECTED \copydoc HK_NULL_POINTER_DETECTED
 */
CFE_Status_t HK_ProcessNewCopyTable(hk_copy_table_entry_t *CpyTblPtr, hk_runtime_tbl_entry_t *RtTblPtr);

/**
 * \brief Tear Down Old Copy Table
 *
 * \par Description
 *        This routine does what is necessary in order to remove the table data
 *          from the system.
 *
 * \par Assumptions, External Events, and Notes:
 *          None
 *
 * \param[in] CpyTblPtr A pointer to the first entry in the copy table.
 * \param[in] RtTblPtr  A pointer to the first entry in the run-time table.
 *
 * \return Execution Status
 * \retval #CFE_SUCCESS              \copydoc CFE_SUCCESS
 * \retval #HK_NULL_POINTER_DETECTED \copydoc HK_NULL_POINTER_DETECTED
 */
CFE_Status_t HK_TearDownOldCopyTable(hk_copy_table_entry_t *CpyTblPtr, hk_runtime_tbl_entry_t *RtTblPtr);

/**
 * \brief Send combined output message
 *
 * \par Description
 *        This routine searches for the combined HK that contains the specified
 *        MID.  Once found, the packet is sent.  If not found, an event is
 *        generated. Also sets the data pieces for this output pkt
 *
 * \par Assumptions, External Events, and Notes:
 *          None
 *
 * \param[in]  WhichMidToSend - the MsgId of the combined output message to send
 */
void HK_SendCombinedHkPacket(CFE_SB_MsgId_t WhichMidToSend);

/**
 * \brief HK_CheckStatusOfTables
 *
 * \par Description
 *        This is a high level routine that controls the actions taken by HK
 *        when a copy table update is detected or a runtime table dump is
 *        pending
 *
 * \par Assumptions, External Events, and Notes:
 *          None
 *
 * \return Table Status
 * \retval #HK_SUCCESS \copydoc HK_SUCCESS
 * \retval #HK_ERROR   \copydoc HK_ERROR
 */
int32 HK_CheckStatusOfTables(void);

/**
 * \brief HK_CheckStatusOfCopyTable
 *
 * \par Description
 *        This is a high level routine that controls the actions taken by HK
 *        when a copy table update is detected
 *
 * \par Assumptions, External Events, and Notes:
 *          None
 *
 * \return Table Status
 * \retval #HK_SUCCESS \copydoc HK_SUCCESS
 * \retval #HK_ERROR   \copydoc HK_ERROR
 */
int32 HK_CheckStatusOfCopyTable(void);

/**
 * \brief HK_CheckStatusOfDumpTable
 *
 * \par Description
 *        This is a high level routine that controls the actions taken by HK
 *        when a runtime table dump is pending
 *
 * \par Assumptions, External Events, and Notes:
 *          None
 *
 * \return Table Status
 * \retval #HK_SUCCESS \copydoc HK_SUCCESS
 * \retval #HK_ERROR   \copydoc HK_ERROR
 */
int32 HK_CheckStatusOfDumpTable(void);

/**
 * \brief Check for Missing Data
 *
 * \par Description
 *        This routine checks for missing data for the given output message.
 *        It returns #HK_MISSING_DATA_DETECTED at the first piece of data that
 *        is not present. The missing Input MsgId is sent back to the caller
 *        through the given pointer named MissingInputMid.
 *
 * \par Assumptions, External Events, and Notes:
 *          None
 *
 * \param[in] OutPktToCheck   MsgId of the combined output message to check
 * \param[in] MissingInputMid A pointer to the caller provided MsgId variable
 *
 * \return Missing Data Status
 * \retval #HK_MISSING_DATA_DETECTED \copydoc HK_MISSING_DATA_DETECTED
 * \retval #HK_NO_MISSING_DATA       \copydoc HK_NO_MISSING_DATA
 */
int32 HK_CheckForMissingData(CFE_SB_MsgId_t OutPktToCheck, CFE_SB_MsgId_t *MissingInputMid);

/**
 * \brief Set Data Present Flags to 'Not Present'
 *
 * \par Description
 *        This routine will set the data present flags to data-not-present for
 *        given combined output message
 *
 * \par Assumptions, External Events, and Notes:
 *          None
 *
 * \param[in] OutPkt The MsgId whose data present flags will be set.
 */
void HK_SetFlagsToNotPresent(CFE_SB_MsgId_t OutPkt);

#endif
