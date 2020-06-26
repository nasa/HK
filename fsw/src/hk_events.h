/************************************************************************
** File: hk_events.h 
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
**  The CFS Housekeeping (HK) Application event id header file
**
** Notes:
**
**
*************************************************************************/
#ifndef _hk_events_h_
#define _hk_events_h_


/*************************************************************************
** Macro definitions
**************************************************************************/

/** \brief <tt> 'HK Initialized.  Version \%d.\%d.\%d.\%d' </tt>
**  \event <tt> 'HK Initialized.  Version \%d.\%d.\%d.\%d' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the Housekeeping App completes its
**  initialization.
**
**  The \c Version fields contain the #HK_MAJOR_VERSION,
**  #HK_MINOR_VERSION, #HK_REVISION, and #HK_MISSION_REV
**  version identifiers. 
**/
#define HK_INIT_EID                        	1


/** \brief <tt> 'Cmd Msg with Invalid command code Rcvd -- ID = 0x\%04X, CC = \%d' </tt>
**  \event <tt> 'Cmd Msg with Invalid command code Rcvd -- ID = 0x\%04X, CC = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the HK application receives an unexpected
**  command code 
**/
#define HK_CC_ERR_EID                      	2


/** \brief <tt> 'Cmd Msg with Bad length Rcvd: ID = 0x\%X, CC = \%d, Exp Len = \%d, Len = \%d' </tt>
**  \event <tt> 'Cmd Msg with Bad length Rcvd: ID = 0x\%X, CC = \%d, Exp Len = \%d, Len = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the actual command length does not equal 
**  the expected command length
**/
#define HK_CMD_LEN_ERR_EID                 	3


/** \brief <tt> 'HK No-op command, Version \%d.\%d.\%d.\%d' </tt>
**  \event <tt> 'HK No-op command, Version \%d.\%d.\%d.\%d' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the HK application successfully receives a 
**  \link #HK_NOOP_CC No-op command. \endlink  The command is used primarily as an 
**  indicator that the HK application can receive commands and generate telemetry.
**
**  The \c Version fields contain the #HK_MAJOR_VERSION,
**  #HK_MINOR_VERSION, #HK_REVISION, and #HK_MISSION_REV
**  version identifiers. 
**/
#define HK_NOOP_CMD_EID                   	4


/** \brief <tt> 'HK Reset Counters command received' </tt>
**  \event <tt> 'HK Reset Counters command received' </tt>
**
**  \par Type: DEUBUG
**
**  \par Cause:
**
**  This event message is issued when the HK application receives a Reset 
**  Counters command.
**/
#define HK_RESET_CNTRS_CMD_EID                   	5

/** \brief <tt> 'HK table definition exceeds packet length. MID:0x\%04X, Length:\%d, Count:\%d' </tt>
**  \event <tt> 'HK table definition exceeds packet length. MID:0x\%04X, Length:\%d, Count:\%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the received input packet is not large 
**  enough to accommodate every entry in the copy table. The count indicates the
**  total number of copy table entries that reference past the end of the input packet. 
**/
#define HK_ACCESSING_PAST_PACKET_END_EID   	6


/** \brief <tt> 'HK Processing New Table: ES_GetPoolBuf for size \%d returned 0x\%04X' </tt>
**  \event <tt> 'HK Processing New Table: ES_GetPoolBuf for size \%d returned 0x\%04X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the HK application receives an error when
**  requesting memory from the ES memory pool
**/
#define HK_MEM_POOL_MALLOC_FAILED_EID      	7


/** \brief <tt> 'HK Processing New Table:SB_Subscribe for Mid 0x\%04X returned 0x\%04X' </tt>
**  \event <tt> 'HK Processing New Table:SB_Subscribe for Mid 0x\%04X returned 0x\%04X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the HK application receives an error while
**  subscribing to the input messages. 
**/
#define HK_CANT_SUBSCRIBE_TO_SB_PKT_EID    	8


