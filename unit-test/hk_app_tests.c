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

uint8 call_count_CFE_EVS_SendEvent;

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_AppMain                                      */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_AppMain
 *
 * Case: Tests the "nominal" mode where all dependent calls should be
 *       successful by default.
 */
void Test_HK_AppMain_Success(void)
{
    /* Arrange */

    /* Act */
    HK_AppMain();

    /* Assert */
    UtAssert_True(UT_GetStubCount(UT_KEY(CFE_ES_ExitApp)) == 1, "CFE_ES_ExitApp() called");
}

/*
 * Function under test: HK_AppMain
 *
 * Case: Tests the case where HK_AppInit fails and the application exits.
 */
void Test_HK_AppMain_InitFail(void)
{
    /* Arrange */
    uint8 call_count_CFE_SB_ReceiveBuffer = 0;
    uint8 call_count_CFE_ES_ExitApp       = 0;

    UT_SetDeferredRetcode(UT_KEY(CFE_EVS_Register), 1, CFE_EVS_INVALID_PARAMETER);

    /* Act */
    HK_AppMain();

    call_count_CFE_SB_ReceiveBuffer = UT_GetStubCount(UT_KEY(CFE_SB_ReceiveBuffer));
    call_count_CFE_ES_ExitApp       = UT_GetStubCount(UT_KEY(CFE_ES_ExitApp));

    /* Assert */

    UtAssert_INT32_EQ(HK_AppData.RunStatus, CFE_ES_RunStatus_APP_ERROR);

    /* Confirm that CFE_SB_ReceiveBuffer was not called */
    UtAssert_INT32_EQ(call_count_CFE_SB_ReceiveBuffer, 0);

    /* Confirm that CFE_ES_ExitApp was called at end of execution */
    UtAssert_INT32_EQ(call_count_CFE_ES_ExitApp, 1);
}

/*
 * Function under test: HK_AppMain
 *
 * Case: Tests a single successful iteration of the main run loop.
 */
void Test_HK_AppMain_SingleRunLoop(void)
{
    /* Arrange */
    CFE_SB_MsgId_t forced_MsgID                    = HK_UT_MID_100;
    size_t         forced_MsgSize                  = 0; /* invalid message size */
    uint8          call_count_CFE_SB_ReceiveBuffer = 0;

    /*
     * Note that CFE_ES_RunLoop returns a boolean value,
     * so in order to exercise the internal "while" loop,
     * it needs to return TRUE.  But this also needs to return
     * FALSE in order to get out of the loop, otherwise
     * it will stay there infinitely.
     *
     * The deferred retcode will accomplish this.
     */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, false);

    /* Set return codes for table functions so that HK_TableInit succeeds. */
    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_GetAddress), 1, CFE_TBL_INFO_UPDATED);
    UT_SetDefaultReturnValue(UT_KEY(HK_ProcessNewCopyTable), CFE_SUCCESS);

    /* Setting a non-HK message ID and message size to avoid
     * a seg fault in HK_AppPipe - the specific behavior of
     * HK_AppPipe does not matter for this test */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &forced_MsgID, sizeof(forced_MsgID), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &forced_MsgSize, sizeof(forced_MsgSize), false);

    /* Act */
    HK_AppMain();

    call_count_CFE_SB_ReceiveBuffer = UT_GetStubCount(UT_KEY(CFE_SB_ReceiveBuffer));

    /* Assert */
    UtAssert_INT32_EQ(call_count_CFE_SB_ReceiveBuffer, 1);

    UtAssert_INT32_EQ(HK_AppData.RunStatus, CFE_ES_RunStatus_APP_RUN);
}

/*
 * Function under test: HK_AppMain
 *
 * Case: Tests the case in which the main run loop is executed, but the
 *       call to CFE_SB_ReceiveBuffer fails.
 */
void Test_HK_AppMain_RcvBufFail(void)
{
    /* Arrange */
    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "HK_APP Exiting due to CFE_SB_RcvMsg error 0x%%08X");

    /* Set return codes for table functions so that HK_TableInit
     * succeeds. */
    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_GetAddress), 1, CFE_TBL_INFO_UPDATED);
    UT_SetDefaultReturnValue(UT_KEY(HK_ProcessNewCopyTable), CFE_SUCCESS);

    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_ReceiveBuffer), CFE_SB_PIPE_RD_ERR);

    /* Act */
    HK_AppMain();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */

    /* The first event message is due to correct initialization.  This test only
     * verifies the fields of the second (because it is generated by the
     * function under test). */
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 2);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, HK_RCV_MSG_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[1].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[1].Spec);

    UtAssert_INT32_EQ(HK_AppData.RunStatus, CFE_ES_RunStatus_APP_ERROR);
}

