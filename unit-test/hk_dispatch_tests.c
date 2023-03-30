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

#include "hk_dispatch.h"
#include "hk_msg.h"
#include "hk_events.h"
#include "hk_msgids.h"
#include "hk_test_utils.h"

/* UT includes */
#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

#include <unistd.h>
#include <stdlib.h>
#include "cfe.h"

/*
 * Helper functions
 */
static void HK_Dispatch_Test_SetupMsg(CFE_SB_MsgId_t MsgId, CFE_MSG_FcnCode_t FcnCode, size_t MsgSize)
{
    /* Note some paths get the MsgId/FcnCode multiple times, so register accordingly, just in case */
    CFE_SB_MsgId_t    RegMsgId[2]   = {MsgId, MsgId};
    CFE_MSG_FcnCode_t RegFcnCode[2] = {FcnCode, FcnCode};
    size_t            RegMsgSize[2] = {MsgSize, MsgSize};

    UT_ResetState(UT_KEY(CFE_MSG_GetMsgId));
    UT_ResetState(UT_KEY(CFE_MSG_GetFcnCode));
    UT_ResetState(UT_KEY(CFE_MSG_GetSize));

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), RegMsgId, sizeof(RegMsgId), true);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), RegFcnCode, sizeof(RegFcnCode), true);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), RegMsgSize, sizeof(RegMsgSize), true);
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_AppPipe                                      */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_AppPipe
 *
 * Case: Tests the case in which a "Send Combined Packet" message
 *       received with the expected length.
 */