/** \brief <tt> 'HK TearDown Old Table: ES_PutPoolBuf for pkt:0x\%08X returned 0x\%04X' </tt>
**  \event <tt> 'HK TearDown Old Table: ES_PutPoolBuf for pkt:0x\%08X returned 0x\%04X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the HK application receives an error while
**  attempting to free the memory for an output message
**/
#define HK_MEM_POOL_FREE_FAILED_EID        	9


/** \brief <tt> 'Unexpected CFE_TBL_GetStatus return (0x\%08X) for Copy Table' </tt>
**  \event <tt> 'Unexpected CFE_TBL_GetStatus return (0x\%08X) for Copy Table' </tt>
**
**  \par Type: CRITICAL
**
**  \par Cause:
**
**  This event message is issued when the HK application receives an unexpected 
**  return value when calling the CFE_TBL_GetStatus API for the copy table
**/
#define HK_UNEXPECTED_GETSTAT_RET_EID       10


/** \brief <tt> 'Combined HK Packet 0x\%04X is not found in current HK Copy Table' </tt>
**  \event <tt> 'Combined HK Packet 0x\%04X is not found in current HK Copy Table' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the HK application receives a command to
**  send a combined output message that is not listed in the copy table. 
**/
#define HK_UNKNOWN_COMBINED_PACKET_EID     	11


/** \brief <tt> 'Combined Packet 0x\%04X missing data from Input Pkt 0x\%04X' </tt>
**  \event <tt> 'Combined Packet 0x\%04X missing data from Input Pkt 0x\%04X' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event message is issued when at least one section of data is missing in
**  an output message. 
**/
#define HK_OUTPKT_MISSING_DATA_EID          12


/** \brief <tt> 'Error Creating SB Pipe,RC=0x\%08X' </tt>
**  \event <tt> 'Error Creating SB Pipe,RC=0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the call to CFE_SB_CreatePipe
**  during HK initialization returns a value other than CFE_SUCCESS  
**/
#define HK_CR_PIPE_ERR_EID					14


/** \brief <tt> 'Error Subscribing to 0x\%04X,RC=0x\%08X' </tt>
**  \event <tt> 'Error Subscribing to 0x\%04X,RC=0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the call to CFE_SB_Subscribe
**  for the #HK_SEND_COMBINED_PKT_MID, during HK initialization 
**  returns a value other than CFE_SUCCESS  
**/
#define HK_SUB_CMB_ERR_EID					15


/** \brief <tt> 'Error Subscribing to HK Request,RC=0x\%08X' </tt>
**  \event <tt> 'Error Subscribing to HK Request,RC=0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the call to CFE_SB_Subscribe
**  for the #HK_SEND_HK_MID, during HK initialization returns 
**  a value other than CFE_SUCCESS 
**/
#define HK_SUB_REQ_ERR_EID					16


/** \brief <tt> 'Error Subscribing to HK Gnd Cmds,RC=0x\%08X' </tt>
**  \event <tt> 'Error Subscribing to HK Gnd Cmds,RC=0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the call to CFE_SB_Subscribe
**  for the #HK_CMD_MID, during HK initialization returns a value
**  other than CFE_SUCCESS  
**/
#define HK_SUB_CMD_ERR_EID					17


/** \brief <tt> 'Error Creating Memory Pool,RC=0x\%08X' </tt>
**  \event <tt> 'Error Creating Memory Pool,RC=0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the call to CFE_ES_PoolCreate
**  during HK initialization returns a value other than CFE_SUCCESS 
**/
#define HK_CR_POOL_ERR_EID					18


/** \brief <tt> 'Error Registering Copy Table,RC=0x\%08X' </tt>
**  \event <tt> 'Error Registering Copy Table,RC=0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the call to CFE_TBL_Register
**  for the copy table returns a value other than CFE_SUCCESS  
**/
#define HK_CPTBL_REG_ERR_EID				19


/** \brief <tt> 'Error Registering Runtime Table,RC=0x\%08X' </tt>
**  \event <tt> 'Error Registering Runtime Table,RC=0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the call to CFE_TBL_Register
**  for the runtime table returns a value other than CFE_SUCCESS  
**/
#define HK_RTTBL_REG_ERR_EID				20


