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

/*
 * App Includes
 */

#include "hk_app.h"
#include "hk_msg.h"
#include "hk_events.h"
#include "hk_version.h"
#include "hk_msgids.h"
#include "hk_utils.h"
#include "hk_test_utils.h"

/* UT includes */
#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

#include <unistd.h>
#include <stdlib.h>
#include "cfe.h"
#include "cfe_msgids.h"

/* hk_utils_tests globals */
hk_runtime_tbl_entry_t NewRtTblPtr[HK_COPY_TABLE_ENTRIES];
hk_copy_table_entry_t  NewCopyTblPtr[HK_COPY_TABLE_ENTRIES];
uint8                  call_count_CFE_EVS_SendEvent;

/*
 * Function Definitions
 */

void UT_CFE_TBL_GetAddress_Handler(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    HK_Test_InitGoodCopyTable(NewCopyTblPtr);
    HK_Test_InitGoodRuntimeTable(NewRtTblPtr);

    HK_AppData.CopyTablePtr    = NewCopyTblPtr;
    HK_AppData.RuntimeTablePtr = NewRtTblPtr;
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_ProcessIncomingHkData                        */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_ProcessIncomingHkData
 *
 * Case: Tests the case where the provided MessageID is not found
 *       in the copy table.
 */
void Test_HK_ProcessIncomingHkData_MidNotFound(void)
{
    /* Arrange */
    int32                  i;
    int32                  NumEntriesWithDataPresent = 0;
    CFE_SB_MsgId_t         forced_MsgID              = HK_UT_MID_100; /* not in copy table */
    CFE_SB_Buffer_t        Buf;
    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &forced_MsgID, sizeof(forced_MsgID), false);

    HK_Test_InitGoodCopyTable(CopyTblPtr);
    HK_Test_InitGoodRuntimeTable(RtTblPtr);

    HK_AppData.CopyTablePtr    = CopyTblPtr;
    HK_AppData.RuntimeTablePtr = RtTblPtr;

    /* Act */
    HK_ProcessIncomingHkData(&Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);

    /* there should be no data present because the MID was not found in the
     * table */
    for (i = 0; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        if (RtTblPtr[i].DataPresent == HK_DATA_PRESENT)
        {
            NumEntriesWithDataPresent++;
        }
    }

    UtAssert_INT32_EQ(NumEntriesWithDataPresent, 0);
}

/*
 * Function under test: HK_ProcessIncomingHkData
 *
 * Case: Tests the case where the provided Message ID exists in the
 *       copy table and the length is valid (equal to the sum of the
 *       input offset and the number of bytes).
 */
void Test_HK_ProcessIncomingHkData_LengthOkEqual(void)
{
    /* Arrange */
    int32           NumEntriesWithDataPresent = 0;
    CFE_SB_MsgId_t  forced_MsgID;
    size_t          forced_Size;
    CFE_SB_Buffer_t Buf;
    int             i;
    CFE_SB_Buffer_t OutputPkt;

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    HK_Test_InitGoodCopyTable(CopyTblPtr);
    HK_Test_InitGoodRuntimeTable(RtTblPtr);

    RtTblPtr[2].OutputPktAddr = &OutputPkt;

    HK_AppData.CopyTablePtr    = CopyTblPtr;
    HK_AppData.RuntimeTablePtr = RtTblPtr;

    forced_MsgID = CopyTblPtr[2].InputMid;
    forced_Size  = (CopyTblPtr[2].InputOffset + CopyTblPtr[2].NumBytes);

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &forced_MsgID, sizeof(forced_MsgID), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &forced_Size, sizeof(forced_Size), false);

    /* Act */
    HK_ProcessIncomingHkData(&Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);

    /* there should be one piece of data present because the MID was found in the
     * table and the length was valid */
    for (i = 0; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        if (RtTblPtr[i].DataPresent == HK_DATA_PRESENT)
        {
            NumEntriesWithDataPresent++;
        }
    }

    UtAssert_INT32_EQ(NumEntriesWithDataPresent, 1);

    /* TODO - there's probably a way in this test to check was was copied into
     * the output buffer */
}

/*
 * Function under test: HK_ProcessIncomingHkData
 *
 * Case: Tests the case where the provided Message ID exists in the
 *       copy table and the length is valid (greater than the sum of the
 *       input offset and the number of bytes).
 */
void Test_HK_ProcessIncomingHkData_LengthOkGreater(void)
{
    /* Arrange */
    int32           i;
    int32           NumEntriesWithDataPresent = 0;
    CFE_SB_MsgId_t  forced_MsgID;
    size_t          forced_Size;
    CFE_SB_Buffer_t Buf;
    CFE_SB_Buffer_t OutputPkt;

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    HK_Test_InitGoodCopyTable(CopyTblPtr);
    HK_Test_InitGoodRuntimeTable(RtTblPtr);

    RtTblPtr[2].OutputPktAddr = &OutputPkt;

    HK_AppData.CopyTablePtr    = CopyTblPtr;
    HK_AppData.RuntimeTablePtr = RtTblPtr;

    forced_MsgID = CopyTblPtr[2].InputMid;
    forced_Size  = (CopyTblPtr[2].InputOffset + CopyTblPtr[2].NumBytes) + 2;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &forced_MsgID, sizeof(forced_MsgID), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &forced_Size, sizeof(forced_Size), false);

    /* Act */
    HK_ProcessIncomingHkData(&Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);

    /* there should be one piece of data present because the MID was found in the
     * table and the length was valid */
    for (i = 0; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        if (RtTblPtr[i].DataPresent == HK_DATA_PRESENT)
        {
            NumEntriesWithDataPresent++;
        }
    }

    UtAssert_INT32_EQ(NumEntriesWithDataPresent, 1);

    /* TODO - there's probably a way in this test to check was was copied into
     * the output buffer */
}

/*
 * Function under test: HK_ProcessIncomingHkData
 *
 * Case: Tests the case where the provided MessageID exists in the
 *       copy table and the length is invalid, causing a message error.
 */