/*
 * Function under test: HK_AppMain
 *
 * Case: Tests the case in which the main run loop is executed, but the
 *       call to CFE_SB_ReceiveBuffer times out.
 */
void Test_HK_AppMain_RcvBufTimeout(void)
{
    /* Arrange */

    /* Set return codes for table functions so that HK_TableInit
     * succeeds. */
    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_GetAddress), 1, CFE_TBL_INFO_UPDATED);
    UT_SetDefaultReturnValue(UT_KEY(HK_ProcessNewCopyTable), CFE_SUCCESS);

    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_ReceiveBuffer), CFE_SB_TIME_OUT);

    /* Act */
    HK_AppMain();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */

    /* The first event message is due to correct initialization.  This test only
     * verifies the fields of the second (because it is generated by the
     * function under test). */
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(HK_AppData.RunStatus, CFE_ES_RunStatus_APP_RUN);
}

/*
 * Function under test: HK_AppMain
 *
 * Case: Tests the case in which the main run loop is executed, but the
 *       call to CFE_SB_ReceiveBuffer times out and the call to
 *       HK_CheckStatusOfTables fails.
 */
void Test_HK_AppMain_RcvBufTimeoutCheckFail(void)
{
    /* Arrange */

    /* Set return codes for table functions so that HK_TableInit
     * succeeds. */
    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_GetAddress), 1, CFE_TBL_INFO_UPDATED);
    UT_SetDefaultReturnValue(UT_KEY(HK_ProcessNewCopyTable), CFE_SUCCESS);

    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_ReceiveBuffer), CFE_SB_TIME_OUT);
    UT_SetDefaultReturnValue(UT_KEY(HK_CheckStatusOfTables), !HK_SUCCESS);

    /* Act */
    HK_AppMain();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */

    /* The first event message is due to correct initialization.  This test only
     * verifies the fields of the second (because it is generated by the
     * function under test). */
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(HK_AppData.RunStatus, CFE_ES_RunStatus_APP_ERROR);
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_AppInit                                      */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_AppInit
 *
 * Case: Tests the nominal case in which all dependent calls succeed.
 */
void Test_HK_AppInit_Success(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        strCmpResult;
    char         ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "HK Initialized.  Version %%d.%%d.%%d.%%d");

    /* Set return codes for table functions so that HK_TableInit succeeds. */
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Load), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Manage), CFE_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_GetAddress), 1, CFE_TBL_INFO_UPDATED);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(HK_ProcessNewCopyTable), CFE_SUCCESS);

    /* Act */
    ReturnValue = HK_AppInit();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, CFE_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_INIT_INF_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_AppInit
 *
 * Case: Tests the case in which the call to CFE_EVS_Register fails.
 */
void Test_HK_AppInit_EVSRegFail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        strCmpResult;
    char         ExpectedSysLogString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedSysLogString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "HK: error registering for event services: 0x%%08X\n");

    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), -1);

    /* Act */
    ReturnValue = HK_AppInit();

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, -1);

    strCmpResult = strncmp(ExpectedSysLogString, context_CFE_ES_WriteToSysLog.Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Sys Log string matched expected result, '%s'", context_CFE_ES_WriteToSysLog.Spec);
}

/*
 * Function under test: HK_AppInit
 *
 * Case: Tests the case in which the call to CFE_SB_CreatePipe fails.
 */
void Test_HK_AppInit_SBCreatePipeFail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        strCmpResult;
    char         ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    int32        ForcedReturnVal = -1;

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "Error Creating SB Pipe,RC=0x%%08X");

    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_CreatePipe), ForcedReturnVal);

    /* Act */
    ReturnValue = HK_AppInit();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, ForcedReturnVal);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_CR_PIPE_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Sys Log string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_AppInit
 *
 * Case: Tests the case in which the first call to CFE_SB_Subscribe fails.
 */
void Test_HK_AppInit_SBSubscribe1Fail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        strCmpResult;
    char         ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    int32        ForcedReturnVal = -1;

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Error Subscribing to HK Snd Cmb Pkt, MID=0x%%08X, RC=0x%%08X");

    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 1, ForcedReturnVal);

    /* Act */
    ReturnValue = HK_AppInit();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, ForcedReturnVal);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_SUB_CMB_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Sys Log string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_AppInit
 *
 * Case: Tests the case in which the second call to CFE_SB_Subscribe fails.
 */
