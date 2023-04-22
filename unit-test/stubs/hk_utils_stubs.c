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
 *
 * Auto-Generated stub implementations for functions defined in hk_utils header
 */

#include "hk_utils.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for HK_CheckForMissingData()
 * ----------------------------------------------------
 */
int32 HK_CheckForMissingData(CFE_SB_MsgId_t OutPktToCheck, CFE_SB_MsgId_t *MissingInputMid)
{
    UT_GenStub_SetupReturnBuffer(HK_CheckForMissingData, int32);

    UT_GenStub_AddParam(HK_CheckForMissingData, CFE_SB_MsgId_t, OutPktToCheck);
    UT_GenStub_AddParam(HK_CheckForMissingData, CFE_SB_MsgId_t *, MissingInputMid);

    UT_GenStub_Execute(HK_CheckForMissingData, Basic, NULL);

    return UT_GenStub_GetReturnValue(HK_CheckForMissingData, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_CheckStatusOfCopyTable()
 * ----------------------------------------------------
 */
int32 HK_CheckStatusOfCopyTable(void)
{
    UT_GenStub_SetupReturnBuffer(HK_CheckStatusOfCopyTable, int32);

    UT_GenStub_Execute(HK_CheckStatusOfCopyTable, Basic, NULL);

    return UT_GenStub_GetReturnValue(HK_CheckStatusOfCopyTable, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_CheckStatusOfDumpTable()
 * ----------------------------------------------------
 */
int32 HK_CheckStatusOfDumpTable(void)
{
    UT_GenStub_SetupReturnBuffer(HK_CheckStatusOfDumpTable, int32);

    UT_GenStub_Execute(HK_CheckStatusOfDumpTable, Basic, NULL);

    return UT_GenStub_GetReturnValue(HK_CheckStatusOfDumpTable, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_CheckStatusOfTables()
 * ----------------------------------------------------
 */
int32 HK_CheckStatusOfTables(void)
{
    UT_GenStub_SetupReturnBuffer(HK_CheckStatusOfTables, int32);

    UT_GenStub_Execute(HK_CheckStatusOfTables, Basic, NULL);

    return UT_GenStub_GetReturnValue(HK_CheckStatusOfTables, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_ProcessIncomingHkData()
 * ----------------------------------------------------
 */
void HK_ProcessIncomingHkData(const CFE_SB_Buffer_t *BufPtr)
{
    UT_GenStub_AddParam(HK_ProcessIncomingHkData, const CFE_SB_Buffer_t *, BufPtr);

    UT_GenStub_Execute(HK_ProcessIncomingHkData, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_ProcessNewCopyTable()
 * ----------------------------------------------------
 */
CFE_Status_t HK_ProcessNewCopyTable(hk_copy_table_entry_t *CpyTblPtr, hk_runtime_tbl_entry_t *RtTblPtr)
{
    UT_GenStub_SetupReturnBuffer(HK_ProcessNewCopyTable, CFE_Status_t);

    UT_GenStub_AddParam(HK_ProcessNewCopyTable, hk_copy_table_entry_t *, CpyTblPtr);
    UT_GenStub_AddParam(HK_ProcessNewCopyTable, hk_runtime_tbl_entry_t *, RtTblPtr);

    UT_GenStub_Execute(HK_ProcessNewCopyTable, Basic, NULL);

    return UT_GenStub_GetReturnValue(HK_ProcessNewCopyTable, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_SendCombinedHkPacket()
 * ----------------------------------------------------
 */
void HK_SendCombinedHkPacket(CFE_SB_MsgId_t WhichMidToSend)
{
    UT_GenStub_AddParam(HK_SendCombinedHkPacket, CFE_SB_MsgId_t, WhichMidToSend);

    UT_GenStub_Execute(HK_SendCombinedHkPacket, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_SetFlagsToNotPresent()
 * ----------------------------------------------------
 */
void HK_SetFlagsToNotPresent(CFE_SB_MsgId_t OutPkt)
{
    UT_GenStub_AddParam(HK_SetFlagsToNotPresent, CFE_SB_MsgId_t, OutPkt);

    UT_GenStub_Execute(HK_SetFlagsToNotPresent, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_TearDownOldCopyTable()
 * ----------------------------------------------------
 */
CFE_Status_t HK_TearDownOldCopyTable(hk_copy_table_entry_t *CpyTblPtr, hk_runtime_tbl_entry_t *RtTblPtr)
{
    UT_GenStub_SetupReturnBuffer(HK_TearDownOldCopyTable, CFE_Status_t);

    UT_GenStub_AddParam(HK_TearDownOldCopyTable, hk_copy_table_entry_t *, CpyTblPtr);
    UT_GenStub_AddParam(HK_TearDownOldCopyTable, hk_runtime_tbl_entry_t *, RtTblPtr);

    UT_GenStub_Execute(HK_TearDownOldCopyTable, Basic, NULL);

    return UT_GenStub_GetReturnValue(HK_TearDownOldCopyTable, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_ValidateHkCopyTable()
 * ----------------------------------------------------
 */
int32 HK_ValidateHkCopyTable(void *TblPtr)
{
    UT_GenStub_SetupReturnBuffer(HK_ValidateHkCopyTable, int32);

    UT_GenStub_AddParam(HK_ValidateHkCopyTable, void *, TblPtr);

    UT_GenStub_Execute(HK_ValidateHkCopyTable, Basic, NULL);

    return UT_GenStub_GetReturnValue(HK_ValidateHkCopyTable, int32);
}