void Test_HK_ProcessIncomingHkData_MessageError(void)
{
    /* Arrange */
    int32                  i;
    int32                  NumEntriesWithDataPresent = 0;
    CFE_SB_MsgId_t         forced_MsgID;
    size_t                 forced_Size;
    CFE_SB_Buffer_t        Buf;
    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];
    int32                  strCmpResult;
    char                   ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    HK_Test_InitGoodCopyTable(CopyTblPtr);
    HK_Test_InitGoodRuntimeTable(RtTblPtr);

    HK_AppData.CopyTablePtr    = CopyTblPtr;
    HK_AppData.RuntimeTablePtr = RtTblPtr;

    forced_MsgID = CopyTblPtr[2].InputMid;
    forced_Size  = (CopyTblPtr[2].InputOffset + CopyTblPtr[2].NumBytes) - 2;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &forced_MsgID, sizeof(forced_MsgID), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &forced_Size, sizeof(forced_Size), false);

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "HK table definition exceeds packet length. MID:0x%%08lX, Length:%%d, Count:%%d");

    /* Act */
    HK_ProcessIncomingHkData(&Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_ACCESSING_PAST_PACKET_END_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);

    /* there should be no data present because the length was not valid*/
    for (i = 0; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        if (RtTblPtr[i].DataPresent == HK_DATA_PRESENT)
        {
            NumEntriesWithDataPresent++;
        }
    }

    UtAssert_INT32_EQ(NumEntriesWithDataPresent, 0);
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_ValidateHkCopyTable                          */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_ValidateHkCopyTable
 *
 * Case: Tests that the HK_ValidateHkCopyTable returns HK_SUCCESS
 */

void Test_HK_ValidateHkCopyTable_Success(void)
{
    /* Arrange */
    hk_copy_table_entry_t CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    HK_Test_InitGoodCopyTable(CopyTblPtr);

    /* Act */
    int32 ReturnValue = HK_ValidateHkCopyTable(CopyTblPtr);

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_SUCCESS);
}

/*
 * Function under test: HK_ValidateHkCopyTable
 *
 * Case: Tests that the HK_ValidateHkCopyTable returns HK_ERROR
 */

void Test_HK_ValidateHkCopyTable_Error(void)
{
    /* Arrange */
    hk_copy_table_entry_t CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    HK_Test_InitOverflowCopyTable(CopyTblPtr);

    /* Act */
    int32 ReturnValue = HK_ValidateHkCopyTable(CopyTblPtr);

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_ERROR);
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_ProcessNewCopyTable                          */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_ProcessNewCopyTable
 *
 * Case: Tests the case where the provided Copy Table Pointer is NULL.
 */
void Test_HK_ProcessNewCopyTable_NullCpyTbl(void)
{
    /* Arrange */
    hk_runtime_tbl_entry_t RtTblPtr;
    int32                  strCmpResult;
    CFE_Status_t           ReturnValue;

    char ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Null pointer detected in new copy tbl processing: CpyTbl = %%p, RtTbl = %%p");

    /* Act */
    ReturnValue = HK_ProcessNewCopyTable(NULL, &RtTblPtr);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_NULL_POINTER_DETECTED);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_NULL_POINTER_NEWCPY_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_ProcessNewCopyTable
 *
 * Case: Tests the case where the provided RT Table Pointer is NULL.
 */
void Test_HK_ProcessNewCopyTable_NullRtTbl(void)
{
    /* Arrange */
    hk_copy_table_entry_t CpyTblPtr;
    int32                 strCmpResult;
    char                  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    CFE_Status_t          ReturnValue;

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Null pointer detected in new copy tbl processing: CpyTbl = %%p, RtTbl = %%p");

    /* Act */
    ReturnValue = HK_ProcessNewCopyTable(&CpyTblPtr, NULL);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_NULL_POINTER_DETECTED);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_NULL_POINTER_NEWCPY_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_ProcessNewCopyTable
 *
 * Case: Tests the case where the call to CFE_ES_GetPoolBuf fails.
 */
void Test_HK_ProcessNewCopyTable_PoolBufFail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        i;
    int32        ExtraSubscribes = 0;
    int          strCmpResult;

    char ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    memset(RtTblPtr, 0, sizeof(RtTblPtr));

    HK_Test_InitGoodCopyTable(CopyTblPtr);

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "HK Processing New Table: ES_GetPoolBuf for size %%d returned 0x%%04X");

    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_GetPoolBuf), (CFE_SUCCESS - 1));

    /* Act */
    ReturnValue = HK_ProcessNewCopyTable(CopyTblPtr, RtTblPtr);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, CFE_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 5);

    for (i = 0; i < 5; i++)
    {
        UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[i].EventID, HK_MEM_POOL_MALLOC_FAILED_EID);

        UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[i].EventType, CFE_EVS_EventType_ERROR);

        strCmpResult =
            strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[i].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

        UtAssert_True(strCmpResult == 0, "Event #%d string matched expected result, '%s'", (int)i,
                      context_CFE_EVS_SendEvent[i].Spec);
    }

    UtAssert_INT32_EQ(RtTblPtr[0].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[1].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[2].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[3].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[4].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);

    for (i = 5; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        if (RtTblPtr[i].InputMidSubscribed == HK_INPUTMID_SUBSCRIBED)
        {
            ExtraSubscribes++;
        }
    }
    UtAssert_INT32_EQ(ExtraSubscribes, 0);
}

/*
 * Function under test: HK_ProcessNewCopyTable
 *
 * Case: Tests the case where the call to CFE_SB_Subscribe fails.
 */
void Test_HK_ProcessNewCopyTable_SubscribeFail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int          strCmpResult;
    int32        i;
    int32        SubscriptionCount = 0;

    char ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "HK Processing New Table:SB_Subscribe for Mid 0x%%08lX returned 0x%%04X");

    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_Subscribe), !CFE_SUCCESS);

    HK_Test_InitGoodCopyTable(CopyTblPtr);
    HK_Test_InitGoodRuntimeTable(RtTblPtr);

    /* Act */
    ReturnValue = HK_ProcessNewCopyTable(CopyTblPtr, RtTblPtr);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, CFE_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 5);

    for (i = 0; i < 5; i++)
    {
        UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[i].EventID, HK_CANT_SUBSCRIBE_TO_SB_PKT_EID);

        UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[i].EventType, CFE_EVS_EventType_ERROR);

        strCmpResult =
            strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[i].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

        UtAssert_True(strCmpResult == 0, "Event #%d string matched expected result, '%s'", (int)i,
                      context_CFE_EVS_SendEvent[i].Spec);
    }

    for (i = 0; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        if (RtTblPtr[i].InputMidSubscribed == HK_INPUTMID_SUBSCRIBED)
        {
            SubscriptionCount++;
        }
    }
    UtAssert_INT32_EQ(SubscriptionCount, 0);
}

/*
 * Function under test: HK_ProcessNewCopyTable
 *
 * Case: Tests the case where the HK_ProcessNewCopyTable succeeds.
 *       Note that this function has multiple branches that lead to
 *       success. In this case, all populated Copy Table entries
 *       have the InputMid and OutputMid defined.
 */
void Test_HK_ProcessNewCopyTable_Success(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        i;
    int32        ExtraSubscribes = 0;

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    memset(RtTblPtr, 0, sizeof(RtTblPtr));

    HK_Test_InitGoodCopyTable(CopyTblPtr);

    /* Act */
    ReturnValue = HK_ProcessNewCopyTable(CopyTblPtr, RtTblPtr);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, CFE_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);

    UtAssert_INT32_EQ(RtTblPtr[0].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[1].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[2].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[3].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[4].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);

    for (i = 5; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        if (RtTblPtr[i].InputMidSubscribed == HK_INPUTMID_SUBSCRIBED)
        {
            ExtraSubscribes++;
        }
    }
    UtAssert_INT32_EQ(ExtraSubscribes, 0);
}

