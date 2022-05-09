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

#include "hk_utils.h"
#include "uttest.h"
#include "utstubs.h"

void HK_ProcessIncomingHkData(const CFE_SB_Buffer_t *BufPtr)
{
    UT_Stub_RegisterContext(UT_KEY(HK_ProcessIncomingHkData), BufPtr);
    UT_DEFAULT_IMPL(HK_ProcessIncomingHkData);
}

int32 HK_ValidateHkCopyTable(void *TblPtr)
{
    UT_Stub_RegisterContext(UT_KEY(HK_ValidateHkCopyTable), TblPtr);
    return UT_DEFAULT_IMPL(HK_ValidateHkCopyTable);
}

int32 HK_ProcessNewCopyTable(hk_copy_table_entry_t *CpyTblPtr, hk_runtime_tbl_entry_t *RtTblPtr)
{
    UT_Stub_RegisterContext(UT_KEY(HK_ProcessNewCopyTable), CpyTblPtr);
    UT_Stub_RegisterContext(UT_KEY(HK_ProcessNewCopyTable), RtTblPtr);
    return UT_DEFAULT_IMPL(HK_ProcessNewCopyTable);
}

int32 HK_TearDownOldCopyTable(hk_copy_table_entry_t *CpyTblPtr, hk_runtime_tbl_entry_t *RtTblPtr)
{
    UT_Stub_RegisterContext(UT_KEY(HK_TearDownOldCopyTable), CpyTblPtr);
    UT_Stub_RegisterContext(UT_KEY(HK_TearDownOldCopyTable), RtTblPtr);
    return UT_DEFAULT_IMPL(HK_TearDownOldCopyTable);
}

void HK_SendCombinedHkPacket(CFE_SB_MsgId_t WhichMidToSend)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(HK_SendCombinedHkPacket), WhichMidToSend);
    UT_DEFAULT_IMPL(HK_SendCombinedHkPacket);
}

int32 HK_CheckStatusOfTables(void)
{
    return UT_DEFAULT_IMPL(HK_CheckStatusOfTables);
}

int32 HK_CheckStatusOfCopyTable(void)
{
    return UT_DEFAULT_IMPL(HK_CheckStatusOfCopyTable);
}

int32 HK_CheckStatusOfDumpTable(void)
{
    return UT_DEFAULT_IMPL(HK_CheckStatusOfDumpTable);
}

int32 HK_CheckForMissingData(CFE_SB_MsgId_t OutPktToCheck, CFE_SB_MsgId_t *MissingInputMid)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(HK_CheckForMissingData), OutPktToCheck);
    UT_Stub_RegisterContext(UT_KEY(HK_CheckForMissingData), MissingInputMid);
    return UT_DEFAULT_IMPL(HK_CheckForMissingData);
}

void HK_SetFlagsToNotPresent(CFE_SB_MsgId_t OutPkt)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(HK_SetFlagsToNotPresent), OutPkt);
    UT_DEFAULT_IMPL(HK_SetFlagsToNotPresent);
}

int32 HK_VerifyCmdLength(const CFE_SB_Buffer_t *BufPtr, size_t ExpectedLength)
{
    UT_Stub_RegisterContext(UT_KEY(HK_VerifyCmdLength), BufPtr);
    UT_Stub_RegisterContextGenericArg(UT_KEY(HK_VerifyCmdLength), ExpectedLength);
    return UT_DEFAULT_IMPL(HK_VerifyCmdLength);
}
