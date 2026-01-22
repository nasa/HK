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
 *  The CFS Housekeeping (HK) Application file containing the application
 *  initialization routines, the main routine and the command interface.
 */

/************************************************************************
** Includes
*************************************************************************/
#include "hk_app.h"
#include "hk_events.h"
#include "hk_cmds.h"
#include "hk_platform_cfg.h"
#include "hk_utils.h"
#include "hk_dispatch.h"
#include "hk_version.h"
#include <string.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send Combined Housekeeping Packet                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CFE_Status_t HK_SendCombinedPktCmd(const HK_SendCombinedPktCmd_t *BufPtr)
{
    const HK_SendCombinedPkt_Payload_t *CmdPtr;

    CmdPtr = &((const HK_SendCombinedPktCmd_t *)BufPtr)->Payload;

    HK_SendCombinedHkPacket(CmdPtr->OutMsgToSend);

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Housekeeping request                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CFE_Status_t HK_SendHkCmd(const HK_SendHkCmd_t *BufPtr)
{
    HK_HkTlm_Payload_t *PayloadPtr;

    PayloadPtr = &HK_AppData.HkPacket.Payload;

    /* copy data into housekeeping packet */
    PayloadPtr->CmdCounter          = HK_AppData.CmdCounter;
    PayloadPtr->ErrCounter          = HK_AppData.ErrCounter;
    PayloadPtr->MissingDataCtr      = HK_AppData.MissingDataCtr;
    PayloadPtr->CombinedPacketsSent = HK_AppData.CombinedPacketsSent;
    PayloadPtr->MemPoolHandle       = HK_AppData.MemPoolHandle;

    /* Send housekeeping telemetry packet...        */
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(HK_AppData.HkPacket.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(HK_AppData.HkPacket.TelemetryHeader), true);

    /* Check for copy table load and runtime dump request */
    if (HK_CheckStatusOfTables() != HK_SUCCESS)
    {
        HK_AppData.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Noop command                                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CFE_Status_t HK_NoopCmd(const HK_NoopCmd_t *BufPtr)
{
    CFE_EVS_SendEvent(HK_NOOP_INF_EID, CFE_EVS_EventType_INFORMATION, "HK No-op command, Version %d.%d.%d.%d",
                      HK_MAJOR_VERSION, HK_MINOR_VERSION, HK_REVISION, HK_MISSION_REV);

    HK_AppData.CmdCounter++;

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Reset counters command                                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CFE_Status_t HK_ResetCountersCmd(const HK_ResetCountersCmd_t *BufPtr)
{
    HK_ResetHkData();
    CFE_EVS_SendEvent(HK_RESET_INF_EID, CFE_EVS_EventType_INFORMATION, "HK Reset Counters command received");

    return CFE_SUCCESS;
}

/************************/
/*  End of File Comment */
/************************/
