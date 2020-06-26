/************************************************************************
** File: hk_tbldefs.h 
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
#ifndef _hk_tbldefs_h_
#define _hk_tbldefs_h_


/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"


/*************************************************************************
** Type definitions
**************************************************************************/

/**  \brief HK Copy Table Entry Format 
*/

typedef struct
{
    CFE_SB_MsgId_t      InputMid;    /**< \brief MsgId of the input packet */
    uint16              InputOffset; /**< \brief ByteOffset into the input pkt where copy will begin */
    CFE_SB_MsgId_t      OutputMid;   /**< \brief MsgId of the output packet */
    uint16              OutputOffset;/**< \brief ByteOffset into the output pkt where data will be placed */
    uint16              NumBytes;    /**< \brief Number of data bytes to copy from input to output pkt */
} hk_copy_table_entry_t;


/**  \brief HK Run-time Table Entry Format 
*/
typedef struct
{
    CFE_SB_MsgPtr_t     OutputPktAddr;     /**< \brief Addr of output packet */
    uint8               InputMidSubscribed;/**< \brief Indicates if input MID has been subscribed to */
    uint8               DataPresent;       /**< \brief Indicates if the data associated with the entry is present */
} hk_runtime_tbl_entry_t;



#endif      /* _hk_tbldefs_h_ */

/************************/
/*  End of File Comment */
/************************/
