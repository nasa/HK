/************************************************************************
** File: hk_msg.h 
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
**  The CFS Housekeeping (HK) Application header file
**
** Notes:
**
**
*************************************************************************/
#ifndef _hk_msg_h_
#define _hk_msg_h_


/*************************************************************************
** Includes
**************************************************************************/
#include "cfe.h"


/****************************
**  HK Command Formats     **
*****************************/

/**
**  \brief Send Combined Output Message Command
**
**  This structure contains the format of the command used to inform HK to send
**  the specified combined output message 
*/
typedef struct
{
	CFE_SB_CmdHdr_t   Hdr;/**< \brief cFE Software Bus Command Message Header #CFE_SB_CmdHdr_t */
	CFE_SB_MsgId_t    OutMsgToSend;/**< \brief MsgId #CFE_SB_MsgId_t of combined tlm pkt to send  */
	
} HK_Send_Out_Msg_t;



/****************************
**  HK Telemetry Formats   **
*****************************/

/**
**  \hktlm HK Application housekeeping Packet
*/
typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];/**< \brief cFE Software Bus Telemetry Message Header */
 
    uint8   CmdCounter;         /**< \hktlmmnemonic \HK_CMDPC
                                \brief Count of valid commands received */
    uint8   ErrCounter;         /**< \hktlmmnemonic \HK_CMDEC
                                \brief Count of invalid commands received */
    uint16  Padding;			/**< \hktlmmnemonic \HK_PADDING
                                \brief Padding to force 32 bit alignment */
    uint16  CombinedPacketsSent;/**< \hktlmmnemonic \HK_CMBPKTSSENT
                                \brief Count of combined tlm pkts sent */
    uint16  MissingDataCtr;     /**< \hktlmmnemonic \HK_MISSDATACTR
                                \brief Number of times missing data was detected */ 
    uint32  MemPoolHandle;      /**< \hktlmmnemonic \HK_MEMPOOLHNDL
                                \brief Memory pool handle used to get mempool diags */

} HK_HkPacket_t;

      
#endif /* _hk_msg_h_ */

/************************/
/*  End of File Comment */
/************************/
