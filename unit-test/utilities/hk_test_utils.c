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
 *   Test utility implementations
 */
#include "cfe.h"
#include "cfe_msgids.h"
#include "hk_app.h"
#include "hk_msg.h"
#include "hk_events.h"
#include "hk_version.h"
#include "hk_msgids.h"
#include "hk_test_utils.h"

/* UT includes */
#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

#define UT_MAX_SENDEVENT_DEPTH 5
CFE_EVS_SendEvent_context_t    context_CFE_EVS_SendEvent[UT_MAX_SENDEVENT_DEPTH];
CFE_ES_WriteToSysLog_context_t context_CFE_ES_WriteToSysLog;

/*
 * Function Definitions
 */
void UT_Handler_CFE_EVS_SendEvent(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context, va_list va)
{
    uint16 CallCount;
    uint16 idx;

    CallCount = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    if (CallCount > (sizeof(context_CFE_EVS_SendEvent) / sizeof(context_CFE_EVS_SendEvent[0])))
    {
        UtAssert_Failed("CFE_EVS_SendEvent UT depth %u exceeded: %u, increase UT_MAX_SENDEVENT_DEPTH",
                        UT_MAX_SENDEVENT_DEPTH, CallCount);
    }
    else
    {
        idx                                      = CallCount - 1;
        context_CFE_EVS_SendEvent[idx].EventID   = UT_Hook_GetArgValueByName(Context, "EventID", uint16);
        context_CFE_EVS_SendEvent[idx].EventType = UT_Hook_GetArgValueByName(Context, "EventType", uint16);

        strncpy(context_CFE_EVS_SendEvent[idx].Spec, UT_Hook_GetArgValueByName(Context, "Spec", const char *),
                CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);
        context_CFE_EVS_SendEvent[idx].Spec[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH - 1] = '\0';
    }
}

void UT_Handler_CFE_ES_WriteToSysLog(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context, va_list va)
{
    strncpy(context_CFE_ES_WriteToSysLog.Spec, UT_Hook_GetArgValueByName(Context, "SpecStringPtr", const char *),
            CFE_MISSION_EVS_MAX_MESSAGE_LENGTH - 1);
    context_CFE_ES_WriteToSysLog.Spec[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH - 1] = '\0';
}

void HK_Test_InitGoodCopyTable(hk_copy_table_entry_t *CpyTbl)
{
    int32 i = 0;

    /* this setup matches the default HK table */
    CpyTbl[0].InputMid     = CFE_SB_ValueToMsgId(CFE_EVS_HK_TLM_MID);
    CpyTbl[0].InputOffset  = 12;
    CpyTbl[0].OutputMid    = CFE_SB_ValueToMsgId(HK_COMBINED_PKT1_MID);
    CpyTbl[0].OutputOffset = 12;
    CpyTbl[0].NumBytes     = 4;

    CpyTbl[1].InputMid     = CFE_SB_ValueToMsgId(CFE_TIME_HK_TLM_MID);
    CpyTbl[1].InputOffset  = 12;
    CpyTbl[1].OutputMid    = CFE_SB_ValueToMsgId(HK_COMBINED_PKT1_MID);
    CpyTbl[1].OutputOffset = 16;
    CpyTbl[1].NumBytes     = 4;

    CpyTbl[2].InputMid     = CFE_SB_ValueToMsgId(CFE_SB_HK_TLM_MID);
    CpyTbl[2].InputOffset  = 12;
    CpyTbl[2].OutputMid    = CFE_SB_ValueToMsgId(HK_COMBINED_PKT1_MID);
    CpyTbl[2].OutputOffset = 20;
    CpyTbl[2].NumBytes     = 4;

    CpyTbl[3].InputMid     = CFE_SB_ValueToMsgId(CFE_ES_HK_TLM_MID);
    CpyTbl[3].InputOffset  = 12;
    CpyTbl[3].OutputMid    = CFE_SB_ValueToMsgId(HK_COMBINED_PKT1_MID);
    CpyTbl[3].OutputOffset = 24;
    CpyTbl[3].NumBytes     = 4;

    CpyTbl[4].InputMid     = CFE_SB_ValueToMsgId(CFE_EVS_HK_TLM_MID);
    CpyTbl[4].InputOffset  = 12;
    CpyTbl[4].OutputMid    = CFE_SB_ValueToMsgId(HK_COMBINED_PKT1_MID);
    CpyTbl[4].OutputOffset = 28;
    CpyTbl[4].NumBytes     = 4;

    for (i = 5; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        CpyTbl[i].InputMid     = CFE_SB_INVALID_MSG_ID;
        CpyTbl[i].InputOffset  = 0;
        CpyTbl[i].OutputMid    = CFE_SB_INVALID_MSG_ID;
        CpyTbl[i].OutputOffset = 0;
        CpyTbl[i].NumBytes     = 0;
    }
}