/*
 * Function under test: HK_ProcessNewCopyTable
 *
 * Case: Tests the case where the HK_ProcessNewCopyTable succeeds.
 *       Note that this function has multiple branches that lead to
 *       success. In this case, one of the populated Copy Table entries
 *       has an undefined InputMid and a defined OutputMid (forces
 *       a different logical branch than the previous success case).
 */
void Test_HK_ProcessNewCopyTable_Success2(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        i;
    int32        ExtraSubscribes = 0;

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    memset(RtTblPtr, 0, sizeof(RtTblPtr));

    HK_Test_InitGoodCopyTable(CopyTblPtr);

    CopyTblPtr[1].InputMid = CFE_SB_INVALID_MSG_ID;

    /* Act */
    ReturnValue = HK_ProcessNewCopyTable(CopyTblPtr, RtTblPtr);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, CFE_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);

    UtAssert_INT32_EQ(RtTblPtr[0].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[1].InputMidSubscribed, HK_INPUTMID_NOT_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[2].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[3].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[4].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);

    for (i = 5; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        if (RtTblPtr[i].InputMidSubscribed == HK_INPUTMID_SUBSCRIBED)
        {
            ExtraSubscribes++;
        }
    }

    UtAssert_INT32_EQ(ExtraSubscribes, 0);
}

/*
 * Function under test: HK_ProcessNewCopyTable
 *
 * Case: Tests the case in which one populated copy table entry
 *       has a size of 0.
 */
void Test_HK_ProcessNewCopyTable_PacketSizeZero(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        i;
    int32        ExtraSubscribes = 0;

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    memset(RtTblPtr, 0, sizeof(RtTblPtr));

    HK_Test_InitGoodCopyTable(CopyTblPtr);

    CopyTblPtr[1].OutputOffset = 0;
    CopyTblPtr[1].NumBytes     = 0;

    /* Act */
    ReturnValue = HK_ProcessNewCopyTable(CopyTblPtr, RtTblPtr);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, CFE_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);

    UtAssert_INT32_EQ(RtTblPtr[0].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[1].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[2].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[3].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[4].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);

    for (i = 5; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        if (RtTblPtr[i].InputMidSubscribed == HK_INPUTMID_SUBSCRIBED)
        {
            ExtraSubscribes++;
        }
    }
    UtAssert_INT32_EQ(ExtraSubscribes, 0);
}

/*
 * Function under test: HK_ProcessNewCopyTable
 *
 * Case: Tests the case in which all populated copy table entries
 *       have a size of 0.
 */
void Test_HK_ProcessNewCopyTable_AllPacketsSizeZero(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        i;
    int32        ExtraSubscribes = 0;

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    memset(RtTblPtr, 0, sizeof(RtTblPtr));

    HK_Test_InitGoodCopyTable(CopyTblPtr);

    for (i = 0; i < 5; i++)
    {
        CopyTblPtr[i].OutputOffset = 0;
        CopyTblPtr[i].NumBytes     = 0;
    }

    /* Act */
    ReturnValue = HK_ProcessNewCopyTable(CopyTblPtr, RtTblPtr);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, CFE_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);

    UtAssert_INT32_EQ(RtTblPtr[0].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[1].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[2].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[3].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);
    UtAssert_INT32_EQ(RtTblPtr[4].InputMidSubscribed, HK_INPUTMID_SUBSCRIBED);

    for (i = 5; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        if (RtTblPtr[i].InputMidSubscribed == HK_INPUTMID_SUBSCRIBED)
        {
            ExtraSubscribes++;
        }
    }

    UtAssert_INT32_EQ(ExtraSubscribes, 0);
}

/*
 * Function under test: HK_ProcessNewCopyTable
 *
 * Case: Tests the case in which the copy table has no populated
 *       entries.
 */
void Test_HK_ProcessNewCopyTable_EmptyTable(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        i;
    int32        SubscriptionCount = 0;

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    memset(RtTblPtr, 0, sizeof(RtTblPtr));

    HK_Test_InitEmptyCopyTable(CopyTblPtr);

    /* Act */
    ReturnValue = HK_ProcessNewCopyTable(CopyTblPtr, RtTblPtr);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, CFE_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);

    for (i = 0; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        if (RtTblPtr[i].InputMidSubscribed == HK_INPUTMID_SUBSCRIBED)
        {
            SubscriptionCount++;
        }
    }

    UtAssert_INT32_EQ(SubscriptionCount, 0);
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_TearDownOldCopyTable                         */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_TearDownCopyTable
 *
 * Case: Tests the case where the provided Copy Table Pointer is NULL.
 */
void Test_HK_TearDownOldCopyTable_NullCpyTbl(void)
{
    /* Arrange */
    hk_runtime_tbl_entry_t RtTblPtr;
    int32                  strCmpResult;
    CFE_Status_t           ReturnValue;

    char ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Null pointer detected in copy tbl tear down: CpyTbl = %%p, RtTbl = %%p");

    /* Act */
    ReturnValue = HK_TearDownOldCopyTable(NULL, &RtTblPtr);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_NULL_POINTER_DETECTED);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_NULL_POINTER_TEARCPY_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_TearDownCopyTable
 *
 * Case: Tests the case where the provided Runtime Table Pointer is NULL.
 */
void Test_HK_TearDownOldCopyTable_NullRtTbl(void)
{
    /* Arrange */
    hk_copy_table_entry_t CpyTblPtr;
    int32                 strCmpResult;
    CFE_Status_t          ReturnValue;

    char ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Null pointer detected in copy tbl tear down: CpyTbl = %%p, RtTbl = %%p");

    /* Act */
    ReturnValue = HK_TearDownOldCopyTable(&CpyTblPtr, NULL);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_NULL_POINTER_DETECTED);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_NULL_POINTER_TEARCPY_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_TearDownCopyTable
 *
 * Case: Tests the case where the call to CFE_ES_PutPoolBuf fails.
 */
void Test_HK_TearDownOldCopyTable_PoolFreeFail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        i;
    int          strCmpResult;

    char ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "HK TearDown: ES_putPoolBuf Err pkt:0x%%08lX ret 0x%%04X, hdl 0x%%08lx");

    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_PutPoolBuf), (CFE_SUCCESS - 1));

    HK_Test_InitGoodCopyTable(CopyTblPtr);
    HK_Test_InitGoodRuntimeTable(RtTblPtr);

    /* Act */
    ReturnValue = HK_TearDownOldCopyTable(CopyTblPtr, RtTblPtr);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, CFE_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 5);

    for (i = 0; i < 5; i++)
    {
        UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[i].EventID, HK_MEM_POOL_FREE_FAILED_EID);

        UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[i].EventType, CFE_EVS_EventType_ERROR);

        strCmpResult =
            strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[i].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

        UtAssert_True(strCmpResult == 0, "Event #%d string matched expected result, '%s'", (int)i,
                      context_CFE_EVS_SendEvent[i].Spec);
    }
}