void Test_HK_AppInit_SBSubscribe2Fail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        strCmpResult;
    char         ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    int32        ForcedReturnVal = -1;

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Error Subscribing to HK Request, MID=0x%%08X, RC=0x%%08X");

    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 2, ForcedReturnVal);

    /* Act */
    ReturnValue = HK_AppInit();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, ForcedReturnVal);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_SUB_REQ_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Sys Log string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_AppInit
 *
 * Case: Tests the case in which the third call to CFE_SB_Subscribe fails.
 */
void Test_HK_AppInit_SBSubscribe3Fail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        strCmpResult;
    char         ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    int32        ForcedReturnVal = -1;

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Error Subscribing to HK Gnd Cmds, MID=0x%%08X, RC=0x%%08X");

    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 3, ForcedReturnVal);

    /* Act */
    ReturnValue = HK_AppInit();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, ForcedReturnVal);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_SUB_CMD_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Sys Log string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_AppInit
 *
 * Case: Tests the case in which the call to CFE_ES_PoolCreate fails.
 */
void Test_HK_AppInit_PoolCreateFail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        strCmpResult;
    char         ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    int32        ForcedReturnVal = -1;

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "Error Creating Memory Pool,RC=0x%%08X");

    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_PoolCreate), ForcedReturnVal);

    /* Act */
    ReturnValue = HK_AppInit();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, ForcedReturnVal);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_CR_POOL_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Sys Log string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_AppInit
 *
 * Case: Tests the case in which the call to HK_TableInit fails.
 */
void Test_HK_AppInit_TblInitFail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;

    /* forcing CFE_TBL_Register to fail indirectly causes HK_TableInit to fail */
    int32 ForcedReturnVal = -1;
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), ForcedReturnVal);

    /* Act */
    ReturnValue = HK_AppInit();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, ForcedReturnVal);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
}

/*
 * Function under test: HK_AppInit
 *
 * Case: Tests the case in which the call to CFE_EVS_SendEvent fails.
 */
void Test_HK_AppInit_SendEventFail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        strCmpResult;
    uint8        call_count_CFE_ES_WriteToSysLog;
    char         ExpectedSysLogString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedSysLogString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "HK App:Error Sending Initialization Event,RC=0x%%08X\n");

    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_SendEvent), -1);

    /* Act */
    ReturnValue = HK_AppInit();

    call_count_CFE_ES_WriteToSysLog = UT_GetStubCount(UT_KEY(CFE_ES_WriteToSysLog));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, -1);

    UtAssert_INT32_EQ(call_count_CFE_ES_WriteToSysLog, 1);

    strCmpResult = strncmp(ExpectedSysLogString, context_CFE_ES_WriteToSysLog.Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Sys Log string matched expected result, '%s'", context_CFE_ES_WriteToSysLog.Spec);
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_TableInit                                    */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_TableInit
 *
 * Case: Tests the case in which the HK_TableInit succeeds.
 */
void Test_HK_TableInit_Success(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;

    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_GetAddress), 1, CFE_TBL_INFO_UPDATED);
    UT_SetDefaultReturnValue(UT_KEY(HK_ProcessNewCopyTable), CFE_SUCCESS);

    /* Act */
    ReturnValue = HK_TableInit();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, CFE_SUCCESS);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
}

/*
 * Function under test: HK_TableInit
 *
 * Case: Tests the case in which the first call to CFE_TBL_Register fails.
 */
void Test_HK_TableInit_RegisterCpyTblFail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;

    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "Error Registering Copy Table,RC=0x%%08X");

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), -1);

    /* Act */
    ReturnValue = HK_TableInit();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, -1);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_CPTBL_REG_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_TableInit
 *
 * Case: Tests the case in which the second call to CFE_TBL_Register fails.
 */
void Test_HK_TableInit_RegisterRtTblFail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        strCmpResult;
    char         ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "Error Registering Runtime Table,RC=0x%%08X");

    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_Register), 2, -1);

    /* Act */
    ReturnValue = HK_TableInit();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, -1);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_RTTBL_REG_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_TableInit
 *
 * Case: Tests the case in which the call to CFE_TBL_Load fails.
 */
