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
 *  The CFS Housekeeping (HK) Application file containing the application
 *  initialization routines, the main routine and the command interface.
 */

/************************************************************************
** Includes
*************************************************************************/
#include "hk_app.h"
#include "hk_events.h"
#include "hk_msgids.h"
#include "hk_dispatch.h"
#include "hk_utils.h"

#include <string.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HK_VerifyCmdLength(const CFE_SB_Buffer_t *BufPtr, size_t ExpectedLength)
{
    int32             Status       = HK_SUCCESS;
    CFE_SB_MsgId_t    MessageID    = CFE_SB_INVALID_MSG_ID; /* Init to invalid value */
    CFE_MSG_FcnCode_t CommandCode  = 0;
    size_t            ActualLength = 0;

    CFE_MSG_GetSize(&BufPtr->Msg, &ActualLength);

    if (ExpectedLength != ActualLength)
    {
        CFE_MSG_GetMsgId(&BufPtr->Msg, &MessageID);
        CFE_MSG_GetFcnCode(&BufPtr->Msg, &CommandCode);

        CFE_EVS_SendEvent(HK_CMD_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Cmd Msg with Bad length Rcvd: ID = 0x%08lX, CC = %d, Exp Len = %d, Len = %d",
                          (unsigned long)CFE_SB_MsgIdToValue(MessageID), CommandCode, (int)ExpectedLength,
                          (int)ActualLength);

        Status = HK_BAD_MSG_LENGTH_RC;
    }

    return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Non-Command Msg Length (Event is different)               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HK_VerifyMsgLength(const CFE_SB_Buffer_t *BufPtr, size_t ExpectedLength)
{
    int32          Status       = HK_SUCCESS;
    CFE_SB_MsgId_t MessageID    = CFE_SB_INVALID_MSG_ID; /* Init to invalid value */
    size_t         ActualLength = 0;

    CFE_MSG_GetSize(&BufPtr->Msg, &ActualLength);

    if (ExpectedLength != ActualLength)
    {
        CFE_MSG_GetMsgId(&BufPtr->Msg, &MessageID);

        CFE_EVS_SendEvent(HK_MSG_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Msg with Bad length Rcvd: ID = 0x%08lX, Exp Len = %u, Len = %u",
                          (unsigned long)CFE_SB_MsgIdToValue(MessageID), (unsigned int)ExpectedLength,
                          (unsigned int)ActualLength);

        Status = HK_BAD_MSG_LENGTH_RC;
    }

    return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send Combined Housekeeping Packet                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HK_SendCombinedPktVerifyDispatch(const CFE_SB_Buffer_t *BufPtr)
{
    if (HK_VerifyMsgLength(BufPtr, sizeof(HK_SendCombinedPktCmd_t)) == HK_SUCCESS)
    {
        HK_SendCombinedPktCmd(BufPtr);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Housekeeping request                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HK_SendHkVerifyDispatch(const CFE_SB_Buffer_t *BufPtr)
{
    if (HK_VerifyMsgLength(BufPtr, sizeof(HK_SendHkCmd_t)) == HK_SUCCESS)
    {
        HK_SendHkCmd(BufPtr);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Noop command                                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HK_NoopVerifyDispatch(const CFE_SB_Buffer_t *BufPtr)
{
    size_t ExpectedLength = sizeof(HK_NoopCmd_t);

    if (HK_VerifyCmdLength(BufPtr, ExpectedLength) == HK_SUCCESS)
    {
        HK_NoopCmd(BufPtr);
    }
    else
    {
        HK_AppData.ErrCounter++;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Reset counters command                                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HK_ResetCountersVerifyDispatch(const CFE_SB_Buffer_t *BufPtr)
{
    size_t ExpectedLength = sizeof(HK_ResetCountersCmd_t);

    if (HK_VerifyCmdLength(BufPtr, ExpectedLength) == HK_SUCCESS)
    {
        HK_ResetCountersCmd(BufPtr);
    }
    else
    {
        HK_AppData.ErrCounter++;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process a command pipe message                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HK_AppPipe(const CFE_SB_Buffer_t *BufPtr)
{
    CFE_SB_MsgId_t    MessageID   = CFE_SB_INVALID_MSG_ID; /* Init to invalid value */
    CFE_MSG_FcnCode_t CommandCode = 0;

    CFE_MSG_GetMsgId(&BufPtr->Msg, &MessageID);

    switch (CFE_SB_MsgIdToValue(MessageID))
    {
        case HK_SEND_COMBINED_PKT_MID:
            HK_SendCombinedPktVerifyDispatch(BufPtr);
            break;

        /* Request for HK's Housekeeping data...      */
        case HK_SEND_HK_MID:
            /* Send out HK's housekeeping data */
            HK_SendHkVerifyDispatch(BufPtr);

            /* Check for copy table load and runtime dump request */
            if (HK_CheckStatusOfTables() != HK_SUCCESS)
            {
                HK_AppData.RunStatus = CFE_ES_RunStatus_APP_ERROR;
            }
            break;

        /* HK ground commands   */
        case HK_CMD_MID:

            CFE_MSG_GetFcnCode(&BufPtr->Msg, &CommandCode);

            switch (CommandCode)
            {
                case HK_NOOP_CC:
                    HK_NoopVerifyDispatch(BufPtr);
                    break;

                case HK_RESET_COUNTERS_CC:
                    HK_ResetCountersVerifyDispatch(BufPtr);
                    break;

                default:
                    CFE_EVS_SendEvent(HK_CC_ERR_EID, CFE_EVS_EventType_ERROR,
                                      "Cmd Msg with Invalid command code Rcvd -- ID = 0x%08lX, CC = %d",
                                      (unsigned long)CFE_SB_MsgIdToValue(MessageID), CommandCode);
                    HK_AppData.ErrCounter++;
                    break;
            }
            break;

        /* Incoming housekeeping data from other Subsystems...       */
        default:
            HK_ProcessIncomingHkData(BufPtr);
            break;
    }
}