void Test_HK_AppPipe_SendCombinedPktCmd(void)
{
    CFE_SB_Buffer_t Buf;

    memset(&Buf, 0, sizeof(Buf));
    HK_Dispatch_Test_SetupMsg(CFE_SB_ValueToMsgId(HK_SEND_COMBINED_PKT_MID), 0, sizeof(HK_SendCombinedPktCmd_t));

    /* Act */
    HK_AppPipe(&Buf);

    UtAssert_STUB_COUNT(HK_SendCombinedPktCmd, 1);

    /* Bad Length */
    HK_Dispatch_Test_SetupMsg(CFE_SB_ValueToMsgId(HK_SEND_COMBINED_PKT_MID), 0, 1);

    /* Act */
    HK_AppPipe(&Buf);

    UtAssert_STUB_COUNT(HK_SendCombinedPktCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_MSG_LEN_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

/*
 * Function under test: HK_AppPipe
 *
 * Case: Tests the case in which a "Housekeeping Request" message
 *       received with the expected length.
 */
void Test_HK_AppPipe_SendHkCmd(void)
{
    CFE_SB_Buffer_t Buf;

    memset(&Buf, 0, sizeof(Buf));
    HK_Dispatch_Test_SetupMsg(CFE_SB_ValueToMsgId(HK_SEND_HK_MID), 0, sizeof(HK_SendHkCmd_t));

    /* Act */
    HK_AppPipe(&Buf);

    UtAssert_STUB_COUNT(HK_SendHkCmd, 1);

    /* Bad Length */
    HK_Dispatch_Test_SetupMsg(CFE_SB_ValueToMsgId(HK_SEND_HK_MID), 0, 1);

    /* Act */
    HK_AppPipe(&Buf);

    UtAssert_STUB_COUNT(HK_SendHkCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_MSG_LEN_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    /* Bad Table Status */
    HK_Dispatch_Test_SetupMsg(CFE_SB_ValueToMsgId(HK_SEND_HK_MID), 0, sizeof(HK_SendHkCmd_t));
    UT_SetDefaultReturnValue(UT_KEY(HK_CheckStatusOfTables), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    /* Act */
    HK_AppPipe(&Buf);

    UtAssert_STUB_COUNT(HK_SendHkCmd, 2);
    UtAssert_UINT32_EQ(HK_AppData.RunStatus, CFE_ES_RunStatus_APP_ERROR);
}

/*
 * Function under test: HK_AppPipe
 *
 * Case: Tests the case in which a Command message is received with a
 *       "No Operation" command code.
 */
void Test_HK_AppPipe_NoopCmd(void)
{
    CFE_SB_Buffer_t Buf;

    memset(&Buf, 0, sizeof(Buf));
    HK_Dispatch_Test_SetupMsg(CFE_SB_ValueToMsgId(HK_CMD_MID), HK_NOOP_CC, sizeof(HK_NoopCmd_t));

    /* Act */
    HK_AppPipe(&Buf);

    UtAssert_STUB_COUNT(HK_NoopCmd, 1);
    UtAssert_ZERO(HK_AppData.ErrCounter);

    /* Bad Length */
    HK_Dispatch_Test_SetupMsg(CFE_SB_ValueToMsgId(HK_CMD_MID), HK_NOOP_CC, 1);

    /* Act */
    HK_AppPipe(&Buf);

    UtAssert_STUB_COUNT(HK_NoopCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_CMD_LEN_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_UINT8_EQ(HK_AppData.ErrCounter, 1);
}

/*
 * Function under test: HK_AppPipe
 *
 * Case: Tests the case in which a Command message is received with a
 *       "Reset" command code.
 */
void Test_HK_AppPipe_ResetCountersCmd(void)
{
    CFE_SB_Buffer_t Buf;

    memset(&Buf, 0, sizeof(Buf));
    HK_Dispatch_Test_SetupMsg(CFE_SB_ValueToMsgId(HK_CMD_MID), HK_RESET_COUNTERS_CC, sizeof(HK_ResetCountersCmd_t));

    /* Act */
    HK_AppPipe(&Buf);

    UtAssert_STUB_COUNT(HK_ResetCountersCmd, 1);
    UtAssert_ZERO(HK_AppData.ErrCounter);

    /* Bad Length */
    HK_Dispatch_Test_SetupMsg(CFE_SB_ValueToMsgId(HK_CMD_MID), HK_RESET_COUNTERS_CC, 1);

    /* Act */
    HK_AppPipe(&Buf);

    UtAssert_STUB_COUNT(HK_ResetCountersCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_CMD_LEN_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_UINT8_EQ(HK_AppData.ErrCounter, 1);
}

/*
 * Function under test: HK_AppPipe
 *
 * Case: Tests the case in which a Command message is received with an
 *       unknown command code.
 */
void Test_HK_AppPipe_UnknownCmd(void)
{
    CFE_SB_Buffer_t Buf;

    memset(&Buf, 0, sizeof(Buf));
    HK_Dispatch_Test_SetupMsg(CFE_SB_ValueToMsgId(HK_CMD_MID), 47, sizeof(HK_NoopCmd_t));

    /* Act */
    HK_AppPipe(&Buf);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_CC_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_UINT8_EQ(HK_AppData.ErrCounter, 1);
}

/*
 * Function under test: HK_AppPipe
 *
 * Case: Tests the case in which a message is received with an unknown
 *       message ID, triggering a call to HK_ProcessIncomingHkData.
 */
void Test_HK_AppPipe_ProcessIncoming(void)
{
    CFE_SB_Buffer_t Buf;

    memset(&Buf, 0, sizeof(Buf));
    HK_Dispatch_Test_SetupMsg(HK_UT_MID_100, 0, sizeof(HK_NoopCmd_t));

    /* Act */
    HK_AppPipe(&Buf);

    UtAssert_STUB_COUNT(HK_ProcessIncomingHkData, 1);
}

/****************************************************************************/

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    UtTest_Add(Test_HK_AppPipe_SendCombinedPktCmd, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_AppPipe_SendCombinedPktCmd");
    UtTest_Add(Test_HK_AppPipe_SendHkCmd, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppPipe_SendHkCmd");
    UtTest_Add(Test_HK_AppPipe_NoopCmd, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppPipe_NoopCmd");
    UtTest_Add(Test_HK_AppPipe_ResetCountersCmd, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppPipe_ResetCountersCmd");
    UtTest_Add(Test_HK_AppPipe_UnknownCmd, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppPipe_UnknownCmd");
    UtTest_Add(Test_HK_AppPipe_ProcessIncoming, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppPipe_ProcessIncoming");
}