void Test_HK_TableInit_LoadCpyTblFail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        strCmpResult;
    char         ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "Error Loading Copy Table,RC=0x%%08X");

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Load), -1);

    /* Act */
    ReturnValue = HK_TableInit();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, -1);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_CPTBL_LD_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_TableInit
 *
 * Case: Tests the case in which the first call to CFE_TBL_Manage fails.
 */
void Test_HK_TableInit_ManageCpyTblFail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        strCmpResult;
    char         ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Error from TBL Manage call for Copy Table,RC=0x%%08X");

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Manage), -1);

    /* Act */
    ReturnValue = HK_TableInit();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, -1);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_CPTBL_MNG_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_TableInit
 *
 * Case: Tests the case in which the second call to CFE_TBL_Manage fails.
 */
void Test_HK_TableInit_ManageRtTblFail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        strCmpResult;
    char         ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Error from TBL Manage call for Runtime Table,RC=0x%%08X");

    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_Manage), 2, -1);

    /* Act */
    ReturnValue = HK_TableInit();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, -1);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_RTTBL_MNG_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_TableInit
 *
 * Case: Tests the case in which the first call to CFE_TBL_GetAddress fails.
 */
void Test_HK_TableInit_GetAddrCpyTblFail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        strCmpResult;
    char         ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "Error Getting Adr for Cpy Tbl,RC=0x%%08X");

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), -1);

    /* Act */
    ReturnValue = HK_TableInit();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, -1);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_CPTBL_GADR_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_TableInit
 *
 * Case: Tests the case in which the second call to CFE_TBL_GetAddress fails.
 */
void Test_HK_TableInit_GetAddrRtTblFail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        strCmpResult;
    char         ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "Error Getting Adr for Runtime Table,RC=0x%%08X");

    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_GetAddress), 1, CFE_TBL_INFO_UPDATED);
    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_GetAddress), 1, -1);

    /* Act */
    ReturnValue = HK_TableInit();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, -1);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_RTTBL_GADR_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/*
 * Function under test: HK_TableInit
 *
 * Case: Tests the case in which the second call to HK_ProcessNewCopyTable fails.
 */
void Test_HK_TableInit_ProcessNewCpyTblFail(void)
{
    /* Arrange */
    CFE_Status_t ReturnValue;
    int32        strCmpResult;
    char         ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Process New Copy Table Failed, status = 0x%%08X");

    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_GetAddress), 1, CFE_TBL_INFO_UPDATED);
    UT_SetDefaultReturnValue(UT_KEY(HK_ProcessNewCopyTable), -1);

    /* Act */
    ReturnValue = HK_TableInit();

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(ReturnValue, -1);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_NEWCPYTBL_INIT_FAILED_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_AppPipe                                      */
/*                                                                    */
/**********************************************************************/

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
    union
    {
        CFE_SB_Buffer_t         sbbuf;
        HK_SendCombinedPktCmd_t msg;
    } msgbuf;

    memset(&msgbuf, 0, sizeof(msgbuf));

    /* Act */
    HK_SendCombinedPktCmd(&msgbuf.sbbuf);

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
    CFE_SB_Buffer_t     Msg;
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
    CFE_SB_Buffer_t Buf;
    int32           strCmpResult;
    char            ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "HK No-op command, Version %%d.%%d.%%d.%%d");

    /* Act */
    HK_NoopCmd(&Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

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
    CFE_SB_Buffer_t Buf;

    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "HK Reset Counters command received");

    memset(&Buf, 0, sizeof(Buf));

    /* Act */
    HK_ResetCountersCmd(&Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, HK_RESET_INF_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);

    /* neither counter is updated in this function in success case */
    UtAssert_INT32_EQ(HK_AppData.CmdCounter, 0);
    UtAssert_INT32_EQ(HK_AppData.ErrCounter, 0);
}

/**********************************************************************/
/*                                                                    */
/* Test functions for HK_ResetHkData                                  */
/*                                                                    */
/**********************************************************************/

/*
 * Function under test: HK_ResetHkData
 *
 * Case: Tests the nominal case of the HK_ResetHkData.  Note that
 *       the function under test has no branches.
 */
void Test_HK_ResetHkData(void)
{
    /* Arrange */
    HK_AppData.CmdCounter          = 1;
    HK_AppData.ErrCounter          = 1;
    HK_AppData.CombinedPacketsSent = 1;
    HK_AppData.MissingDataCtr      = 1;

    /* Act */
    HK_ResetHkData();

    /* Assert */
    UtAssert_INT32_EQ(HK_AppData.CmdCounter, 0);
    UtAssert_INT32_EQ(HK_AppData.ErrCounter, 0);
    UtAssert_INT32_EQ(HK_AppData.CombinedPacketsSent, 0);
    UtAssert_INT32_EQ(HK_AppData.MissingDataCtr, 0);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
}