void HK_Test_InitOverflowCopyTable(hk_copy_table_entry_t *CpyTbl)
{
    int32 i;
    int32 overflowBytes = (HK_MAX_COMBINED_PACKET_SIZE / HK_COPY_TABLE_ENTRIES) + 1;

    /* Set each table entry such that the combined size exceeds HK_MAX_COMBINED_PACKET_SIZE */
    for (i = 0; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        CpyTbl[i].InputMid     = CFE_SB_ValueToMsgId(CFE_EVS_HK_TLM_MID);
        CpyTbl[i].InputOffset  = 12;
        CpyTbl[i].OutputMid    = CFE_SB_ValueToMsgId(HK_COMBINED_PKT1_MID);
        CpyTbl[i].OutputOffset = ((i + 1) * overflowBytes);
        CpyTbl[i].NumBytes     = overflowBytes; /* Ensures overflow when summed across all entries */
    }
}

void HK_Test_InitEmptyCopyTable(hk_copy_table_entry_t *CpyTbl)
{
    int32 i = 0;

    for (i = 0; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        CpyTbl[i].InputMid     = CFE_SB_INVALID_MSG_ID;
        CpyTbl[i].InputOffset  = 0;
        CpyTbl[i].OutputMid    = CFE_SB_INVALID_MSG_ID;
        CpyTbl[i].OutputOffset = 0;
        CpyTbl[i].NumBytes     = 0;
    }
}

void HK_Test_InitGoodRuntimeTable(hk_runtime_tbl_entry_t *RtTbl)
{
    int32                   i = 0;
    CFE_SB_Buffer_t         Buffer;
    hk_runtime_tbl_entry_t *RtEntry = NULL;

    /* Loop thru the RunTime table initializing the fields */
    for (i = 0; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        RtEntry = &RtTbl[i];

        if (i < 5)
        {
            RtEntry->OutputPktAddr      = &Buffer; /* just needs to be non-null */
            RtEntry->InputMidSubscribed = HK_INPUTMID_SUBSCRIBED;
            RtEntry->DataPresent        = HK_DATA_NOT_PRESENT;
        }
        else
        {
            RtEntry->OutputPktAddr      = NULL;
            RtEntry->InputMidSubscribed = HK_INPUTMID_NOT_SUBSCRIBED;
            RtEntry->DataPresent        = HK_DATA_NOT_PRESENT;
        }
    }
}

void HK_Test_InitEmptyRuntimeTable(hk_runtime_tbl_entry_t *RtTbl)
{
    int32 i = 0;

    hk_runtime_tbl_entry_t *RtEntry = NULL;

    /* Loop thru the RunTime table initializing the fields */
    for (i = 0; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        RtEntry = &RtTbl[i];

        RtEntry->OutputPktAddr      = NULL;
        RtEntry->InputMidSubscribed = HK_INPUTMID_NOT_SUBSCRIBED;
        RtEntry->DataPresent        = HK_DATA_NOT_PRESENT;
    }
}

void HK_Test_Setup(void)
{
    /* initialize test environment to default state for every test */
    UT_ResetState(0);

    memset(&HK_AppData, 0, sizeof(HK_AppData));
    memset(context_CFE_EVS_SendEvent, 0, sizeof(context_CFE_EVS_SendEvent));
    memset(&context_CFE_ES_WriteToSysLog, 0, sizeof(context_CFE_ES_WriteToSysLog));

    /* Register custom handlers */
    UT_SetVaHandlerFunction(UT_KEY(CFE_EVS_SendEvent), UT_Handler_CFE_EVS_SendEvent, NULL);
    UT_SetVaHandlerFunction(UT_KEY(CFE_ES_WriteToSysLog), UT_Handler_CFE_ES_WriteToSysLog, NULL);
}

void HK_Test_TearDown(void)
{
    /* cleanup test environment */
}