/*
 * Function under test: HK_TearDownOldCopyTable
 *
 * Case: Tests the case where the HK_TearDownOldCopyTable succeeds.
 *       Note that this function has multiple branches that lead to
 *       success. In this case, all of the populated Runtime Table
 *       entries have the same OutputPktAddr.
 */
void Test_HK_TearDownOldCopyTable_Success(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        i;

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    HK_Test_InitGoodCopyTable(CopyTblPtr);
    HK_Test_InitGoodRuntimeTable(RtTblPtr);

    /* Act */
    ReturnValue = HK_TearDownOldCopyTable(CopyTblPtr, RtTblPtr);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, CFE_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);

    for (i = 0; i < 5; i++)
    {
        UtAssert_True(RtTblPtr[i].OutputPktAddr == NULL, "RtTblPtr[%d].OutputPktAddr == NULL", (int)i);
        UtAssert_INT32_EQ(RtTblPtr[i].InputMidSubscribed, HK_INPUTMID_NOT_SUBSCRIBED);
    }
}

/*
 * Function under test: HK_TearDownOldCopyTable
 *
 * Case: Tests the case where the provided Copy Table and
 *       Runtime Table have no populated entries.
 */
void Test_HK_TearDownOldCopyTable_EmptyTable(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        i;
    int32        SubscriptionCount = 0;

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    HK_Test_InitEmptyCopyTable(CopyTblPtr);
    HK_Test_InitEmptyRuntimeTable(RtTblPtr);

    /* Act */
    ReturnValue = HK_TearDownOldCopyTable(CopyTblPtr, RtTblPtr);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, CFE_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);

    for (i = 0; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        if (RtTblPtr[i].InputMidSubscribed == HK_INPUTMID_SUBSCRIBED)
        {
            SubscriptionCount++;
        }
    }

    UtAssert_INT32_EQ(SubscriptionCount, 0);
}

/*
 * Function under test: HK_TearDownOldCopyTable
 *
 * Case: Tests the case where the HK_TearDownOldCopyTable succeeds.
 *       Note that this function has multiple branches that lead to
 *       success. In this case, one of the populated Runtime Table
 *       entries has a different OutputPktAddr than the other
 *       populated entries (forcing a different logical path).
 */
void Test_HK_TearDownOldCopyTable_Success2(void)
{
    /* Arrange */
    CFE_Status_t           ReturnValue;
    int32                  i;
    CFE_SB_Buffer_t        Buffer;
    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    HK_Test_InitGoodCopyTable(CopyTblPtr);
    HK_Test_InitGoodRuntimeTable(RtTblPtr);

    RtTblPtr[4].OutputPktAddr = &Buffer; /* Just needs to not match the
                                      other filled entries */

    /* Act */
    ReturnValue = HK_TearDownOldCopyTable(CopyTblPtr, RtTblPtr);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, CFE_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);

    for (i = 0; i < 5; i++)
    {
        UtAssert_True(RtTblPtr[i].OutputPktAddr == NULL, "RtTblPtr[%d].OutputPktAddr == NULL", (int)i);
        UtAssert_INT32_EQ(RtTblPtr[i].InputMidSubscribed, HK_INPUTMID_NOT_SUBSCRIBED);
    }
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_SendCombinedHkPacket                         */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_SendCombinedHkPacket
 *
 * Case: Tests the case where the HK_SendCombinedHkPacket function
 *       does not see any missing data.
 */
void Test_HK_SendCombinedHkPacket_NoMissingData(void)
{
    /* Arrange */
    int32          call_count_CFE_SB_TimeStampMsg;
    int32          call_count_CFE_SB_TransmitMsg;
    CFE_SB_MsgId_t SendMid = CFE_SB_ValueToMsgId(HK_COMBINED_PKT1_MID); /* MID in the table */
    int32          i;

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    HK_Test_InitGoodCopyTable(CopyTblPtr);
    HK_Test_InitGoodRuntimeTable(RtTblPtr);

    for (i = 0; i < 5; i++)
    {
        RtTblPtr[i].DataPresent = HK_DATA_PRESENT;
    }

    HK_AppData.CopyTablePtr    = CopyTblPtr;
    HK_AppData.RuntimeTablePtr = RtTblPtr;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &SendMid, sizeof(SendMid), false);

    /* Act */
    HK_SendCombinedHkPacket(SendMid);

    call_count_CFE_EVS_SendEvent   = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    call_count_CFE_SB_TimeStampMsg = UT_GetStubCount(UT_KEY(CFE_SB_TimeStampMsg));
    call_count_CFE_SB_TransmitMsg  = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));

    /* Assert */
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(HK_AppData.MissingDataCtr, 0);
    UtAssert_INT32_EQ(call_count_CFE_SB_TimeStampMsg, 1);
    UtAssert_INT32_EQ(call_count_CFE_SB_TransmitMsg, 1);
    UtAssert_INT32_EQ(HK_AppData.CombinedPacketsSent, 1);
}

/*
 * Function under test: HK_SendCombinedHkPacket
 *
 * Case: Tests the case where the HK_SendCombinedHkPacket function
 *       does see missing data.
 */
void Test_HK_SendCombinedHkPacket_MissingData(void)
{
    /* Arrange */
    CFE_SB_MsgId_t SendMid = CFE_SB_ValueToMsgId(HK_COMBINED_PKT1_MID); /* MID in the table */

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    HK_Test_InitGoodCopyTable(CopyTblPtr);
    HK_Test_InitGoodRuntimeTable(RtTblPtr);

    HK_AppData.CopyTablePtr    = CopyTblPtr;
    HK_AppData.RuntimeTablePtr = RtTblPtr;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &SendMid, sizeof(SendMid), false);

    /* Act */
    HK_SendCombinedHkPacket(SendMid);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(HK_AppData.MissingDataCtr, 1);

#if HK_DISCARD_INCOMPLETE_COMBO == 0
    int32 call_count_CFE_SB_TimeStampMsg = UT_GetStubCount(UT_KEY(CFE_SB_TimeStampMsg));
    int32 call_count_CFE_SB_TransmitMsg  = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));

    UtAssert_INT32_EQ(call_count_CFE_SB_TimeStampMsg, 1);
    UtAssert_INT32_EQ(call_count_CFE_SB_TransmitMsg, 1);
    UtAssert_INT32_EQ(HK_AppData.CombinedPacketsSent, 1);