/****************************************************************************/

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    /* Test functions for HK_AppMain */
    UtTest_Add(Test_HK_AppMain_Success, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppMain_Success");
    UtTest_Add(Test_HK_AppMain_InitFail, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppMain_InitFail");
    UtTest_Add(Test_HK_AppMain_SingleRunLoop, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppMain_SingleRunLoop");
    UtTest_Add(Test_HK_AppMain_RcvBufFail, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppMain_RcvBufFail");

    UtTest_Add(Test_HK_AppMain_RcvBufTimeout, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppMain_RcvBufTimeout");
    UtTest_Add(Test_HK_AppMain_RcvBufTimeoutCheckFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_AppMain_RcvBufTimeoutCheckFail");

    /* Test functions for HK_AppInit */
    UtTest_Add(Test_HK_AppInit_Success, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppInit_Success");
    UtTest_Add(Test_HK_AppInit_EVSRegFail, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppInit_EVSRegFail");
    UtTest_Add(Test_HK_AppInit_SBCreatePipeFail, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppInit_SBCreatePipeFail");
    UtTest_Add(Test_HK_AppInit_SBSubscribe1Fail, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppInit_SBSubscribe1Fail");
    UtTest_Add(Test_HK_AppInit_SBSubscribe2Fail, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppInit_SBSubscribe2Fail");
    UtTest_Add(Test_HK_AppInit_SBSubscribe3Fail, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppInit_SBSubscribe3Fail");
    UtTest_Add(Test_HK_AppInit_PoolCreateFail, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppInit_PoolCreateFail");
    UtTest_Add(Test_HK_AppInit_TblInitFail, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppInit_TblInitFail");
    UtTest_Add(Test_HK_AppInit_SendEventFail, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppInit_SendEventFail");

    /* Test functions for HK_TableInit */
    UtTest_Add(Test_HK_TableInit_Success, HK_Test_Setup, HK_Test_TearDown, "Test_HK_TableInit_Success");
    UtTest_Add(Test_HK_TableInit_RegisterCpyTblFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_TableInit_RegisterCpyTblFail");
    UtTest_Add(Test_HK_TableInit_RegisterRtTblFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_TableInit_RegisterRtTblFail");
    UtTest_Add(Test_HK_TableInit_LoadCpyTblFail, HK_Test_Setup, HK_Test_TearDown, "Test_HK_TableInit_LoadCpyTblFail");
    UtTest_Add(Test_HK_TableInit_ManageCpyTblFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_TableInit_ManageCpyTblFail");
    UtTest_Add(Test_HK_TableInit_ManageRtTblFail, HK_Test_Setup, HK_Test_TearDown, "Test_HK_TableInit_ManageRtTblFail");
    UtTest_Add(Test_HK_TableInit_GetAddrCpyTblFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_TableInit_GetAddrCpyTblFail");
    UtTest_Add(Test_HK_TableInit_GetAddrRtTblFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_TableInit_GetAddrRtTblFail");
    UtTest_Add(Test_HK_TableInit_ProcessNewCpyTblFail, HK_Test_Setup, HK_Test_TearDown,
               "Test_HK_TableInit_ProcessNewCpyTblFail");

    /* Test functions for HK_SendCombinedHkPacket */
    UtTest_Add(Test_HK_SendCombinedPktCmd, HK_Test_Setup, HK_Test_TearDown, "Test_HK_SendCombinedPktCmd");

    /* Test functions for HK_SendHkCmd */
    UtTest_Add(Test_HK_SendHkCmd, HK_Test_Setup, HK_Test_TearDown, "Test_HK_SendHkCmd");

    /* Test functions for HK_NoopCmd */
    UtTest_Add(Test_HK_NoopCmd, HK_Test_Setup, HK_Test_TearDown, "Test_HK_NoopCmd");

    /* Test functions for HK_ResetCountersCmd */
    UtTest_Add(Test_HK_ResetCountersCmd, HK_Test_Setup, HK_Test_TearDown, "Test_HK_ResetCountersCmd");

    /* Test functions for HK_ResetHkData */
    UtTest_Add(Test_HK_ResetHkData, HK_Test_Setup, HK_Test_TearDown, "Test_HK_ResetHkData");
}
