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
#include "hk_perfids.h"
#include "hk_verify.h"
#include "hk_version.h"
#include "hk_platform_cfg.h"
#include "hk_utils.h"
#include "hk_dispatch.h"
#include <string.h>

/************************************************************************
** HK global data
*************************************************************************/
HK_AppData_t HK_AppData;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HK application entry point and main process loop                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HK_AppMain(void)
{
    CFE_Status_t     Status = CFE_SUCCESS;
    CFE_SB_Buffer_t *BufPtr = NULL;

    /*
    ** Create the first Performance Log entry
    */
    CFE_ES_PerfLogEntry(HK_APPMAIN_PERF_ID);

    /* Perform Application Initialization */
    Status = HK_AppInit();
    if (Status != CFE_SUCCESS)
    {
        HK_AppData.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    /*
    ** Application Main Loop.
    */
    while (CFE_ES_RunLoop(&HK_AppData.RunStatus) == true)
    {
        /*
        ** Performance Log Exit Stamp.
        */
        CFE_ES_PerfLogExit(HK_APPMAIN_PERF_ID);

        /*
        ** Pend on the arrival of the next Software Bus message.
        */
        Status = CFE_SB_ReceiveBuffer(&BufPtr, HK_AppData.CmdPipe, HK_SB_TIMEOUT);

        if (Status == CFE_SUCCESS)
        {
            /*
            ** Performance Log Entry Stamp.
            */
            CFE_ES_PerfLogEntry(HK_APPMAIN_PERF_ID);

            /* Perform Message Processing */
            HK_AppPipe(BufPtr);
        }
        else if (Status == CFE_SB_TIME_OUT)
        {
            /* Check for copy table load and runtime dump request. This is
             * generally done during the housekeeping cycle.  If we are
             * getting routine messages at a rate of less than 1Hz we do
             * the routine maintenance here. */
            if (HK_CheckStatusOfTables() != HK_SUCCESS)
            {
                HK_AppData.RunStatus = CFE_ES_RunStatus_APP_ERROR;
            }
        }
        else
        {
            CFE_EVS_SendEvent(HK_RCV_MSG_ERR_EID, CFE_EVS_EventType_ERROR,
                              "HK_APP Exiting due to CFE_SB_RcvMsg error 0x%08X", (unsigned int)Status);

            /* Write to syslog in case there is a problem with event services */
            CFE_ES_WriteToSysLog("HK_APP Exiting due to CFE_SB_RcvMsg error 0x%08X\n", (unsigned int)Status);

            HK_AppData.RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }
    } /* end while */

    /*
     ** Performance Log Exit Stamp.
     */
    CFE_ES_PerfLogExit(HK_APPMAIN_PERF_ID);

    /*
    ** Exit the Application.
    */
    CFE_ES_ExitApp(HK_AppData.RunStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HK application initialization routine                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CFE_Status_t HK_AppInit(void)
{
    CFE_Status_t Status = CFE_SUCCESS;

    HK_AppData.RunStatus = CFE_ES_RunStatus_APP_RUN;

    /* Initialize housekeeping packet  */
    CFE_MSG_Init(CFE_MSG_PTR(HK_AppData.HkPacket.TelemetryHeader), CFE_SB_ValueToMsgId(HK_HK_TLM_MID),
                 sizeof(HK_HkPacket_t));

    /* Register for event services...        */
    Status = CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);
    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("HK: error registering for event services: 0x%08X\n", (unsigned int)Status);
        return Status;
    }

    /* Create HK Command Pipe */
    Status = CFE_SB_CreatePipe(&HK_AppData.CmdPipe, HK_PIPE_DEPTH, HK_PIPE_NAME);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HK_CR_PIPE_ERR_EID, CFE_EVS_EventType_ERROR, "Error Creating SB Pipe,RC=0x%08X",
                          (unsigned int)Status);
        return Status;
    }

    /* Subscribe to 'Send Combined HK Pkt' Command */
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(HK_SEND_COMBINED_PKT_MID), HK_AppData.CmdPipe);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HK_SUB_CMB_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Error Subscribing to HK Snd Cmb Pkt, MID=0x%08X, RC=0x%08X", HK_SEND_COMBINED_PKT_MID,
                          (unsigned int)Status);
        return Status;
    }

    /* Subscribe to Housekeeping Request */
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(HK_SEND_HK_MID), HK_AppData.CmdPipe);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HK_SUB_REQ_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Error Subscribing to HK Request, MID=0x%08X, RC=0x%08X", HK_SEND_HK_MID,
                          (unsigned int)Status);
        return Status;
    }

    /* Subscribe to HK ground commands */
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(HK_CMD_MID), HK_AppData.CmdPipe);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HK_SUB_CMD_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Error Subscribing to HK Gnd Cmds, MID=0x%08X, RC=0x%08X", HK_CMD_MID, (unsigned int)Status);
        return Status;
    }

    /* Create a memory pool for combined output messages */
    Status = CFE_ES_PoolCreate(&HK_AppData.MemPoolHandle, HK_AppData.MemPoolBuffer, sizeof(HK_AppData.MemPoolBuffer));
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HK_CR_POOL_ERR_EID, CFE_EVS_EventType_ERROR, "Error Creating Memory Pool,RC=0x%08X",
                          (unsigned int)Status);
        return Status;
    }

    HK_ResetHkData();

    /* Register The HK Tables */
    Status = HK_TableInit();
    if (Status != CFE_SUCCESS)
    {
        /* Specific failure is detailed in function HK_TableInit */
        return Status;
    }

    /* Application initialization event */
    Status = CFE_EVS_SendEvent(HK_INIT_INF_EID, CFE_EVS_EventType_INFORMATION, "HK Initialized.  Version %d.%d.%d.%d",
                               HK_MAJOR_VERSION, HK_MINOR_VERSION, HK_REVISION, HK_MISSION_REV);

    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("HK App:Error Sending Initialization Event,RC=0x%08X\n", (unsigned int)Status);
    }

    return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HK application table initialization routine                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CFE_Status_t HK_TableInit(void)
{
    CFE_Status_t Status = CFE_SUCCESS;

    /* Register The HK Copy Table */
    Status = CFE_TBL_Register(&HK_AppData.CopyTableHandle, HK_COPY_TABLE_NAME,
                              (sizeof(hk_copy_table_entry_t) * HK_COPY_TABLE_ENTRIES),
                              CFE_TBL_OPT_DBL_BUFFER | CFE_TBL_OPT_LOAD_DUMP, HK_ValidateHkCopyTable);

    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HK_CPTBL_REG_ERR_EID, CFE_EVS_EventType_ERROR, "Error Registering Copy Table,RC=0x%08X",
                          (unsigned int)Status);
        return Status;
    }

    /* Register The HK Runtime Table */
    Status = CFE_TBL_Register(&HK_AppData.RuntimeTableHandle, HK_RUNTIME_TABLE_NAME,
                              (sizeof(hk_runtime_tbl_entry_t) * HK_COPY_TABLE_ENTRIES),
                              CFE_TBL_OPT_SNGL_BUFFER | CFE_TBL_OPT_DUMP_ONLY, NULL);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HK_RTTBL_REG_ERR_EID, CFE_EVS_EventType_ERROR, "Error Registering Runtime Table,RC=0x%08X",
                          (unsigned int)Status);
        return Status;
    }

    Status = CFE_TBL_Load(HK_AppData.CopyTableHandle, CFE_TBL_SRC_FILE, HK_COPY_TABLE_FILENAME);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HK_CPTBL_LD_ERR_EID, CFE_EVS_EventType_ERROR, "Error Loading Copy Table,RC=0x%08X",
                          (unsigned int)Status);
        return Status;
    }

    Status = CFE_TBL_Manage(HK_AppData.CopyTableHandle);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HK_CPTBL_MNG_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Error from TBL Manage call for Copy Table,RC=0x%08X", (unsigned int)Status);
        return Status;
    }

    Status = CFE_TBL_Manage(HK_AppData.RuntimeTableHandle);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HK_RTTBL_MNG_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Error from TBL Manage call for Runtime Table,RC=0x%08X", (unsigned int)Status);
        return Status;
    }

    Status = CFE_TBL_GetAddress((void *)(&HK_AppData.CopyTablePtr), HK_AppData.CopyTableHandle);
    /* Status should be CFE_TBL_INFO_UPDATED because we loaded it above */
    if (Status != CFE_TBL_INFO_UPDATED)
    {
        CFE_EVS_SendEvent(HK_CPTBL_GADR_ERR_EID, CFE_EVS_EventType_ERROR, "Error Getting Adr for Cpy Tbl,RC=0x%08X",
                          (unsigned int)Status);
        return Status;
    }

    Status = CFE_TBL_GetAddress((void *)(&HK_AppData.RuntimeTablePtr), HK_AppData.RuntimeTableHandle);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HK_RTTBL_GADR_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Error Getting Adr for Runtime Table,RC=0x%08X", (unsigned int)Status);
        return Status;
    }

    Status = HK_ProcessNewCopyTable(HK_AppData.CopyTablePtr, HK_AppData.RuntimeTablePtr);

    if (Status != CFE_SUCCESS)
    {
        /* If status was not success, then the copy table function received a NULL pointer argument */
        CFE_EVS_SendEvent(HK_NEWCPYTBL_INIT_FAILED_EID, CFE_EVS_EventType_ERROR,
                          "Process New Copy Table Failed, status = 0x%08X", (unsigned int)Status);

        return Status;
    }

    return CFE_SUCCESS;
} /* HK_TableInit */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send Combined Housekeeping Packet                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HK_SendCombinedPktCmd(const CFE_SB_Buffer_t *BufPtr)
{
    const HK_SendCombinedPkt_Payload_t *CmdPtr;

    CmdPtr = &((const HK_SendCombinedPktCmd_t *)BufPtr)->Payload;

    HK_SendCombinedHkPacket(CmdPtr->OutMsgToSend);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Housekeeping request                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HK_SendHkCmd(const CFE_SB_Buffer_t *BufPtr)
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
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Noop command                                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HK_NoopCmd(const CFE_SB_Buffer_t *BufPtr)
{
    CFE_EVS_SendEvent(HK_NOOP_INF_EID, CFE_EVS_EventType_INFORMATION, "HK No-op command, Version %d.%d.%d.%d",
                      HK_MAJOR_VERSION, HK_MINOR_VERSION, HK_REVISION, HK_MISSION_REV);

    HK_AppData.CmdCounter++;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Reset counters command                                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HK_ResetCountersCmd(const CFE_SB_Buffer_t *BufPtr)
{
    HK_ResetHkData();
    CFE_EVS_SendEvent(HK_RESET_INF_EID, CFE_EVS_EventType_INFORMATION, "HK Reset Counters command received");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Reset housekeeping data                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HK_ResetHkData(void)
{
    HK_AppData.CmdCounter          = 0;
    HK_AppData.ErrCounter          = 0;
    HK_AppData.CombinedPacketsSent = 0;
    HK_AppData.MissingDataCtr      = 0;
}

/************************/
/*  End of File Comment */
/************************/