/** \brief <tt> 'Error Loading Copy Table,RC=0x\%08X' </tt>
**  \event <tt> 'Error Loading Copy Table,RC=0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the call to CFE_TBL_Load
**  for the copy table returns a value other than CFE_SUCCESS  
**/
#define HK_CPTBL_LD_ERR_EID					21


/** \brief <tt> 'Error from TBL Manage call for Copy Table,RC=0x\%08X' </tt>
**  \event <tt> 'Error from TBL Manage call for Copy Table,RC=0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the call to CFE_TBL_Manage
**  for the copy table returns a value other than CFE_SUCCESS  
**/
#define HK_CPTBL_MNG_ERR_EID				22


/** \brief <tt> 'Error from TBL Manage call for Runtime Table,RC=0x\%08X' </tt>
**  \event <tt> 'Error from TBL Manage call for Runtime Table,RC=0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the call to CFE_TBL_Manage
**  for the runtime table returns a value other than CFE_SUCCESS 
**/
#define HK_RTTBL_MNG_ERR_EID				23


/** \brief <tt> 'Error Getting Adr for Cpy Tbl,RC=0x\%08X' </tt>
**  \event <tt> 'Error Getting Adr for Cpy Tbl,RC=0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the call to CFE_TBL_GetAddress
**  for the copy table returns a value other than CFE_TBL_INFO_UPDATED 
**/
#define HK_CPTBL_GADR_ERR_EID				24


/** \brief <tt> 'Error Getting Adr for Runtime Table,RC=0x\%08X' </tt>
**  \event <tt> 'Error Getting Adr for Runtime Table,RC=0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the call to CFE_TBL_GetAddress
**  for the runtime table returns a value other than CFE_SUCCESS 
**/
#define HK_RTTBL_GADR_ERR_EID				25


/** \brief <tt> 'HK_APP Exiting due to CFE_SB_RcvMsg error 0x\%08X' </tt>
**  \event <tt> 'HK_APP Exiting due to CFE_SB_RcvMsg error 0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the call to CFE_SB_RcvMsg
**  returns a value other than CFE_SUCCESS in the main loop
**/
#define HK_RCV_MSG_ERR_EID					26


/** \brief <tt> 'Unexpected CFE_TBL_GetStatus return (0x\%08X) for Runtime Table' </tt>
**  \event <tt> 'Unexpected CFE_TBL_GetStatus return (0x\%08X) for Runtime Table' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the HK application receives an unexpected 
**  return value when calling the CFE_TBL_GetStatus API for the dump table
**/
#define HK_UNEXPECTED_GETSTAT2_RET_EID       27


/** \brief <tt> 'Msg with Bad length Rcvd: ID = 0x\%04X, Exp Len = \%d, Len = \%d' </tt>
**  \event <tt> 'Msg with Bad length Rcvd: ID = 0x\%04X, Exp Len = \%d, Len = \%d' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event is issued when an internal message which has a length that is inconsistant
**  with the expected length for its message id.  
**/
#define HK_MSG_LEN_ERR_EID 28


/** \brief <tt> 'Unexpected CFE_TBL_GetAddress return (0x%08X) for Copy Table' </tt>
**  \event <tt> 'Unexpected CFE_TBL_GetAddress return (0x%08X) for Copy Table' </tt> 
**
**  \par Type: CRITICAL
**
**  \par Cause:
**
**  This event message is issued when the HK application receives an unexpected 
**  return value when calling the CFE_TBL_GetAddress API for the copy table during
**  the check of the table status. This is a critical error (due to unexpected behavior
**  of a cFE service) and will cause the HK app to exit.
**/
#define HK_UNEXPECTED_GETADDR_RET_EID 29


/** \brief <tt> 'Unexpected CFE_TBL_Update return (0x%08X) for Copy Table' </tt>
**  \event <tt> 'Unexpected CFE_TBL_Update return (0x%08X) for Copy Table' </tt> 
**
**  \par Type: CRITICAL
**
**  \par Cause:
**
**  This event message is issued when the HK application receives an unexpected 
**  return value when calling the CFE_TBL_Update API for the copy table during
**  the check of the table status. This is a critical error (due to unexpected behavior
**  of a cFE service) and will cause the HK app to exit.
**/
#define HK_UNEXPECTED_TBLUPD_RET_EID 30


