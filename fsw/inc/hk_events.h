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
 *  The CFS Housekeeping (HK) Application event id header file
 */
#ifndef HK_EVENTS_H
#define HK_EVENTS_H

/**
 * \defgroup cfshkevents CFS Housekeeping Event IDs
 * \{
 */

/**
 * \brief HK Initialization Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause:
 *
 *  This event message is issued when the Housekeeping App completes its
 *  initialization.
 */
#define HK_INIT_INF_EID 1

/**
 * \brief HK Command Code Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the HK application receives an unexpected
 *  command code
 */
#define HK_CC_ERR_EID 2

/**
 * \brief HK Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the actual command length does not equal
 *  the expected command length
 */
#define HK_CMD_LEN_ERR_EID 3

/**
 * \brief HK No-op Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause:
 *
 *  This event message is issued when the HK application successfully receives a
 *  \link #HK_NOOP_CC No-op command. \endlink  The command is used primarily as an
 *  indicator that the HK application can receive commands and generate telemetry.
 */
#define HK_NOOP_INF_EID 4

/**
 * \brief HK Reset Counters Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause:
 *
 *  This event message is issued when the HK application receives a Reset
 *  Counters command.
 */
#define HK_RESET_INF_EID 5

/**
 * \brief HK Table Definition Exceeds Packet Length Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the received input packet is not large
 *  enough to accommodate every entry in the copy table. The count indicates the
 *  total number of copy table entries that reference past the end of the input packet.
 */
#define HK_ACCESSING_PAST_PACKET_END_EID 6

/**
 * \brief HK New Table ES Memory Pool Request Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the HK application receives an error when
 *  requesting memory from the ES memory pool
 */
#define HK_MEM_POOL_MALLOC_FAILED_EID 7

/**
 * \brief HK Input Message Subscribe Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the HK application receives an error while
 *  subscribing to the input messages.
 */
#define HK_CANT_SUBSCRIBE_TO_SB_PKT_EID 8

/**
 * \brief HK Table Release Memory Pool Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the HK application receives an error while
 *  attempting to free the memory for an output message
 */
#define HK_MEM_POOL_FREE_FAILED_EID 9

/**
 * \brief HK Table Get Status Failed Event ID
 *
 *  \par Type: CRITICAL
 *
 *  \par Cause:
 *
 *  This event message is issued when the HK application receives an unexpected
 *  return value when calling the #CFE_TBL_GetStatus API for the copy table
 */
#define HK_UNEXPECTED_GETSTAT_RET_EID 10

/**
 * \brief HK Combined Packet Message Not In Copy Table Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the HK application receives a command to
 *  send a combined output message that is not listed in the copy table.
 */
#define HK_UNKNOWN_COMBINED_PACKET_EID 11

/**
 * \brief HK Combined Packet Missing Data From Input Packet Event ID
 *
 *  \par Type: DEBUG
 *
 *  \par Cause:
 *
 *  This event message is issued when at least one section of data is missing in
 *  an output message.
 */
#define HK_OUTPKT_MISSING_DATA_EID 12

/**
 * \brief HK Create Software Bus Pipe Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the call to #CFE_SB_CreatePipe
 *  during HK initialization returns a value other than #CFE_SUCCESS
 */
#define HK_CR_PIPE_ERR_EID 14

/**
 * \brief HK Subscribe To Send Combined Packet Request Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the call to #CFE_SB_Subscribe
 *  for the #HK_SEND_COMBINED_PKT_MID, during HK initialization
 *  returns a value other than #CFE_SUCCESS
 */
#define HK_SUB_CMB_ERR_EID 15

/**
 * \brief HK Subscribe to Housekeeping Request Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the call to #CFE_SB_Subscribe
 *  for the #HK_SEND_HK_MID, during HK initialization returns
 *  a value other than #CFE_SUCCESS
 */
#define HK_SUB_REQ_ERR_EID 16

/**
 * \brief HK Subscribe to Ground Commands Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the call to #CFE_SB_Subscribe
 *  for the #HK_CMD_MID, during HK initialization returns a value
 *  other than #CFE_SUCCESS
 */
#define HK_SUB_CMD_ERR_EID 17

/**
 * \brief HK Memory Pool Create Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the call to #CFE_ES_PoolCreate
 *  during HK initialization returns a value other than #CFE_SUCCESS
 */
#define HK_CR_POOL_ERR_EID 18

/**
 * \brief HK Copy Table Registration Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the call to #CFE_TBL_Register
 *  for the copy table returns a value other than #CFE_SUCCESS
 */
#define HK_CPTBL_REG_ERR_EID 19

/**
 * \brief HK Runtime Table Registration Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the call to #CFE_TBL_Register
 *  for the runtime table returns a value other than #CFE_SUCCESS
 */
#define HK_RTTBL_REG_ERR_EID 20

/**
 * \brief HK Copy Table Load Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the call to #CFE_TBL_Load
 *  for the copy table returns a value other than #CFE_SUCCESS
 */
#define HK_CPTBL_LD_ERR_EID 21

/**
 * \brief HK Copy Table Manage Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the call to #CFE_TBL_Manage
 *  for the copy table returns a value other than #CFE_SUCCESS
 */
