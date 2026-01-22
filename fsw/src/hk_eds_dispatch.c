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

/**
 * @file
 *   CFS Checksum (CS) Application provides the service of background
 *   checksumming user-defined objects in the CFS
 */
#include "cfe.h"

#include "hk_app.h"
#include "hk_cmds.h"
#include "hk_events.h"
#include "hk_msgids.h"
#include "hk_dispatch.h"
#include "hk_utils.h"

#include "hk_eds_dictionary.h"
#include "hk_eds_dispatcher.h"

/* clang-format off */
static const EdsDispatchTable_EdsComponent_HK_Application_CFE_SB_Telecommand_t HK_TC_DISPATCH_TABLE =
{
    .CMD =
    {
        .NoopCmd_indication                        = HK_NoopCmd,
        .ResetCountersCmd_indication                       = HK_ResetCountersCmd,
    },
    .SEND_HK          =
    {
        .indication = HK_SendHkCmd
    },
    .SEND_COMBINED_PKT =
    {
        .indication = HK_SendCombinedPktCmd
    }
};
/* clang-format on */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HK_AppPipe() -- Process command pipe message           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HK_AppPipe(const CFE_SB_Buffer_t *BufPtr)
{
    CFE_Status_t      status;
    CFE_SB_MsgId_t    MsgId;
    CFE_MSG_Size_t    MsgSize;
    CFE_MSG_FcnCode_t MsgFc;

    status = EdsDispatch_EdsComponent_HK_Application_Telecommand(BufPtr, &HK_TC_DISPATCH_TABLE);

    /* These status codes means the message was not addressed to one of the known msgids for this app */
    if (status == CFE_STATUS_VALIDATION_FAILURE || status == CFE_STATUS_UNKNOWN_MSG_ID)
    {
        /* Incoming housekeeping data from other Subsystems...       */
        HK_ProcessIncomingHkData(BufPtr);
    }
    else if (status != CFE_SUCCESS)
    {
        ++HK_AppData.HkPacket.Payload.ErrCounter;

        CFE_MSG_GetMsgId(&BufPtr->Msg, &MsgId);
        CFE_MSG_GetFcnCode(&BufPtr->Msg, &MsgFc);
        if (status == CFE_STATUS_WRONG_MSG_LENGTH)
        {
            CFE_MSG_GetSize(&BufPtr->Msg, &MsgSize);
            CFE_EVS_SendEvent(HK_CMD_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Invalid msg length: ID = 0x%08lX, CC = %d, Len = %lu",
                              (unsigned long)CFE_SB_MsgIdToValue(MsgId), MsgFc, (unsigned long)MsgSize);
        }
        else
        {
            CFE_EVS_SendEvent(HK_CC_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Invalid message recvd: ID=0x%08lX, CC=%d, status=%d",
                              (unsigned long)CFE_SB_MsgIdToValue(MsgId), MsgFc, (int)status);
        }
    }
}