#endif
}

/*
 * Function under test: HK_SendCombinedHkPacket
 *
 * Case: Tests the case where the HK_SendCombinedHkPacket function
 *       is called with both the Copy Table and Runtime Table empty.
 */
void Test_HK_SendCombinedHkPacket_EmptyTable(void)
{
    /* Arrange */
    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    HK_Test_InitEmptyCopyTable(CopyTblPtr);
    HK_Test_InitEmptyRuntimeTable(RtTblPtr);
    HK_AppData.RuntimeTablePtr = RtTblPtr;
    HK_AppData.CopyTablePtr    = CopyTblPtr;

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Combined HK Packet 0x%%08lX is not found in current HK Copy Table");

    /* Act */
    HK_SendCombinedHkPacket(CFE_SB_ValueToMsgId(CFE_EVS_HK_TLM_MID));

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_UNKNOWN_COMBINED_PACKET_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_SendCombinedHkPacket
 *
 * Case: Tests the case where the HK_SendCombinedHkPacket function
 *       is called for a message that is not present in the Copy
 *       Table.
 */
void Test_HK_SendCombinedHkPacket_PacketNotFound(void)
{
    /* Arrange */
    CFE_SB_MsgId_t  forced_MsgID = HK_UT_MID_100; /* does not match provided parameter */
    int32           strCmpResult;
    char            ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    CFE_SB_Buffer_t Buffer;

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    HK_Test_InitEmptyCopyTable(CopyTblPtr);
    HK_Test_InitEmptyRuntimeTable(RtTblPtr);

    /* populate one entry in the RT table */
    RtTblPtr[1].OutputPktAddr = &Buffer; /* just needs to be non-null */

    HK_AppData.RuntimeTablePtr = RtTblPtr;
    HK_AppData.CopyTablePtr    = CopyTblPtr;

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Combined HK Packet 0x%%08lX is not found in current HK Copy Table");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &forced_MsgID, sizeof(forced_MsgID), false);

    /* Act */
    HK_SendCombinedHkPacket(CFE_SB_ValueToMsgId(CFE_EVS_HK_TLM_MID));

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_UNKNOWN_COMBINED_PACKET_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_CheckStatusOfTables                          */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_CheckStatusOfTables
 *
 * Case: Tests the case where the HK_CheckStatusOfTables function
 *       succeeds.
 */
void Test_HK_CheckStatusOfTables_AllSuccess(void)
{
    /* Arrange */
    int32 ReturnValue;
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), CFE_SUCCESS);

    /* Act */
    ReturnValue = HK_CheckStatusOfTables();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
}

/*
 * Function under test: HK_CheckStatusOfTables
 *
 * Case: Tests the case in which the Copy Table fails its status check.
 */
void Test_HK_CheckStatusOfTables_CpyTblFail(void)
{
    /* Arrange */
    int32 ReturnValue;
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), !CFE_SUCCESS);

    /* Act */
    ReturnValue = HK_CheckStatusOfTables();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_ERROR);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
}

/*
 * Function under test: HK_CheckStatusOfTables
 *
 * Case: Tests the case in which the Runtime Table fails its status check.
 */
void Test_HK_CheckStatusOfTables_RtTblFail(void)
{
    /* Arrange */
    int32 ReturnValue;
    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_GetStatus), 2, !CFE_SUCCESS); /* copy tbl will pass, rt tbl will fail */

    /* Act */
    ReturnValue = HK_CheckStatusOfTables();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_ERROR);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_CheckStatusOfCopyTable                       */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_CheckStatusOfCopyTable
 *
 * Case: Tests the case in which the status of the copy table is
 *       CFE_SUCCESS.
 */
void Test_HK_CheckStatusOfCopyTable_TblSuccess(void)
{
    /* Arrange */
    int32 ReturnValue;
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), CFE_SUCCESS);

    /* Act */
    ReturnValue = HK_CheckStatusOfCopyTable();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
}

/*
 * Function under test: HK_CheckStatusOfCopyTable
 *
 * Case: Tests the case in which the call to CFE_TBL_GetStatus fails.
 */
void Test_HK_CheckStatusOfCopyTable_TblStatFail(void)
{
    /* Arrange */
    int32 ReturnValue;
    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Unexpected CFE_TBL_GetStatus return (0x%%08X) for Copy Table");

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), !CFE_SUCCESS);

    /* Act */
    ReturnValue = HK_CheckStatusOfCopyTable();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_ERROR);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_UNEXPECTED_GETSTAT_RET_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_CRITICAL);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_CheckStatusOfCopyTable
 *
 * Case: Tests the case in which the call to CFE_TBL_GetStatus returns
 *       CFE_TBL_INFO_VALIDATION_PENDING, and the subsequent validation
 *       succeeds.
 */
void Test_HK_CheckStatusOfCopyTable_TblValPendingSuccess(void)
{
    /* Arrange */
    int32 ReturnValue;
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), CFE_TBL_INFO_VALIDATION_PENDING);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Validate), CFE_SUCCESS);

    /* Act */
    ReturnValue = HK_CheckStatusOfCopyTable();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_SUCCESS);
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
}

/*
 * Function under test: HK_CheckStatusOfCopyTable
 *
 * Case: Tests the case in which the call to CFE_TBL_GetStatus returns
 *       CFE_TBL_INFO_VALIDATION_PENDING, and the subsequent validation
 *       fails.
 */
void Test_HK_CheckStatusOfCopyTable_TblValPendingFail(void)
{
    /* Arrange */
    int32 ReturnValue;
    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Unexpected CFE_TBL_Validate return (0x%%08X) for Copy Table");

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), CFE_TBL_INFO_VALIDATION_PENDING);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Validate), !CFE_SUCCESS);

    /* Act */
    ReturnValue = HK_CheckStatusOfCopyTable();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_UNEXPECTED_TBLVLD_RET_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_CheckStatusOfCopyTable
 *
 * Case: Tests the case in which the call to CFE_TBL_GetStatus returns
 *       CFE_TBL_INFO_UPDATE_PENDING, and the subsequent call to
 *       CFE_TBL_ReleaseAddress fails.
 */
void Test_HK_CheckStatusOfCopyTable_TblInfoUpReleaseFail(void)
{
    /* Arrange */
    int32 ReturnValue;
    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Unexpected CFE_TBL_ReleaseAddress return (0x%%08X) for Copy Table");

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), CFE_TBL_INFO_UPDATE_PENDING);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_ReleaseAddress), !CFE_SUCCESS);

    /* Act */
    ReturnValue = HK_CheckStatusOfCopyTable();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_ERROR);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 2);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, HK_UNEXPECTED_RELADDR_RET_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_CRITICAL);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[1].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[1].Spec);
}

