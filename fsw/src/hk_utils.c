/************************************************************************
** File: hk_utils.c 
**
** NASA Docket No. GSC-16,127-1, and identified as "Core Flight Software System
** (CFS) Housekeeping Application Version 2” 
**
** Copyright © 2007-2014 United States Government as represented by the
** Administrator of the National Aeronautics and Space Administration. All Rights
** Reserved. 
**
** Licensed under the Apache License, Version 2.0 (the "License"); 
** you may not use this file except in compliance with the License. 
** You may obtain a copy of the License at 
** http://www.apache.org/licenses/LICENSE-2.0 
** 
** Unless required by applicable law or agreed to in writing, software 
** distributed under the License is distributed on an "AS IS" BASIS, 
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
** See the License for the specific language governing permissions and 
** limitations under the License. 
**
** Purpose: 
**  The CFS Housekeeping (HK) Application file containing the functions
**  used combine the input messages into output messages.
**
** Notes:
**
**
*************************************************************************/

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
void HK_ProcessIncomingHkData (CFE_SB_MsgPtr_t MessagePtr)
{
    hk_copy_table_entry_t         * StartOfCopyTable = NULL;
    hk_copy_table_entry_t         * CpyTblEntry      = NULL;
    hk_runtime_tbl_entry_t        * StartOfRtTable   = NULL;
    hk_runtime_tbl_entry_t        * RtTblEntry       = NULL;
    uint16                          Loop             = 0;
    CFE_SB_MsgId_t                  MessageID        = 0xFFFF;
    uint8                         * DestPtr          = NULL;
    uint8                         * SrcPtr           = NULL;
    int32                           MessageLength    = 0;
    int32                           MessageErrors    = 0;
    int32                           LastByteAccessed = 0;


    StartOfCopyTable = (hk_copy_table_entry_t *)  HK_AppData.CopyTablePtr;
    StartOfRtTable   = (hk_runtime_tbl_entry_t *) HK_AppData.RuntimeTablePtr;
    MessageID        = CFE_SB_GetMsgId (MessagePtr);
    MessageErrors    = 0;
    
    /* Spin thru the entire table looking for matches */
    for (Loop=0; Loop < HK_COPY_TABLE_ENTRIES; Loop++)
    {
        CpyTblEntry = & StartOfCopyTable [Loop];
        RtTblEntry  = & StartOfRtTable [Loop];
        
        /* Does the inputMID for this table entry match what we're looking for */       
        if (MessageID == CpyTblEntry->InputMid)
        {
            /* Ensure that we don't reference past the end of the input packet */
            MessageLength    = CFE_SB_GetTotalMsgLength(MessagePtr);
            LastByteAccessed = CpyTblEntry->InputOffset + CpyTblEntry->NumBytes;
            if (MessageLength >= LastByteAccessed)
            {
                /* We have a match.  Build the Source and Destination addresses
                   and move the data */
                DestPtr = ( (uint8 *) RtTblEntry->OutputPktAddr) + CpyTblEntry->OutputOffset;
                SrcPtr  = ( (uint8 *) MessagePtr) + CpyTblEntry->InputOffset;

                CFE_PSP_MemCpy (DestPtr, SrcPtr, CpyTblEntry->NumBytes);
                
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
        CFE_EVS_SendEvent (HK_ACCESSING_PAST_PACKET_END_EID, CFE_EVS_ERROR,
                           "HK table definition exceeds packet length. MID:0x%04X, Length:%d, Count:%d",
                           MessageID, (int)MessageLength, (int)MessageErrors);
    }

}   /* end HK_ProcessIncomingHkData */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HK validate the copy table contents                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HK_ValidateHkCopyTable (void * TblPtr)
{
    return HK_SUCCESS;

}   /* end HK_ValidateHkCopyTable */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HK process new copy table                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HK_ProcessNewCopyTable (hk_copy_table_entry_t * CpyTblPtr, 
                              hk_runtime_tbl_entry_t * RtTblPtr)
{
    hk_copy_table_entry_t         * StartOfCopyTable          = NULL;
    hk_copy_table_entry_t         * OuterCpyEntry             = NULL;
    hk_copy_table_entry_t         * InnerDefEntry             = NULL;
    hk_runtime_tbl_entry_t        * StartOfRtTable            = NULL;
    hk_runtime_tbl_entry_t        * OuterRtEntry              = NULL;
    hk_runtime_tbl_entry_t        * InnerRtEntry              = NULL;
    int32                           Loop1                     = 0;
    int32                           Loop2                     = 0;
    CFE_SB_MsgId_t                  MidOfThisPacket           = 0xFFFF;
    int32                           SizeOfThisPacket          = 0;
    int32                           FurthestByteFromThisEntry = 0;
    CFE_SB_MsgPtr_t                 NewPacketAddr             = 0;
    int32                           Result                    = CFE_SUCCESS;

    /* Ensure that the input arguments are valid */
    if ( ((void *)CpyTblPtr == NULL) || ((void *)RtTblPtr == NULL) )
    {
        CFE_EVS_SendEvent( HK_NULL_POINTER_NEWCPY_ERR_EID, CFE_EVS_DEBUG,
                           "Null pointer detected in new copy tbl processing: CpyTbl = 0x%08X, RtTbl = 0x%08X",
                           (int)((void *)CpyTblPtr), (int)((void *)RtTblPtr) );
        return ( HK_NULL_POINTER_DETECTED );
    }
     
    StartOfCopyTable = CpyTblPtr;
    StartOfRtTable  = RtTblPtr;
    
    /* Loop thru the RunTime table initializing the fields */
    for (Loop1 = 0; Loop1 < HK_COPY_TABLE_ENTRIES; Loop1++)
    {
        OuterRtEntry  = & StartOfRtTable [Loop1];

        OuterRtEntry->OutputPktAddr      = NULL;
        OuterRtEntry->InputMidSubscribed = HK_INPUTMID_NOT_SUBSCRIBED;
        OuterRtEntry->DataPresent        = HK_DATA_NOT_PRESENT;
        
    }

    /* Loop thru the table looking for all of the SB packets that need to be built */
    for (Loop1 = 0; Loop1 < HK_COPY_TABLE_ENTRIES; Loop1++)
    {
        OuterCpyEntry = & StartOfCopyTable [Loop1];
        OuterRtEntry  = & StartOfRtTable [Loop1];

        /* If the both MIDs exists but the Packet Address has yet to be assigned, 
           we need to build an SB packet, so compute the size */
        if ( (OuterCpyEntry->OutputMid     != HK_UNDEFINED_ENTRY) && 
             (OuterCpyEntry->InputMid      != HK_UNDEFINED_ENTRY) &&
             (OuterRtEntry->OutputPktAddr  == NULL) )
        {
            /* We have a table entry that needs a SB message to be built */        
            MidOfThisPacket  = OuterCpyEntry->OutputMid;
            SizeOfThisPacket = 0;

            /* Spin thru entire table looking for duplicate OutputMid's.  This will let 
               us find the byte offset furthest from the beginning of the packet */
            for (Loop2=0; Loop2 < HK_COPY_TABLE_ENTRIES; Loop2++)
            {
                InnerDefEntry = & StartOfCopyTable [Loop2];
                
                /* If this entry's MID matches the one we're looking for */
                if (InnerDefEntry->OutputMid == MidOfThisPacket)
                {
                    /* The byte furthest away from the section described by this entry */
                    FurthestByteFromThisEntry = InnerDefEntry->OutputOffset +
                                                InnerDefEntry->NumBytes;

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
                Result = CFE_ES_GetPoolBuf ((uint32 **) & NewPacketAddr,
                                            HK_AppData.MemPoolHandle,
                                            SizeOfThisPacket);
                
                if (Result >= CFE_SUCCESS)
                {
                   /* Spin thru entire table (again) looking for duplicate OutputMid's.  
                       This will let us assign the packet created above to all 
                       of the table entries that need to use it */
                    for (Loop2=0; Loop2 < HK_COPY_TABLE_ENTRIES; Loop2++)
                    {
                        InnerDefEntry = & StartOfCopyTable [Loop2];
                        InnerRtEntry  = & StartOfRtTable [Loop2];
                
                        /* If this entry's MID matches the one we're looking for */
                        if (InnerDefEntry->OutputMid == MidOfThisPacket)
                        {
                            InnerRtEntry->OutputPktAddr = NewPacketAddr;
                        }
                    }

                    /* Init the SB Packet only once regardless of how many times its in the table */
                    CFE_SB_InitMsg (NewPacketAddr, MidOfThisPacket, SizeOfThisPacket, TRUE);                    
                    
                }
                else
                {
                    CFE_EVS_SendEvent (HK_MEM_POOL_MALLOC_FAILED_EID, CFE_EVS_ERROR,
                                       "HK Processing New Table: ES_GetPoolBuf for size %d returned 0x%04X",
                                       (int)SizeOfThisPacket, (unsigned int)Result);
                }
            }
        }

        /* If HK needs to subscribe to this Input packet... */
        if ( (OuterRtEntry->InputMidSubscribed == HK_INPUTMID_NOT_SUBSCRIBED) &&
             (OuterCpyEntry->InputMid          != HK_UNDEFINED_ENTRY) )
        {
            Result = CFE_SB_Subscribe (OuterCpyEntry->InputMid, HK_AppData.CmdPipe);
            
            if (Result == CFE_SUCCESS)
            {
                /* Spin thru entire table (again) looking for duplicate InputMid's.  
                   This will let us mark each duplicate as already having been subscribed */
                for (Loop2=0; Loop2 < HK_COPY_TABLE_ENTRIES; Loop2++)
                {
                    InnerDefEntry = & StartOfCopyTable [Loop2];
                    InnerRtEntry  = & StartOfRtTable [Loop2];
            
                    /* If this entry's MID matches the one we're looking for */
                    if (OuterCpyEntry->InputMid == InnerDefEntry->InputMid)
                    {
                        InnerRtEntry->InputMidSubscribed = HK_INPUTMID_SUBSCRIBED;
                    }
                }
            }
            else
            {
                CFE_EVS_SendEvent (HK_CANT_SUBSCRIBE_TO_SB_PKT_EID, CFE_EVS_ERROR,
                                   "HK Processing New Table:SB_Subscribe for Mid 0x%04X returned 0x%04X",
                                   OuterCpyEntry->InputMid, (unsigned int)Result);
            }
        }
    }

    return ( CFE_SUCCESS );

}   /* end HK_ProcessNewCopyTable */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HK Tear down old copy table                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HK_TearDownOldCopyTable (hk_copy_table_entry_t * CpyTblPtr, 
                               hk_runtime_tbl_entry_t * RtTblPtr)
{
    hk_copy_table_entry_t         * StartOfCopyTable = NULL;
    hk_copy_table_entry_t         * OuterCpyEntry    = NULL;
    hk_copy_table_entry_t         * InnerDefEntry    = NULL;
    hk_runtime_tbl_entry_t        * StartOfRtTable   = NULL;
    hk_runtime_tbl_entry_t        * OuterRtEntry     = NULL;
    hk_runtime_tbl_entry_t        * InnerRtEntry     = NULL;
    int32                           Loop1            = 0;
    int32                           Loop2            = 0;
    CFE_SB_MsgId_t                  MidOfThisPacket  = 0xFFFF;
    void                          * OutputPktAddr    = NULL;
    void                          * SavedPktAddr     = NULL;
    int32                           Result           = CFE_SUCCESS;

    /* Ensure that the input arguments are valid */
    if ( ((void *)CpyTblPtr == NULL) || ((void *)RtTblPtr == NULL) )
    {
        CFE_EVS_SendEvent( HK_NULL_POINTER_TEARCPY_ERR_EID, CFE_EVS_ERROR,
                           "Null pointer detected in copy tbl tear down: CpyTbl = 0x%08X, RtTbl = 0x%08X",
                           (int)((void *)CpyTblPtr), (int)((void *)RtTblPtr) );
        return ( HK_NULL_POINTER_DETECTED );
    }
     
    StartOfCopyTable = CpyTblPtr;
    StartOfRtTable  = RtTblPtr;
   
    /* Loop thru the table looking for all of the SB packets that need to be freed */
    for (Loop1 = 0; Loop1 < HK_COPY_TABLE_ENTRIES; Loop1++)
    {
        OuterCpyEntry = & StartOfCopyTable [Loop1];
        OuterRtEntry  = & StartOfRtTable  [Loop1];

        /* If a Packet Address has been assigned, it needs to get deleted */
        if (OuterRtEntry->OutputPktAddr != NULL)
        {
            OutputPktAddr   = OuterRtEntry->OutputPktAddr;
            MidOfThisPacket = OuterCpyEntry->OutputMid;

            SavedPktAddr = OutputPktAddr;
            Result = CFE_ES_PutPoolBuf (HK_AppData.MemPoolHandle, (uint32 *) OutputPktAddr);
            if (Result >= CFE_SUCCESS)
            {               
                /* Spin thru the entire table looking for entries that used the same SB packets */
                for (Loop2=0; Loop2 < HK_COPY_TABLE_ENTRIES; Loop2++)
                {
                    InnerDefEntry = & StartOfCopyTable [Loop2];
                    InnerRtEntry  = & StartOfRtTable [Loop2];
                    
                    if ( (InnerDefEntry->OutputMid    == MidOfThisPacket) &&
                         (InnerRtEntry->OutputPktAddr == SavedPktAddr) )
                    {
                        /* NULL out the table entry whose packet was freed above */
                        InnerRtEntry->OutputPktAddr = (CFE_SB_MsgPtr_t) NULL;
                    }
                }
            }
            else
            {
                CFE_EVS_SendEvent (HK_MEM_POOL_FREE_FAILED_EID, CFE_EVS_ERROR,
                                   "HK TearDown: ES_putPoolBuf Err pkt:0x%08lX ret 0x%04X, hdl 0x%08lx",
                                   (unsigned long)SavedPktAddr, (unsigned int)Result, (unsigned long)HK_AppData.MemPoolHandle);

            }
        }

        /* If the InputMid for this Table Entry has been subscribed, it needs to
           get Unsubscribed as do any other identical InputMids throughout the table.
           We don't have to worry about leaving any Mid's subscribed since the entire table
           is getting clobbered. */
        if (OuterRtEntry->InputMidSubscribed == HK_INPUTMID_SUBSCRIBED)
        {
            CFE_SB_Unsubscribe (OuterCpyEntry->InputMid, HK_AppData.CmdPipe);
                        
            /* Spin thru the entire table looking for entries that used the same SB packets */
            for (Loop2=0; Loop2 < HK_COPY_TABLE_ENTRIES; Loop2++)
            {
                InnerDefEntry = & StartOfCopyTable [Loop2];
                InnerRtEntry  = & StartOfRtTable [Loop2];
                
                if (InnerDefEntry->InputMid == OuterCpyEntry->InputMid)
                {
                    InnerRtEntry->InputMidSubscribed = HK_INPUTMID_NOT_SUBSCRIBED;
                }
            }
        }

    }

    return ( CFE_SUCCESS );

}   /* end HK_TearDownOldCopyTable */




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HK Send combined output message                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HK_SendCombinedHkPacket (CFE_SB_MsgId_t WhichMidToSend)
{
    boolean                         PacketFound      = FALSE;
    hk_runtime_tbl_entry_t        * StartOfRtTable   = NULL;
    hk_runtime_tbl_entry_t        * RtTblEntry       = NULL;
    int32                           Loop             = 0;
    CFE_SB_MsgId_t                  ThisEntrysOutMid = 0xFFFF;
    CFE_SB_MsgId_t                  InputMidMissing  = 0xFFFF;

    StartOfRtTable  = (hk_runtime_tbl_entry_t *)  HK_AppData.RuntimeTablePtr;

    /* Look thru each item in this Table, but only send this packet once, at most */
    for (Loop = 0; ( (Loop < HK_COPY_TABLE_ENTRIES) && (PacketFound == FALSE) ); Loop++)
    {
        RtTblEntry  = & StartOfRtTable [Loop];

        /* Empty table entries are defined by NULL's in this field */
        if (RtTblEntry->OutputPktAddr != NULL)
        {
            ThisEntrysOutMid = CFE_SB_GetMsgId (RtTblEntry->OutputPktAddr);
            if (ThisEntrysOutMid == WhichMidToSend)
            {
                if(HK_CheckForMissingData(ThisEntrysOutMid,&InputMidMissing)==HK_MISSING_DATA_DETECTED)
                {
                    HK_AppData.MissingDataCtr++;
                    
                    CFE_EVS_SendEvent (HK_OUTPKT_MISSING_DATA_EID, CFE_EVS_DEBUG,
                       "Combined Packet 0x%04X missing data from Input Pkt 0x%04X", 
                       ThisEntrysOutMid,InputMidMissing);
                    
                }
#if HK_DISCARD_INCOMPLETE_COMBO == 1
                else /* This clause is only exclusive if discarding incomplete packets */
#endif
                { 
                    /* Send the combined housekeeping telemetry packet...        */
                    CFE_SB_TimeStampMsg ( (CFE_SB_Msg_t *) RtTblEntry->OutputPktAddr);
                    CFE_SB_SendMsg      ( (CFE_SB_Msg_t *) RtTblEntry->OutputPktAddr);
                
                    HK_AppData.CombinedPacketsSent ++ ;
                }

                HK_SetFlagsToNotPresent(ThisEntrysOutMid);               
                
                PacketFound = TRUE;
                
            }
        }
    }

    if (PacketFound == FALSE)
    {
        CFE_EVS_SendEvent (HK_UNKNOWN_COMBINED_PACKET_EID, CFE_EVS_INFORMATION,
                         "Combined HK Packet 0x%04X is not found in current HK Copy Table", 
                         WhichMidToSend);
    }

}   /* end HK_SendCombinedHkPacket */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Check the status of HK tables and perform any necessary action. */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HK_CheckStatusOfTables (void)
{
    int32   HKStatus = HK_ERROR;  /* Assume failure */

    HKStatus = HK_CheckStatusOfCopyTable( );    

    /* Only check the Dump Table if there were no problems with the Copy Table */
    if ( HKStatus == HK_SUCCESS )
    {
        HKStatus = HK_CheckStatusOfDumpTable( );
    }

    return ( HKStatus );
}   /* end HK_CheckStatusOfTables */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                     */
/* Check the status of HK Copy table and perform any necessary action. */
/*                                                                     */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HK_CheckStatusOfCopyTable (void)
{
    int32   Status   = CFE_SUCCESS;
    int32   HKStatus = HK_ERROR;  /* Assume failure */

    /* Determine if the copy table has a validation or update that needs to be performed */
    Status = CFE_TBL_GetStatus(HK_AppData.CopyTableHandle);

    if (Status == CFE_TBL_INFO_VALIDATION_PENDING)
    {
        /* Validate the specified Table */
        Status = CFE_TBL_Validate(HK_AppData.CopyTableHandle);

        if ( Status != CFE_SUCCESS )
        {
            CFE_EVS_SendEvent( HK_UNEXPECTED_TBLVLD_RET_EID, CFE_EVS_ERROR, "Unexpected CFE_TBL_Validate return (0x%08X) for Copy Table",
                              (unsigned int)Status );
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
        Status = HK_TearDownOldCopyTable(HK_AppData.CopyTablePtr, HK_AppData.RuntimeTablePtr);
            
        /* release address must be called for update to take. */
        Status = CFE_TBL_ReleaseAddress(HK_AppData.CopyTableHandle);

        /* Releasing the address should only return CFE_SUCCESS at this point since we
           already had an update-pending response earlier */
        if ( Status == CFE_SUCCESS )
        {
            /* Update the copy table */
            Status = CFE_TBL_Update(HK_AppData.CopyTableHandle);

            if ( Status == CFE_SUCCESS )
            {
                /* Get address of the newly updated copy table. */
                Status = CFE_TBL_GetAddress((void *)(&HK_AppData.CopyTablePtr), HK_AppData.CopyTableHandle);

                /* Status should only be CFE_TBL_INFO_UPDATED because we updated it above */
                if ( Status == CFE_TBL_INFO_UPDATED )
                {
                    Status = HK_ProcessNewCopyTable(HK_AppData.CopyTablePtr, HK_AppData.RuntimeTablePtr);

                    if ( Status == CFE_SUCCESS )
                    {
                        HKStatus = HK_SUCCESS;
                    }
                    else
                    {
                        /* If status was not success, then the copy table function received a NULL pointer argument */
                        CFE_EVS_SendEvent( HK_NEWCPYTBL_HK_FAILED_EID, CFE_EVS_CRITICAL, 
                                           "Process New Copy Table Failed, status = 0x%08X",
                                           (unsigned int)Status );
                    }
                }
                else
                {
                    CFE_EVS_SendEvent( HK_UNEXPECTED_GETADDR_RET_EID, CFE_EVS_CRITICAL, "Unexpected CFE_TBL_GetAddress return (0x%08X) for Copy Table",
                                      (unsigned int)Status );
                }
            }
            else
            {
                CFE_EVS_SendEvent( HK_UNEXPECTED_TBLUPD_RET_EID, CFE_EVS_CRITICAL, "Unexpected CFE_TBL_Update return (0x%08X) for Copy Table",
                                  (unsigned int)Status );
            }
        }
        else
        {
            CFE_EVS_SendEvent( HK_UNEXPECTED_RELADDR_RET_EID, CFE_EVS_CRITICAL, "Unexpected CFE_TBL_ReleaseAddress return (0x%08X) for Copy Table",
                              (unsigned int)Status );
        }
    }
    else if(Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent (HK_UNEXPECTED_GETSTAT_RET_EID, CFE_EVS_CRITICAL,
               "Unexpected CFE_TBL_GetStatus return (0x%08X) for Copy Table", 
               (unsigned int)Status);
    }
    else
    {
        /* Only way to get here is if Status == CFE_SUCCESS */
        HKStatus = HK_SUCCESS;
    }

    return ( HKStatus );
}   /* end HK_CheckStatusOfCopyTable */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                     */
/* Check the status of HK Dump table and perform any necessary action. */
/*                                                                     */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HK_CheckStatusOfDumpTable (void)
{
    int32   Status   = CFE_SUCCESS;
    int32   HKStatus = HK_ERROR;  /* Assume failure */

    HKStatus = HK_ERROR; /* Need to reset for the dump table case */

    /* Determine if the runtime table has a dump pending */   
    Status = CFE_TBL_GetStatus(HK_AppData.RuntimeTableHandle);
    
    if (Status == CFE_TBL_INFO_DUMP_PENDING)
    {
        /* Dump the specified Table, cfe tbl manager makes copy */
        Status  = CFE_TBL_DumpToBuffer(HK_AppData.RuntimeTableHandle);
    
        if ( Status != CFE_SUCCESS )
        {
            CFE_EVS_SendEvent( HK_UNEXPECTED_DUMPTOBUFFER_RET_EID, CFE_EVS_CRITICAL, "Unexpected CFE_TBL_DumpToBuffer return (0x%08X) for Runtime Table",
                              (unsigned int)Status );
        }
        else
        {
            HKStatus = HK_SUCCESS;
        }         
    }
    else if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent (HK_UNEXPECTED_GETSTAT2_RET_EID, CFE_EVS_CRITICAL,
               "Unexpected CFE_TBL_GetStatus return (0x%08X) for Runtime Table", 
               (unsigned int)Status);
    }
    else
    {
        /* Only way to get here is if Status == CFE_SUCCESS */
        HKStatus = HK_SUCCESS;
    }
        
    return ( HKStatus );
}   /* end HK_CheckStatusOfDumpTable */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HK Check for missing combined output message data               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HK_CheckForMissingData(CFE_SB_MsgId_t OutPktToCheck, CFE_SB_MsgId_t *MissingInputMid)
{
    int32                           Loop             = 0;
    int32                           Status           = HK_NO_MISSING_DATA;
    hk_copy_table_entry_t         * StartOfCopyTable = NULL;
    hk_copy_table_entry_t         * CpyTblEntry      = NULL;
    hk_runtime_tbl_entry_t        * StartOfRtTable   = NULL;
    hk_runtime_tbl_entry_t        * RtTblEntry       = NULL;

    StartOfCopyTable = (hk_copy_table_entry_t *) HK_AppData.CopyTablePtr;
    StartOfRtTable  = (hk_runtime_tbl_entry_t *)  HK_AppData.RuntimeTablePtr;

    /* Loop thru each item in the runtime table until end is reached or 
     * data-not-present detected */
    do
    {
        CpyTblEntry = &StartOfCopyTable[Loop];
        RtTblEntry  = &StartOfRtTable[Loop];

        /* Empty table entries are defined by NULL's in this field */
        if ((RtTblEntry->OutputPktAddr != NULL)&&
           (CpyTblEntry->OutputMid==OutPktToCheck)&&
           (RtTblEntry->DataPresent==HK_DATA_NOT_PRESENT))           
        {           
            *MissingInputMid = CpyTblEntry->InputMid;
            Status = HK_MISSING_DATA_DETECTED;          
        }
        
        Loop++;
        
    }while((Loop < HK_COPY_TABLE_ENTRIES)&&(Status == HK_NO_MISSING_DATA));
    
    return Status;
    
}/* end HK_CheckForMissingData */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HK Set data present flags to 'data-not-present'                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HK_SetFlagsToNotPresent(CFE_SB_MsgId_t OutPkt)
{
    int32                           Loop             = 0;
    hk_copy_table_entry_t         * StartOfCopyTable = NULL;
    hk_copy_table_entry_t         * CpyTblEntry      = NULL;
    hk_runtime_tbl_entry_t        * StartOfRtTable   = NULL;
    hk_runtime_tbl_entry_t        * RtTblEntry       = NULL;

    StartOfCopyTable = (hk_copy_table_entry_t *) HK_AppData.CopyTablePtr;
    StartOfRtTable  = (hk_runtime_tbl_entry_t *)  HK_AppData.RuntimeTablePtr;

    /* Look thru each item in the runtime table until end is reached */ 
    for(Loop = 0;Loop < HK_COPY_TABLE_ENTRIES; Loop++)
    {
        CpyTblEntry = &StartOfCopyTable[Loop];
        RtTblEntry  = &StartOfRtTable[Loop];

        /* Empty table entries are defined by NULL's in this field */
        if ((RtTblEntry->OutputPktAddr != NULL)&&
           (CpyTblEntry->OutputMid==OutPkt))           
        {           
            RtTblEntry->DataPresent = HK_DATA_NOT_PRESENT;          
        }
                
    }
        
}/* end HK_SetFlagsToNotPresent */


/************************/
/*  End of File Comment */
/************************/
