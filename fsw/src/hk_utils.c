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
 *  The CFS Housekeeping (HK) Application file containing the functions
 *  used combine the input messages into output messages.
 */

/************************************************************************
** Includes
*************************************************************************/

#include "cfe.h"
#include "hk_utils.h"
#include "hk_app.h"
#include "hk_events.h"
#include <string.h>

/*************************************************************************
** Function definitions
**************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HK process incoming housekeeping data                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HK_ProcessIncomingHkData(const CFE_SB_Buffer_t *BufPtr)
{
    hk_copy_table_entry_t * StartOfCopyTable = HK_AppData.CopyTablePtr;
    hk_copy_table_entry_t * CpyTblEntry      = NULL;
    hk_runtime_tbl_entry_t *StartOfRtTable   = HK_AppData.RuntimeTablePtr;
    hk_runtime_tbl_entry_t *RtTblEntry       = NULL;
    uint16                  Loop             = 0;
    CFE_SB_MsgId_t          MessageID        = CFE_SB_INVALID_MSG_ID;
    uint8 *                 DestPtr          = NULL;
    uint8 *                 SrcPtr           = NULL;
    size_t                  MessageLength    = 0;
    int32                   MessageErrors    = 0;
    int32                   LastByteAccessed;

    CFE_MSG_GetMsgId(&BufPtr->Msg, &MessageID);

    /* Spin thru the entire table looking for matches */
    for (Loop = 0; Loop < HK_COPY_TABLE_ENTRIES; Loop++)
    {
        CpyTblEntry = &StartOfCopyTable[Loop];
        RtTblEntry  = &StartOfRtTable[Loop];

        /* Does the inputMID for this table entry match what we're looking for */
        if (CFE_SB_MsgId_Equal(MessageID, CpyTblEntry->InputMid))
        {
            /* Ensure that we don't reference past the end of the input packet */
            CFE_MSG_GetSize(&BufPtr->Msg, &MessageLength);
            LastByteAccessed = CpyTblEntry->InputOffset + CpyTblEntry->NumBytes;
            if (MessageLength >= LastByteAccessed)
            {
                /* We have a match.  Build the Source and Destination addresses
                   and move the data */
                DestPtr = ((uint8 *)RtTblEntry->OutputPktAddr) + CpyTblEntry->OutputOffset;
                SrcPtr  = ((uint8 *)BufPtr) + CpyTblEntry->InputOffset;

                memcpy(DestPtr, SrcPtr, CpyTblEntry->NumBytes);

                /* Set the data present field to indicate the data is there */
                RtTblEntry->DataPresent = HK_DATA_PRESENT;
            }
            else
            {
                /* Error: copy data is past the end of the input packet */
                MessageErrors++;
            }
        }
    }

    /* Send, at most, one error event per input packet */
    if (MessageErrors != 0)
    {
        CFE_EVS_SendEvent(HK_ACCESSING_PAST_PACKET_END_EID, CFE_EVS_EventType_ERROR,
                          "HK table definition exceeds packet length. MID:0x%08lX, Length:%d, Count:%d",
                          (unsigned long)CFE_SB_MsgIdToValue(MessageID), (int)MessageLength, (int)MessageErrors);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HK validate the copy table contents                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HK_ValidateHkCopyTable(void *TblPtr)
{
    int32                  HKStatus;
    int32                  i        = 0;
    int32                  sumBytes = 0;
    hk_copy_table_entry_t *tbl      = (hk_copy_table_entry_t *)TblPtr;

    /* Loop thru the table and add up all the bytes copied for testing overflow scenario */
    for (i = 0; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        sumBytes += tbl[i].NumBytes;
    }

    /* Check if the accumulated bytes exceed the allowed packet size, indicating an overflow */
    if (sumBytes > HK_MAX_COMBINED_PACKET_SIZE)
    {
        HKStatus = HK_ERROR;

        CFE_EVS_SendEvent(HK_NEWCPYTBL_HK_FAILED_EID, CFE_EVS_EventType_ERROR,
                          "HK Validate: table contents has size %d > %d\n", (int)sumBytes, HK_MAX_COMBINED_PACKET_SIZE);
    }
    else
    {
        HKStatus = HK_SUCCESS;
    }

    return HKStatus;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HK process new copy table                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CFE_Status_t HK_ProcessNewCopyTable(hk_copy_table_entry_t *CpyTblPtr, hk_runtime_tbl_entry_t *RtTblPtr)
{
    hk_copy_table_entry_t * StartOfCopyTable = NULL;
    hk_copy_table_entry_t * OuterCpyEntry    = NULL;
    hk_copy_table_entry_t * InnerDefEntry    = NULL;
    hk_runtime_tbl_entry_t *StartOfRtTable   = NULL;
    hk_runtime_tbl_entry_t *OuterRtEntry     = NULL;
    hk_runtime_tbl_entry_t *InnerRtEntry     = NULL;
    int32                   Loop1            = 0;
    int32                   Loop2;
    CFE_SB_MsgId_t          MidOfThisPacket;
    int32                   SizeOfThisPacket;
    int32                   FurthestByteFromThisEntry;
    CFE_SB_Buffer_t *       NewPacketAddr;
    CFE_Status_t            Result;

    /* Ensure that the input arguments are valid */
    if (((void *)CpyTblPtr == NULL) || ((void *)RtTblPtr == NULL))
    {
        CFE_EVS_SendEvent(HK_NULL_POINTER_NEWCPY_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Null pointer detected in new copy tbl processing: CpyTbl = %p, RtTbl = %p",
                          (void *)CpyTblPtr, (void *)RtTblPtr);
        return HK_NULL_POINTER_DETECTED;
    }

    StartOfCopyTable = CpyTblPtr;
    StartOfRtTable   = RtTblPtr;

    /* Loop thru the RunTime table initializing the fields */
    for (Loop1 = 0; Loop1 < HK_COPY_TABLE_ENTRIES; Loop1++)
    {
        OuterRtEntry = &StartOfRtTable[Loop1];

        OuterRtEntry->OutputPktAddr      = NULL;
        OuterRtEntry->InputMidSubscribed = HK_INPUTMID_NOT_SUBSCRIBED;
        OuterRtEntry->DataPresent        = HK_DATA_NOT_PRESENT;
    }

    /* Loop thru the table looking for all of the SB packets that need to be built */
    for (Loop1 = 0; Loop1 < HK_COPY_TABLE_ENTRIES; Loop1++)
    {
        OuterCpyEntry = &StartOfCopyTable[Loop1];
        OuterRtEntry  = &StartOfRtTable[Loop1];

        /* If the both MIDs exists but the Packet Address has yet to be assigned,
           we need to build an SB packet, so compute the size */
        if (CFE_SB_IsValidMsgId(OuterCpyEntry->OutputMid) && CFE_SB_IsValidMsgId(OuterCpyEntry->InputMid) &&
            (OuterRtEntry->OutputPktAddr == NULL))
        {
            /* We have a table entry that needs a SB message to be built */
            MidOfThisPacket  = OuterCpyEntry->OutputMid;
            SizeOfThisPacket = 0;

            /* Spin thru entire table looking for duplicate OutputMid's.  This will let
               us find the byte offset furthest from the beginning of the packet */
            for (Loop2 = 0; Loop2 < HK_COPY_TABLE_ENTRIES; Loop2++)
            {
                InnerDefEntry = &StartOfCopyTable[Loop2];

                /* If this entry's MID matches the one we're looking for */
                if (CFE_SB_MsgId_Equal(InnerDefEntry->OutputMid, MidOfThisPacket))
                {
                    /* The byte furthest away from the section described by this entry */
                    FurthestByteFromThisEntry = InnerDefEntry->OutputOffset + InnerDefEntry->NumBytes;

                    /* Save the byte offset of the byte furthest from the packet start */
                    if (FurthestByteFromThisEntry > SizeOfThisPacket)
                    {
                        SizeOfThisPacket = FurthestByteFromThisEntry;
                    }
                }
            }

            /* Build the packet with the size computed above */
            NewPacketAddr = NULL;
            if (SizeOfThisPacket > 0)
            {
                Result = CFE_ES_GetPoolBuf((void **)&NewPacketAddr, HK_AppData.MemPoolHandle, SizeOfThisPacket);

                if (Result >= CFE_SUCCESS)
                {
                    /* Spin thru entire table (again) looking for duplicate OutputMid's.
                        This will let us assign the packet created above to all
                        of the table entries that need to use it */
                    for (Loop2 = 0; Loop2 < HK_COPY_TABLE_ENTRIES; Loop2++)
                    {
                        InnerDefEntry = &StartOfCopyTable[Loop2];
                        InnerRtEntry  = &StartOfRtTable[Loop2];

                        /* If this entry's MID matches the one we're looking for */
                        if (CFE_SB_MsgId_Equal(InnerDefEntry->OutputMid, MidOfThisPacket))
                        {
                            InnerRtEntry->OutputPktAddr = NewPacketAddr;
                        }
                    }

                    /* Init the SB Packet only once regardless of how many times its in the table */
                    CFE_MSG_Init(&NewPacketAddr->Msg, MidOfThisPacket, SizeOfThisPacket);
                }
                else
                {
                    CFE_EVS_SendEvent(HK_MEM_POOL_MALLOC_FAILED_EID, CFE_EVS_EventType_ERROR,
                                      "HK Processing New Table: ES_GetPoolBuf for size %d returned 0x%04X",
                                      (int)SizeOfThisPacket, (unsigned int)Result);
                }
            }
        }

        /* If HK needs to subscribe to this Input packet... */
        if ((OuterRtEntry->InputMidSubscribed == HK_INPUTMID_NOT_SUBSCRIBED) &&
            CFE_SB_IsValidMsgId(OuterCpyEntry->InputMid))
        {
            Result = CFE_SB_Subscribe(OuterCpyEntry->InputMid, HK_AppData.CmdPipe);

            if (Result == CFE_SUCCESS)
            {
                /* Spin thru entire table (again) looking for duplicate InputMid's.
                   This will let us mark each duplicate as already having been subscribed */
                for (Loop2 = 0; Loop2 < HK_COPY_TABLE_ENTRIES; Loop2++)
                {
                    InnerDefEntry = &StartOfCopyTable[Loop2];
                    InnerRtEntry  = &StartOfRtTable[Loop2];

                    /* If this entry's MID matches the one we're looking for */
                    if (CFE_SB_MsgId_Equal(OuterCpyEntry->InputMid, InnerDefEntry->InputMid))
                    {
                        InnerRtEntry->InputMidSubscribed = HK_INPUTMID_SUBSCRIBED;
                    }
                }
            }
            else
            {
                CFE_EVS_SendEvent(HK_CANT_SUBSCRIBE_TO_SB_PKT_EID, CFE_EVS_EventType_ERROR,
                                  "HK Processing New Table:SB_Subscribe for Mid 0x%08lX returned 0x%04X",
                                  (unsigned long)CFE_SB_MsgIdToValue(OuterCpyEntry->InputMid), (unsigned int)Result);
            }
        }
    }

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HK Tear down old copy table                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CFE_Status_t HK_TearDownOldCopyTable(hk_copy_table_entry_t *CpyTblPtr, hk_runtime_tbl_entry_t *RtTblPtr)
{
    hk_copy_table_entry_t * StartOfCopyTable = NULL;
    hk_copy_table_entry_t * OuterCpyEntry    = NULL;
    hk_copy_table_entry_t * InnerDefEntry    = NULL;
    hk_runtime_tbl_entry_t *StartOfRtTable   = NULL;
    hk_runtime_tbl_entry_t *OuterRtEntry     = NULL;
    hk_runtime_tbl_entry_t *InnerRtEntry     = NULL;
    int32                   Loop1            = 0;
    int32                   Loop2;
    CFE_SB_MsgId_t          MidOfThisPacket;
    void *                  OutputPktAddr = NULL;
    void *                  SavedPktAddr  = NULL;
    CFE_Status_t            Result;

    /* Ensure that the input arguments are valid */
    if (((void *)CpyTblPtr == NULL) || ((void *)RtTblPtr == NULL))
    {
        CFE_EVS_SendEvent(HK_NULL_POINTER_TEARCPY_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Null pointer detected in copy tbl tear down: CpyTbl = %p, RtTbl = %p", (void *)CpyTblPtr,
                          (void *)RtTblPtr);
        return HK_NULL_POINTER_DETECTED;
    }

    StartOfCopyTable = CpyTblPtr;
    StartOfRtTable   = RtTblPtr;

    /* Loop thru the table looking for all of the SB packets that need to be freed */
    for (Loop1 = 0; Loop1 < HK_COPY_TABLE_ENTRIES; Loop1++)
    {
        OuterCpyEntry = &StartOfCopyTable[Loop1];
        OuterRtEntry  = &StartOfRtTable[Loop1];

        /* If a Packet Address has been assigned, it needs to get deleted */
        if (OuterRtEntry->OutputPktAddr != NULL)
        {
            OutputPktAddr   = OuterRtEntry->OutputPktAddr;
            MidOfThisPacket = OuterCpyEntry->OutputMid;

            SavedPktAddr = OutputPktAddr;
            Result       = CFE_ES_PutPoolBuf(HK_AppData.MemPoolHandle, (uint32 *)OutputPktAddr);
            if (Result >= CFE_SUCCESS)
            {
                /* Spin thru the entire table looking for entries that used the same SB packets */
                for (Loop2 = 0; Loop2 < HK_COPY_TABLE_ENTRIES; Loop2++)
                {
                    InnerDefEntry = &StartOfCopyTable[Loop2];
                    InnerRtEntry  = &StartOfRtTable[Loop2];

                    if (CFE_SB_MsgId_Equal(InnerDefEntry->OutputMid, MidOfThisPacket) &&
                        (InnerRtEntry->OutputPktAddr == SavedPktAddr))
                    {
                        /* NULL out the table entry whose packet was freed above */
                        InnerRtEntry->OutputPktAddr = (CFE_SB_Buffer_t *)NULL;
                    }
                }
            }
            else
            {
                CFE_EVS_SendEvent(HK_MEM_POOL_FREE_FAILED_EID, CFE_EVS_EventType_ERROR,
                                  "HK TearDown: ES_putPoolBuf Err pkt:0x%08lX ret 0x%04X, hdl 0x%08lx",
                                  (unsigned long)SavedPktAddr, (unsigned int)Result,
                                  CFE_RESOURCEID_TO_ULONG(HK_AppData.MemPoolHandle));
            }
        }

        /* If the InputMid for this Table Entry has been subscribed, it needs to
           get Unsubscribed as do any other identical InputMids throughout the table.
           We don't have to worry about leaving any Mid's subscribed since the entire table
           is getting clobbered. */
        if (OuterRtEntry->InputMidSubscribed == HK_INPUTMID_SUBSCRIBED)
        {
            CFE_SB_Unsubscribe(OuterCpyEntry->InputMid, HK_AppData.CmdPipe);

            /* Spin thru the entire table looking for entries that used the same SB packets */
            for (Loop2 = 0; Loop2 < HK_COPY_TABLE_ENTRIES; Loop2++)
            {
                InnerDefEntry = &StartOfCopyTable[Loop2];
                InnerRtEntry  = &StartOfRtTable[Loop2];

                if (CFE_SB_MsgId_Equal(InnerDefEntry->InputMid, OuterCpyEntry->InputMid))
                {
                    InnerRtEntry->InputMidSubscribed = HK_INPUTMID_NOT_SUBSCRIBED;
                }
            }
        }
    }

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HK Send combined output message                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HK_SendCombinedHkPacket(CFE_SB_MsgId_t WhichMidToSend)
{
    bool                    PacketFound      = false;
    hk_runtime_tbl_entry_t *StartOfRtTable   = HK_AppData.RuntimeTablePtr;
    hk_runtime_tbl_entry_t *RtTblEntry       = NULL;
    int32                   Loop             = 0;
    CFE_SB_MsgId_t          ThisEntrysOutMid = CFE_SB_INVALID_MSG_ID;
    CFE_SB_MsgId_t          InputMidMissing  = CFE_SB_INVALID_MSG_ID;
    CFE_SB_Buffer_t *       OutBuffer        = NULL;

    /* Look thru each item in this Table, but only send this packet once, at most */
    for (Loop = 0; ((Loop < HK_COPY_TABLE_ENTRIES) && (PacketFound == false)); Loop++)
    {
        RtTblEntry = &StartOfRtTable[Loop];

        /* Empty table entries are defined by NULL's in this field */
        if (RtTblEntry->OutputPktAddr != NULL)
        {
            OutBuffer = (CFE_SB_Buffer_t *)RtTblEntry->OutputPktAddr;
            CFE_MSG_GetMsgId(&OutBuffer->Msg, &ThisEntrysOutMid);

            if (CFE_SB_MsgId_Equal(ThisEntrysOutMid, WhichMidToSend))
            {
                if (HK_CheckForMissingData(ThisEntrysOutMid, &InputMidMissing) == HK_MISSING_DATA_DETECTED)
                {
                    HK_AppData.MissingDataCtr++;

                    CFE_EVS_SendEvent(HK_OUTPKT_MISSING_DATA_EID, CFE_EVS_EventType_DEBUG,
                                      "Combined Packet 0x%08lX missing data from Input Pkt 0x%08lX",
                                      (unsigned long)CFE_SB_MsgIdToValue(ThisEntrysOutMid),
                                      (unsigned long)CFE_SB_MsgIdToValue(InputMidMissing));
                }
#if HK_DISCARD_INCOMPLETE_COMBO == 1
                else /* This clause is only exclusive if discarding incomplete packets */
#endif
                {
                    /* Send the combined housekeeping telemetry packet...        */
                    CFE_SB_TimeStampMsg(&OutBuffer->Msg);
                    CFE_SB_TransmitMsg(&OutBuffer->Msg, true);

                    HK_AppData.CombinedPacketsSent++;
                }

                HK_SetFlagsToNotPresent(ThisEntrysOutMid);

                PacketFound = true;
            }
        }
    }

    if (PacketFound == false)
    {
        CFE_EVS_SendEvent(HK_UNKNOWN_COMBINED_PACKET_EID, CFE_EVS_EventType_INFORMATION,
                          "Combined HK Packet 0x%08lX is not found in current HK Copy Table",
                          (unsigned long)CFE_SB_MsgIdToValue(WhichMidToSend));
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Check the status of HK tables and perform any necessary action. */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HK_CheckStatusOfTables(void)
{
    int32 HKStatus;

    HKStatus = HK_CheckStatusOfCopyTable();

    /* Only check the Dump Table if there were no problems with the Copy Table */
    if (HKStatus == HK_SUCCESS)
    {
        HKStatus = HK_CheckStatusOfDumpTable();
    }

    return HKStatus;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                     */
/* Check the status of HK Copy table and perform any necessary action. */
/*                                                                     */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HK_CheckStatusOfCopyTable(void)
{
    int32 Status;
    int32 HKStatus = HK_ERROR; /* Assume failure */

    /* Determine if the copy table has a validation or update that needs to be performed */
    Status = CFE_TBL_GetStatus(HK_AppData.CopyTableHandle);

    if (Status == CFE_TBL_INFO_VALIDATION_PENDING)
    {
        /* Validate the specified Table */
        Status = CFE_TBL_Validate(HK_AppData.CopyTableHandle);

        if (Status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(HK_UNEXPECTED_TBLVLD_RET_EID, CFE_EVS_EventType_ERROR,
                              "Unexpected CFE_TBL_Validate return (0x%08X) for Copy Table", (unsigned int)Status);
        }
        HKStatus = HK_SUCCESS; /* This could just be a bad table, so no reason to abort the app */
    }
    else if (Status == CFE_TBL_INFO_UPDATE_PENDING)
    {
        /* Unsubscribe to input msgs and free out pkt buffers */
        /* If the copy table pointer is bad, that's okay here because it will be re-allocated later.
           If the runtime table pointer is bad, the process new copy table call later on will
           flag the error.  So we can ignore the return status at this point.
        */
        HK_TearDownOldCopyTable(HK_AppData.CopyTablePtr, HK_AppData.RuntimeTablePtr);

        /* release address must be called for update to take. */
        Status = CFE_TBL_ReleaseAddress(HK_AppData.CopyTableHandle);

        /* Releasing the address should only return CFE_SUCCESS at this point since we
           already had an update-pending response earlier */
        if (Status == CFE_SUCCESS)
        {
            /* Update the copy table */
            Status = CFE_TBL_Update(HK_AppData.CopyTableHandle);

            if (Status == CFE_SUCCESS)
            {
                /* Get address of the newly updated copy table. */
                Status = CFE_TBL_GetAddress((void *)(&HK_AppData.CopyTablePtr), HK_AppData.CopyTableHandle);

                /* Status should only be CFE_TBL_INFO_UPDATED because we updated it above */
                if (Status == CFE_TBL_INFO_UPDATED)
                {
                    Status = HK_ProcessNewCopyTable(HK_AppData.CopyTablePtr, HK_AppData.RuntimeTablePtr);

                    if (Status == CFE_SUCCESS)
                    {
                        HKStatus = HK_SUCCESS;
                    }
                    else
                    {
                        /* If status was not success, then the copy table function received a NULL pointer argument */
                        CFE_EVS_SendEvent(HK_NEWCPYTBL_HK_FAILED_EID, CFE_EVS_EventType_CRITICAL,
                                          "Process New Copy Table Failed, status = 0x%08X", (unsigned int)Status);
                    }
                }
                else
                {
                    CFE_EVS_SendEvent(HK_UNEXPECTED_GETADDR_RET_EID, CFE_EVS_EventType_CRITICAL,
                                      "Unexpected CFE_TBL_GetAddress return (0x%08X) for Copy Table",
                                      (unsigned int)Status);
                }
            }
            else
            {
                CFE_EVS_SendEvent(HK_UNEXPECTED_TBLUPD_RET_EID, CFE_EVS_EventType_CRITICAL,
                                  "Unexpected CFE_TBL_Update return (0x%08X) for Copy Table", (unsigned int)Status);
            }
        }
        else
        {
            CFE_EVS_SendEvent(HK_UNEXPECTED_RELADDR_RET_EID, CFE_EVS_EventType_CRITICAL,
                              "Unexpected CFE_TBL_ReleaseAddress return (0x%08X) for Copy Table", (unsigned int)Status);
        }
    }
    else if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HK_UNEXPECTED_GETSTAT_RET_EID, CFE_EVS_EventType_CRITICAL,
                          "Unexpected CFE_TBL_GetStatus return (0x%08X) for Copy Table", (unsigned int)Status);
    }
    else
    {
        /* Only way to get here is if Status == CFE_SUCCESS */
        HKStatus = HK_SUCCESS;
    }

    return HKStatus;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                     */
/* Check the status of HK Dump table and perform any necessary action. */
/*                                                                     */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HK_CheckStatusOfDumpTable(void)
{
    int32 Status;
    int32 HKStatus = HK_ERROR; /* Assume failure */

    /* Determine if the runtime table has a dump pending */
    Status = CFE_TBL_GetStatus(HK_AppData.RuntimeTableHandle);

    if (Status == CFE_TBL_INFO_DUMP_PENDING)
    {
        /* Dump the specified Table, cfe tbl manager makes copy */
        Status = CFE_TBL_DumpToBuffer(HK_AppData.RuntimeTableHandle);

        if (Status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(HK_UNEXPECTED_DUMPTOBUFFER_RET_EID, CFE_EVS_EventType_CRITICAL,
                              "Unexpected CFE_TBL_DumpToBuffer return (0x%08X) for Runtime Table",
                              (unsigned int)Status);
        }
        else
        {
            HKStatus = HK_SUCCESS;
        }
    }
    else if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HK_UNEXPECTED_GETSTAT2_RET_EID, CFE_EVS_EventType_CRITICAL,
                          "Unexpected CFE_TBL_GetStatus return (0x%08X) for Runtime Table", (unsigned int)Status);
    }
    else
    {
        /* Only way to get here is if Status == CFE_SUCCESS */
        HKStatus = HK_SUCCESS;
    }

    return HKStatus;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HK Check for missing combined output message data               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HK_CheckForMissingData(CFE_SB_MsgId_t OutPktToCheck, CFE_SB_MsgId_t *MissingInputMid)
{
    int32                   Loop             = 0;
    int32                   Status           = HK_NO_MISSING_DATA;
    hk_copy_table_entry_t * StartOfCopyTable = HK_AppData.CopyTablePtr;
    hk_copy_table_entry_t * CpyTblEntry      = NULL;
    hk_runtime_tbl_entry_t *StartOfRtTable   = HK_AppData.RuntimeTablePtr;
    hk_runtime_tbl_entry_t *RtTblEntry       = NULL;

    /* Loop thru each item in the runtime table until end is reached or
     * data-not-present detected */
    do
    {
        CpyTblEntry = &StartOfCopyTable[Loop];
        RtTblEntry  = &StartOfRtTable[Loop];

        /* Empty table entries are defined by NULL's in this field */
        if ((RtTblEntry->OutputPktAddr != NULL) && CFE_SB_MsgId_Equal(CpyTblEntry->OutputMid, OutPktToCheck) &&
            (RtTblEntry->DataPresent == HK_DATA_NOT_PRESENT))
        {
            *MissingInputMid = CpyTblEntry->InputMid;
            Status           = HK_MISSING_DATA_DETECTED;
        }

        Loop++;

    } while ((Loop < HK_COPY_TABLE_ENTRIES) && (Status == HK_NO_MISSING_DATA));

    return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HK Set data present flags to 'data-not-present'                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HK_SetFlagsToNotPresent(CFE_SB_MsgId_t OutPkt)
{
    int32                   Loop             = 0;
    hk_copy_table_entry_t * StartOfCopyTable = HK_AppData.CopyTablePtr;
    hk_copy_table_entry_t * CpyTblEntry      = NULL;
    hk_runtime_tbl_entry_t *StartOfRtTable   = HK_AppData.RuntimeTablePtr;
    hk_runtime_tbl_entry_t *RtTblEntry       = NULL;

    /* Look thru each item in the runtime table until end is reached */
    for (Loop = 0; Loop < HK_COPY_TABLE_ENTRIES; Loop++)
    {
        CpyTblEntry = &StartOfCopyTable[Loop];
        RtTblEntry  = &StartOfRtTable[Loop];

        /* Empty table entries are defined by NULL's in this field */
        if ((RtTblEntry->OutputPktAddr != NULL) && CFE_SB_MsgId_Equal(CpyTblEntry->OutputMid, OutPkt))
        {
            RtTblEntry->DataPresent = HK_DATA_NOT_PRESENT;
        }
    }
}

/************************/
/*  End of File Comment */
/************************/