/*
 * Function under test: HK_CheckStatusOfCopyTable
 *
 * Case: Tests the case in which the call to CFE_TBL_GetStatus returns
 *       CFE_TBL_INFO_UPDATE_PENDING, and the subsequent call to
 *       CFE_TBL_Update fails.
 */
void Test_HK_CheckStatusOfCopyTable_TblInfoUpUpdateFail(void)
{
    /* Arrange */
    int32 ReturnValue;
    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Unexpected CFE_TBL_Update return (0x%%08X) for Copy Table");

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), CFE_TBL_INFO_UPDATE_PENDING);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_ReleaseAddress), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Update), !CFE_SUCCESS);

    /* Act */
    ReturnValue = HK_CheckStatusOfCopyTable();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_ERROR);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 2);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, HK_UNEXPECTED_TBLUPD_RET_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_CRITICAL);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[1].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[1].Spec);
}

/*
 * Function under test: HK_CheckStatusOfCopyTable
 *
 * Case: Tests the case in which the call to CFE_TBL_GetStatus returns
 *       CFE_TBL_INFO_UPDATE_PENDING, and the subsequent call to
 *       CFE_TBL_GetAddress fails.
 */
void Test_HK_CheckStatusOfCopyTable_TblInfoUpGetAddrFail(void)
{
    /* Arrange */
    int32 ReturnValue;
    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Unexpected CFE_TBL_GetAddress return (0x%%08X) for Copy Table");

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), CFE_TBL_INFO_UPDATE_PENDING);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_ReleaseAddress), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Update), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), !CFE_TBL_INFO_UPDATED);

    /* Act */
    ReturnValue = HK_CheckStatusOfCopyTable();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_ERROR);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 2);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, HK_UNEXPECTED_GETADDR_RET_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_CRITICAL);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[1].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[1].Spec);
}

/*
 * Function under test: HK_CheckStatusOfCopyTable
 *
 * Case: Tests the case in which the call to CFE_TBL_GetStatus returns
 *       CFE_TBL_INFO_UPDATE_PENDING, and the subsequent processing of
 *       the update fails.
 */
void Test_HK_CheckStatusOfCopyTable_TblInfoUpProcessFail(void)
{
    /* Arrange */
    int32 ReturnValue;
    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Process New Copy Table Failed, status = 0x%%08X");

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), CFE_TBL_INFO_UPDATE_PENDING);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_ReleaseAddress), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Update), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_TBL_INFO_UPDATED);

    /* Act */
    ReturnValue = HK_CheckStatusOfCopyTable();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_ERROR);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 3);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventID, HK_NEWCPYTBL_HK_FAILED_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventType, CFE_EVS_EventType_CRITICAL);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[2].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[2].Spec);
}

/*
 * Function under test: HK_CheckStatusOfCopyTable
 *
 * Case: Tests the case in which the call to CFE_TBL_GetStatus returns
 *       CFE_TBL_INFO_UPDATE_PENDING, and the subsequent processing of
 *       the update succeeds.
 */
void Test_HK_CheckStatusOfCopyTable_TblInfoUpProcessSuccess(void)
{
    /* Arrange */
    int32                  ReturnValue;
    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    HK_Test_InitGoodCopyTable(CopyTblPtr);
    HK_Test_InitGoodRuntimeTable(RtTblPtr);

    HK_AppData.CopyTablePtr    = CopyTblPtr;
    HK_AppData.RuntimeTablePtr = RtTblPtr;

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), CFE_TBL_INFO_UPDATE_PENDING);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_ReleaseAddress), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Update), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_TBL_INFO_UPDATED);
    UT_SetHandlerFunction(UT_KEY(CFE_TBL_GetAddress), UT_CFE_TBL_GetAddress_Handler, NULL);

    /* Act */
    ReturnValue = HK_CheckStatusOfCopyTable();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_SUCCESS);
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_CheckStatusOfDumpTable                       */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_CheckStatusOfDumpTable
 *
 * Case: Tests the case in which the call to CFE_TBL_GetStatus returns
 *       CFE_SUCCESS.
 */
void Test_HK_CheckStatusOfDumpTable_TblStatSuccess(void)
{
    /* Arrange */
    int32 ReturnValue;
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), CFE_SUCCESS);

    /* Act */
    ReturnValue = HK_CheckStatusOfDumpTable();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
}

/*
 * Function under test: HK_CheckStatusOfDumpTable
 *
 * Case: Tests the case in which the call to CFE_TBL_GetStatus returns
 *       an error.
 */
void Test_HK_CheckStatusOfDumpTable_TblStatFail(void)
{
    /* Arrange */
    int32 ReturnValue;
    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Unexpected CFE_TBL_GetStatus return (0x%%08X) for Runtime Table");

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), !CFE_SUCCESS);

    /* Act */
    ReturnValue = HK_CheckStatusOfDumpTable();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_ERROR);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_UNEXPECTED_GETSTAT2_RET_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_CRITICAL);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_CheckStatusOfDumpTable
 *
 * Case: Tests the case in which the call to CFE_TBL_GetStatus returns
 *       CFE_TBL_INFO_DUMP_PENDING and the subsequent call to
 *       CFE_TBL_DumpToBuffer succeeds.
 */
void Test_HK_CheckStatusOfDumpTable_TblStatUpDumpSuccess(void)
{
    /* Arrange */
    int32 ReturnValue;

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), CFE_TBL_INFO_DUMP_PENDING);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_DumpToBuffer), CFE_SUCCESS);

    /* Act */
    ReturnValue = HK_CheckStatusOfDumpTable();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
}

/*
 * Function under test: HK_CheckStatusOfDumpTable
 *
 * Case: Tests the case in which the call to CFE_TBL_GetStatus returns
 *       CFE_TBL_INFO_DUMP_PENDING and the subsequent call to
 *       CFE_TBL_DumpToBuffer fails.
 */
void Test_HK_CheckStatusOfDumpTable_TblStatUpDumpFail(void)
{
    /* Arrange */
    int32 ReturnValue;
    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Unexpected CFE_TBL_DumpToBuffer return (0x%%08X) for Runtime Table");

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), CFE_TBL_INFO_DUMP_PENDING);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_DumpToBuffer), !CFE_SUCCESS);

    /* Act */
    ReturnValue = HK_CheckStatusOfDumpTable();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, HK_ERROR);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_UNEXPECTED_DUMPTOBUFFER_RET_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_CRITICAL);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_CheckForMissingData                          */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_CheckForMissingData
 *
 * Case: Tests the case in which missing data is detected because
 *       table entries matching the output MID do not have data present.
 */