/** \brief <tt> 'Unexpected CFE_TBL_ReleaseAddress return (0x%08X) for Copy Table' </tt>
**  \event <tt> 'Unexpected CFE_TBL_ReleaseAddress return (0x%08X) for Copy Table' </tt> 
**
**  \par Type: CRITICAL
**
**  \par Cause:
**
**  This event message is issued when the HK application receives an unexpected 
**  return value when calling the CFE_TBL_ReleaseAddress API for the copy table during
**  the check of the table status. This is a critical error (due to unexpected behavior
**  of a cFE service) and will cause the HK app to exit.
**/
#define HK_UNEXPECTED_RELADDR_RET_EID 31


/** \brief <tt> 'Unexpected CFE_TBL_Validate return (0x%08X) for Copy Table' </tt>
**  \event <tt> 'Unexpected CFE_TBL_Validate return (0x%08X) for Copy Table' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the HK application receives an unexpected 
**  return value when calling the CFE_TBL_Validate API for the copy table during
**  the check of the table status.
**/
#define HK_UNEXPECTED_TBLVLD_RET_EID 32


/** \brief <tt> 'Unexpected CFE_TBL_DumpToBuffer return (0x%08X) for Runtime Table' </tt>
**  \event <tt> 'Unexpected CFE_TBL_DumpToBuffer return (0x%08X) for Runtime Table' </tt> 
**
**  \par Type: CRITICAL
**
**  \par Cause:
**
**  This event message is issued when the HK application receives an unexpected 
**  return value when calling the CFE_TBL_DumpToBuffer API for the runtime table during
**  the check of the table status.  This is a critical error (due to unexpected behavior
**  of a cFE service) and will cause the HK app to exit.
**/
#define HK_UNEXPECTED_DUMPTOBUFFER_RET_EID 32


/** \brief <tt> 'Null pointer detected in new copy tbl processing: CpyTbl = 0x\%08X, RtTbl = 0x\%08X' </tt>
**  \event <tt> 'Null pointer detected in new copy tbl processing: CpyTbl = 0x\%08X, RtTbl = 0x\%08X' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event is issued when at least one of the input arguments for processing a new copy table is NULL.  
**/
#define HK_NULL_POINTER_NEWCPY_ERR_EID 33


/** \brief <tt> 'Null pointer detected in copy tbl tear down: CpyTbl = 0x\%08X, RtTbl = 0x\%08X' </tt>
**  \event <tt> 'Null pointer detected in copy tbl tear down: CpyTbl = 0x\%08X, RtTbl = 0x\%08X' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event is issued when at least one of the input arguments for tearing down an old copy table is NULL.  
**/
#define HK_NULL_POINTER_TEARCPY_ERR_EID 34


/** \brief <tt> 'Process New Copy Table Failed, status = 0x\%08X' </tt>
**  \event <tt> 'Process New Copy Table Failed, status = 0x\%08X' </tt> 
**
**  \par Type: CRITICAL
**
**  \par Cause:
**
**  This event is issued when an error occurs while processing a new copy table during housekeeping processing.
**  The probable reason for a failure is that a NULL pointer was used as an argument in the called routine (a status
**  value of 0xFFFFFFFF indicates a NULL pointer was detected).
**  In this case (as opposed to during initialization), the event is critical.   
**/
#define HK_NEWCPYTBL_HK_FAILED_EID 35


/** \brief <tt> 'Process New Copy Table Failed, status = 0x\%08X' </tt>
**  \event <tt> 'Process New Copy Table Failed, status = 0x\%08X' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event is issued when an error occurs while processing a new copy table during app initialization.
**  The probable reason for a failure is that a NULL pointer was used as an argument in the called routine (a status
**  value of 0xFFFFFFFF indicates a NULL pointer was detected).
**  In this case (as opposed to during housekeeping processing), the event is an error, but not critical.   
**/
#define HK_NEWCPYTBL_INIT_FAILED_EID 36


#endif /* _hk_events_h_ */

/************************/
/*  End of File Comment */
/************************/