#define HK_CPTBL_MNG_ERR_EID 22

/**
 * \brief HK Runtime Table Manage Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the call to #CFE_TBL_Manage
 *  for the runtime table returns a value other than #CFE_SUCCESS
 */
#define HK_RTTBL_MNG_ERR_EID 23

/**
 * \brief HK Copy Table Get Address Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the call to #CFE_TBL_GetAddress
 *  for the copy table returns a value other than #CFE_TBL_INFO_UPDATED
 */
#define HK_CPTBL_GADR_ERR_EID 24

/**
 * \brief HK Runtime Table Get Address Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the call to #CFE_TBL_GetAddress
 *  for the runtime table returns a value other than #CFE_SUCCESS
 */
#define HK_RTTBL_GADR_ERR_EID 25

/**
 * \brief HK Exit Due To Software Bus Receive Error Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the call to #CFE_SB_ReceiveBuffer
 *  returns a value other than #CFE_SUCCESS in the main loop
 */
#define HK_RCV_MSG_ERR_EID 26

/**
 * \brief HK Runtime Table Get Status Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the HK application receives an unexpected
 *  return value when calling the #CFE_TBL_GetStatus API for the dump table
 */
#define HK_UNEXPECTED_GETSTAT2_RET_EID 27

/**
 * \brief HK Message Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event is issued when an internal message which has a length that is inconsistant
 *  with the expected length for its message id.
 */
#define HK_MSG_LEN_ERR_EID 28

/**
 * \brief HK Copy Table Get Address Unexpected Value Event ID
 *
 *  \par Type: CRITICAL
 *
 *  \par Cause:
 *
 *  This event message is issued when the HK application receives an unexpected
 *  return value when calling the #CFE_TBL_GetAddress API for the copy table during
 *  the check of the table status. This is a critical error (due to unexpected behavior
 *  of a cFE service) and will cause the HK app to exit.
 */
#define HK_UNEXPECTED_GETADDR_RET_EID 29

/**
 * \brief HK Copy Table Update Failed Event ID
 *
 *  \par Type: CRITICAL
 *
 *  \par Cause:
 *
 *  This event message is issued when the HK application receives an unexpected
 *  return value when calling the #CFE_TBL_Update API for the copy table during
 *  the check of the table status. This is a critical error (due to unexpected behavior
 *  of a cFE service) and will cause the HK app to exit.
 */
#define HK_UNEXPECTED_TBLUPD_RET_EID 30

/**
 * \brief HK Copy Table Release Address Failed Event ID
 *
 *  \par Type: CRITICAL
 *
 *  \par Cause:
 *
 *  This event message is issued when the HK application receives an unexpected
 *  return value when calling the #CFE_TBL_ReleaseAddress API for the copy table during
 *  the check of the table status. This is a critical error (due to unexpected behavior
 *  of a cFE service) and will cause the HK app to exit.
 */
#define HK_UNEXPECTED_RELADDR_RET_EID 31

/**
 * \brief HK Copy Table Validate Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message is issued when the HK application receives an unexpected
 *  return value when calling the #CFE_TBL_Validate API for the copy table during
 *  the check of the table status.
 */
#define HK_UNEXPECTED_TBLVLD_RET_EID 32

/**
 * \brief HK Runtime Table Dump To Buffer Failed Event ID
 *
 *  \par Type: CRITICAL
 *
 *  \par Cause:
 *
 *  This event message is issued when the HK application receives an unexpected
 *  return value when calling the #CFE_TBL_DumpToBuffer API for the runtime table during
 *  the check of the table status.  This is a critical error (due to unexpected behavior
 *  of a cFE service) and will cause the HK app to exit.
 */
#define HK_UNEXPECTED_DUMPTOBUFFER_RET_EID 33

/**
 * \brief HK New Copy Table Null Pointer Detected Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event is issued when at least one of the input arguments for processing a new copy table is NULL.
 */
#define HK_NULL_POINTER_NEWCPY_ERR_EID 34

/**
 * \brief HK Copy Table Teardown Null Pointer Detected Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event is issued when at least one of the input arguments for tearing down an old copy table is NULL.
 */
#define HK_NULL_POINTER_TEARCPY_ERR_EID 35

/**
 * \brief HK New Copy Table Housekeeping Processing Failed Event ID
 *
 *  \par Type: CRITICAL
 *
 *  \par Cause:
 *
 *  This event is issued when an error occurs while processing a new copy table during housekeeping processing.
 *  The probable reason for a failure is that a NULL pointer was used as an argument in the called routine (a status
 *  value of 0xFFFFFFFF indicates a NULL pointer was detected).
 *  In this case (as opposed to during initialization), the event is critical.
 */
#define HK_NEWCPYTBL_HK_FAILED_EID 36

/**
 * \brief HK New Copy Table Initialization Processing Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event is issued when an error occurs while processing a new copy table during app initialization.
 *  The probable reason for a failure is that a NULL pointer was used as an argument in the called routine (a status
 *  value of 0xFFFFFFFF indicates a NULL pointer was detected).
 *  In this case (as opposed to during housekeeping processing), the event is an error, but not critical.
 */
#define HK_NEWCPYTBL_INIT_FAILED_EID 37

/**\}*/

#endif