void Test_HK_CheckForMissingData_MissingData(void)
{
    /* Arrange */
    int32          ReturnValue;
    CFE_SB_MsgId_t OutPktToCheck   = CFE_SB_ValueToMsgId(HK_COMBINED_PKT1_MID); /* MID in the table */
    CFE_SB_MsgId_t MissingInputMid = CFE_SB_INVALID_MSG_ID;

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    HK_Test_InitGoodCopyTable(CopyTblPtr);
    HK_Test_InitGoodRuntimeTable(RtTblPtr);

    HK_AppData.CopyTablePtr    = CopyTblPtr;
    HK_AppData.RuntimeTablePtr = RtTblPtr;

    /* Act */
    ReturnValue = HK_CheckForMissingData(OutPktToCheck, &MissingInputMid);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(ReturnValue, HK_MISSING_DATA_DETECTED);

    /* Check MissingInputMid is set correctly */
    UtAssert_INT32_EQ(CFE_SB_MsgIdToValue(MissingInputMid), CFE_EVS_HK_TLM_MID);
}

/*
 * Function under test: HK_CheckForMissingData
 *
 * Case: Tests the case in which no missing data is detected because
 *       all entries in the Runtime Table are empty.
 */
void Test_HK_CheckForMissingData_NoMissingData_AddrNull(void)
{
    /* Arrange */
    int32          ReturnValue;
    CFE_SB_MsgId_t OutPktToCheck   = CFE_SB_ValueToMsgId(HK_COMBINED_PKT1_MID);
    CFE_SB_MsgId_t MissingInputMid = CFE_SB_INVALID_MSG_ID;

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    HK_Test_InitEmptyCopyTable(CopyTblPtr);
    HK_Test_InitEmptyRuntimeTable(RtTblPtr);

    HK_AppData.CopyTablePtr    = CopyTblPtr;
    HK_AppData.RuntimeTablePtr = RtTblPtr;

    /* Act */
    ReturnValue = HK_CheckForMissingData(OutPktToCheck, &MissingInputMid);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(ReturnValue, HK_NO_MISSING_DATA);

    /* check that the MissingInputMid did not change */
    UtAssert_BOOL_FALSE(CFE_SB_IsValidMsgId(MissingInputMid));
}

/*
 * Function under test: HK_CheckForMissingData
 *
 * Case: Tests the case in which no missing data is detected because
 *       Message ID to check is not present in the table.
 */
void Test_HK_CheckForMissingData_NoMissingData_OutputMidMismatch(void)
{
    /* Arrange */
    int32          ReturnValue;
    CFE_SB_MsgId_t OutPktToCheck   = CFE_SB_ValueToMsgId(HK_COMBINED_PKT2_MID); /* MID not in the copy table */
    CFE_SB_MsgId_t MissingInputMid = CFE_SB_INVALID_MSG_ID;

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    HK_Test_InitGoodCopyTable(CopyTblPtr);
    HK_Test_InitGoodRuntimeTable(RtTblPtr);

    HK_AppData.CopyTablePtr    = CopyTblPtr;
    HK_AppData.RuntimeTablePtr = RtTblPtr;

    /* Act */
    ReturnValue = HK_CheckForMissingData(OutPktToCheck, &MissingInputMid);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(ReturnValue, HK_NO_MISSING_DATA);

    /* check that the MissingInputMid did not change */
    UtAssert_BOOL_FALSE(CFE_SB_IsValidMsgId(MissingInputMid));
}

/*
 * Function under test: HK_CheckForMissingData
 *
 * Case: Tests the case in which no missing data is detected because
 *       table entries matching the Message ID have data present.
 */
