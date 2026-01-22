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

/*
 * App Includes
 */

#include "hk_app.h"
#include "hk_cmds.h"
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

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_SendCombinedPktCmd                            */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_SendCombinedPktCmd
 *
 * Case: Tests the nominal case of the HK_SendCombinedPktCmd.  Note that
 *       the function under test has no branches.
 */
void Test_HK_SendCombinedPktCmd(void)
{
    HK_SendCombinedPktCmd_t msgbuf;

    memset(&msgbuf, 0, sizeof(msgbuf));

    /* Act */
    HK_SendCombinedPktCmd(&msgbuf);

    /* Assert */
    UtAssert_STUB_COUNT(HK_SendCombinedHkPacket, 1);
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_SendHkCmd                              */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_SendHkCmd
 *
 * Case: Tests the nominal case of the HK_SendHkCmd.  Note that
 *       the function under test has no branches.
 */
void Test_HK_SendHkCmd(void)
{
    /* Arrange */
    HK_SendHkCmd_t      Msg;
    uint8               call_count_CFE_SB_TimeStampMsg;
    uint8               call_count_CFE_SB_TransmitMsg;
    HK_HkTlm_Payload_t *PayloadPtr;

    /* Setup app data values */
    HK_AppData.CmdCounter          = 1;
    HK_AppData.ErrCounter          = 2;
    HK_AppData.MissingDataCtr      = 3;
    HK_AppData.CombinedPacketsSent = 4;
    HK_AppData.MemPoolHandle       = HK_UT_MEMPOOL_1;

    memset(&Msg, 0, sizeof(Msg));

    /* Act */
    HK_SendHkCmd(&Msg);

    call_count_CFE_SB_TimeStampMsg = UT_GetStubCount(UT_KEY(CFE_SB_TimeStampMsg));
    call_count_CFE_SB_TransmitMsg  = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));

    /* Assert */
    PayloadPtr = &HK_AppData.HkPacket.Payload;
    UtAssert_INT32_EQ(HK_AppData.CmdCounter, PayloadPtr->CmdCounter);
    UtAssert_INT32_EQ(HK_AppData.ErrCounter, PayloadPtr->ErrCounter);
    UtAssert_INT32_EQ(HK_AppData.MissingDataCtr, PayloadPtr->MissingDataCtr);
    UtAssert_INT32_EQ(HK_AppData.CombinedPacketsSent, PayloadPtr->CombinedPacketsSent);
    UtAssert_True(CFE_RESOURCEID_TEST_EQUAL(HK_AppData.MemPoolHandle, PayloadPtr->MemPoolHandle),
                  "CFE_RESOURCEID_TEST_EQUAL(HK_AppData.MemPoolHandle, PayloadPtr->MemPoolHandle)");

    UtAssert_INT32_EQ(call_count_CFE_SB_TimeStampMsg, 1);
    UtAssert_INT32_EQ(call_count_CFE_SB_TransmitMsg, 1);

    /* Bad Table Status */
    UT_SetDefaultReturnValue(UT_KEY(HK_CheckStatusOfTables), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    /* Act */
    HK_SendHkCmd(&Msg);

    /* Assert */
    UtAssert_UINT32_EQ(HK_AppData.RunStatus, CFE_ES_RunStatus_APP_ERROR);
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_NoopCmd                                      */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_NoopCmd
 *
 * Case: Tests the case in which the HK_NoopCmd function processes a
 *       Noop command message with the correct length.
 */
void Test_HK_NoopCmd(void)
{
    /* Arrange */
    HK_NoopCmd_t Buf;
    int32        strCmpResult;
    char         ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "HK No-op command, Version %%d.%%d.%%d.%%d");

    /* Act */
    HK_NoopCmd(&Buf);

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_NOOP_INF_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);

    UtAssert_INT32_EQ(HK_AppData.CmdCounter, 1);
    UtAssert_INT32_EQ(HK_AppData.ErrCounter, 0);
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_ResetCountersCmd                                 */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_ResetCountersCmd
 *
 * Case: Tests the case in which the HK_ResetCountersCmd function processes
 *       a Reset Counters command message with the correct length.
 */
void Test_HK_ResetCountersCmd(void)
{
    /* Arrange */
    HK_ResetCountersCmd_t Buf;

    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "HK Reset Counters command received");

    memset(&Buf, 0, sizeof(Buf));

    /* Act */
    HK_ResetCountersCmd(&Buf);

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_RESET_INF_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);

    /* neither counter is updated in this function in success case */
    UtAssert_INT32_EQ(HK_AppData.CmdCounter, 0);
    UtAssert_INT32_EQ(HK_AppData.ErrCounter, 0);
}

/****************************************************************************/

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    /* Test functions for HK_SendCombinedHkPacket */
    UtTest_Add(Test_HK_SendCombinedPktCmd, HK_Test_Setup, HK_Test_TearDown, "Test_HK_SendCombinedPktCmd");

    /* Test functions for HK_SendHkCmd */
    UtTest_Add(Test_HK_SendHkCmd, HK_Test_Setup, HK_Test_TearDown, "Test_HK_SendHkCmd");

    /* Test functions for HK_NoopCmd */
    UtTest_Add(Test_HK_NoopCmd, HK_Test_Setup, HK_Test_TearDown, "Test_HK_NoopCmd");

    /* Test functions for HK_ResetCountersCmd */
    UtTest_Add(Test_HK_ResetCountersCmd, HK_Test_Setup, HK_Test_TearDown, "Test_HK_ResetCountersCmd");
}