void Test_HK_CheckForMissingData_NoMissingData_DataPresent(void)
{
    /* Arrange */
    int32          ReturnValue;
    CFE_SB_MsgId_t OutPktToCheck   = CFE_SB_ValueToMsgId(HK_COMBINED_PKT1_MID); /* MID in the table */
    CFE_SB_MsgId_t MissingInputMid = CFE_SB_INVALID_MSG_ID;
    int32          i;

    hk_runtime_tbl_entry_t RtTblPtr[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CopyTblPtr[HK_COPY_TABLE_ENTRIES];

    HK_Test_InitGoodCopyTable(CopyTblPtr);
    HK_Test_InitGoodRuntimeTable(RtTblPtr);

    for (i = 0; i < 5; i++)
    {
        RtTblPtr[i].DataPresent = HK_DATA_PRESENT;
    }

    HK_AppData.CopyTablePtr    = CopyTblPtr;
    HK_AppData.RuntimeTablePtr = RtTblPtr;

    /* Act */
    ReturnValue = HK_CheckForMissingData(OutPktToCheck, &MissingInputMid);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(ReturnValue, HK_NO_MISSING_DATA);

    /* check that the MissingInputMid did not change */
    UtAssert_BOOL_FALSE(CFE_SB_IsValidMsgId(MissingInputMid));
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_SetFlagsToNotPresent                         */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_SetFlagsToNotPresent
 *
 * Case: Tests the nominal case of the HK_SetFlagsToNotPresent function.
 */
void Test_HK_SetFlagsToNotPresent(void)
{
    /* Arrange */
    int32                  EntriesWithDataPresent = 0;
    hk_runtime_tbl_entry_t RtTbl[HK_COPY_TABLE_ENTRIES];
    hk_copy_table_entry_t  CpyTbl[HK_COPY_TABLE_ENTRIES];
    CFE_SB_Buffer_t        Buffer;
    int32                  i;
    for (i = 0; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        RtTbl[i].OutputPktAddr = &Buffer; /* just needs to be non-null */
        CpyTbl[i].OutputMid    = CFE_SB_ValueToMsgId(CFE_EVS_HK_TLM_MID);
        RtTbl[i].DataPresent   = HK_DATA_PRESENT;
    }

    /* make it so that exactly one entry shouldn't have flags cleared */
    CpyTbl[0].OutputMid = HK_UT_MID_100;

    HK_AppData.CopyTablePtr    = CpyTbl;
    HK_AppData.RuntimeTablePtr = RtTbl;

    /* Act */
    HK_SetFlagsToNotPresent(CFE_SB_ValueToMsgId(CFE_EVS_HK_TLM_MID));

    /* Assert */
    for (i = 0; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        if (RtTbl[i].DataPresent == HK_DATA_PRESENT)
        {
            EntriesWithDataPresent++;
        }
    }

    UtAssert_INT32_EQ(EntriesWithDataPresent, 1);
}

/****************************************************************************/

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    /* Boiler Plate */

    /* Test functions for HK_ProcessIncomingHkData */
    UtTest_Add(Test_HK_ProcessIncomingHkData_MidNotFound, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_ProcessIncomingHkData_MidNotFound");
    UtTest_Add(Test_HK_ProcessIncomingHkData_LengthOkEqual, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_ProcessIncomingHkData_LengthOkEqual");
    UtTest_Add(Test_HK_ProcessIncomingHkData_LengthOkGreater, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_ProcessIncomingHkData_LengthOkGreater");
    UtTest_Add(Test_HK_ProcessIncomingHkData_MessageError, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_ProcessIncomingHkData_MessageError");

    /* Test functions for HK_ValidateHkCopyTable */
    UtTest_Add(Test_HK_ValidateHkCopyTable_Success, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_ValidateHkCopyTable_Success");
    UtTest_Add(Test_HK_ValidateHkCopyTable_Error, HK_Test_Setup, HK_Test_TearDown, "Test_HK_ValidateHkCopyTable_Error");

    /* Test functions for HK_ProcessNewCopyTable */
    UtTest_Add(Test_HK_ProcessNewCopyTable_EmptyTable, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_ProcessNewCopyTable_EmptyTable");
    UtTest_Add(Test_HK_ProcessNewCopyTable_NullCpyTbl, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_ProcessNewCopyTable_NullCpyTbl");
    UtTest_Add(Test_HK_ProcessNewCopyTable_NullRtTbl, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_ProcessNewCopyTable_NullRtTbl");
    UtTest_Add(Test_HK_ProcessNewCopyTable_PoolBufFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_ProcessNewCopyTable_PoolBufFail");
    UtTest_Add(Test_HK_ProcessNewCopyTable_SubscribeFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_ProcessNewCopyTable_SubscribeFail");
    UtTest_Add(Test_HK_ProcessNewCopyTable_Success, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_ProcessNewCopyTable_Success");
    UtTest_Add(Test_HK_ProcessNewCopyTable_Success2, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_ProcessNewCopyTable_Success2");
    UtTest_Add(Test_HK_ProcessNewCopyTable_PacketSizeZero, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_ProcessNewCopyTable_PacketSizeZero");
    UtTest_Add(Test_HK_ProcessNewCopyTable_AllPacketsSizeZero, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_ProcessNewCopyTable_AllPacketsSizeZero");

    /* Test functions for HK_TearDownOldCopyTable */
    UtTest_Add(Test_HK_TearDownOldCopyTable_NullCpyTbl, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_TearDownOldCopyTable_NullCpyTbl");
    UtTest_Add(Test_HK_TearDownOldCopyTable_NullRtTbl, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_TearDownOldCopyTable_NullRtTbl");
    UtTest_Add(Test_HK_TearDownOldCopyTable_PoolFreeFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_TearDownOldCopyTable_PoolFreeFail");
    UtTest_Add(Test_HK_TearDownOldCopyTable_Success, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_TearDownOldCopyTable_Success");
    UtTest_Add(Test_HK_TearDownOldCopyTable_EmptyTable, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_TearDownOldCopyTable_EmptyTable");
    UtTest_Add(Test_HK_TearDownOldCopyTable_Success2, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_TearDownOldCopyTable_Success2");

    /* Test functions for HK_SendCombinedHkPacket */
    UtTest_Add(Test_HK_SendCombinedHkPacket_NoMissingData, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_SendCombinedHkPacket_NoMissingData");
    UtTest_Add(Test_HK_SendCombinedHkPacket_MissingData, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_SendCombinedHkPacket_MissingData");
    UtTest_Add(Test_HK_SendCombinedHkPacket_EmptyTable, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_SendCombinedHkPacket_EmptyTable");
    UtTest_Add(Test_HK_SendCombinedHkPacket_PacketNotFound, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_SendCombinedHkPacket_PacketNotFound");

    /* Test functions for HK_CheckStatusOfTables */
    UtTest_Add(Test_HK_CheckStatusOfTables_AllSuccess, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckStatusOfTables_AllSuccess");
    UtTest_Add(Test_HK_CheckStatusOfTables_CpyTblFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckStatusOfTables_CpyTblFail");
    UtTest_Add(Test_HK_CheckStatusOfTables_RtTblFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckStatusOfTables_RtTblFail");

    /* Test functions for HK_CheckStatusOfCopyTable */
    UtTest_Add(Test_HK_CheckStatusOfCopyTable_TblSuccess, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckStatusOfCopyTable_TblSuccess");
    UtTest_Add(Test_HK_CheckStatusOfCopyTable_TblStatFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckStatusOfCopyTable_TblStatFail");
    UtTest_Add(Test_HK_CheckStatusOfCopyTable_TblValPendingSuccess, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckStatusOfCopyTable_TblValPendingSuccess");
    UtTest_Add(Test_HK_CheckStatusOfCopyTable_TblValPendingFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckStatusOfCopyTable_TblValPendingFail");
    UtTest_Add(Test_HK_CheckStatusOfCopyTable_TblInfoUpReleaseFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckStatusOfCopyTable_TblInfoUpReleaseFail");
    UtTest_Add(Test_HK_CheckStatusOfCopyTable_TblInfoUpUpdateFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckStatusOfCopyTable_TblInfoUpUpdateFail");
    UtTest_Add(Test_HK_CheckStatusOfCopyTable_TblInfoUpGetAddrFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckStatusOfCopyTable_TblInfoUpGetAddrFail");
    UtTest_Add(Test_HK_CheckStatusOfCopyTable_TblInfoUpProcessFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckStatusOfCopyTable_TblInfoUpProcessFail");
    UtTest_Add(Test_HK_CheckStatusOfCopyTable_TblInfoUpProcessSuccess, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckStatusOfCopyTable_TblInfoUpProcessSuccess");

    /* Test functions for HK_CheckStatusOfDumpTable */
    UtTest_Add(Test_HK_CheckStatusOfDumpTable_TblStatSuccess, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckStatusOfDumpTable_TblStatSuccess");
    UtTest_Add(Test_HK_CheckStatusOfDumpTable_TblStatFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckStatusOfDumpTable_TblStatFail");
    UtTest_Add(Test_HK_CheckStatusOfDumpTable_TblStatUpDumpSuccess, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckStatusOfDumpTable_TblStatUpDumpSuccess");
    UtTest_Add(Test_HK_CheckStatusOfDumpTable_TblStatUpDumpFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckStatusOfDumpTable_TblStatUpDumpFail");

    /* Test functions for HK_CheckForMissingData */
    UtTest_Add(Test_HK_CheckForMissingData_MissingData, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckForMissingData_MissingData");
    UtTest_Add(Test_HK_CheckForMissingData_NoMissingData_AddrNull, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckForMissingData_NoMissingData_AddrNull");
    UtTest_Add(Test_HK_CheckForMissingData_NoMissingData_OutputMidMismatch, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckForMissingData_NoMissingData_OutputMidMismatch");
    UtTest_Add(Test_HK_CheckForMissingData_NoMissingData_DataPresent, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_CheckForMissingData_NoMissingData_DataPresent");

    /* Test functions for HK_SetFlagsToNotPresent */
    UtTest_Add(Test_HK_SetFlagsToNotPresent, HK_Test_Setup, HK_Test_TearDown, "Test_HK_SetFlagsToNotPresent");
}
