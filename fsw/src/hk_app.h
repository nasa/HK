/************************************************************************
** File: hk_app.h
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
#ifndef _hk_app_h_
#define _hk_app_h_


/************************************************************************
** Includes
*************************************************************************/

#include "cfe.h"
#include "hk_msgdefs.h"
#include "hk_msg.h"
#include "hk_utils.h"
#include "hk_platform_cfg.h"

/*************************************************************************
** Macro definitions
**************************************************************************/
#define HK_PIPE_NAME          "HK_CMD_PIPE" /**< \brief Application Pipe Name  */

#define HK_SUCCESS            (0)  /**< \brief HK return code for success */
#define HK_ERROR              (-1) /**< \brief HK return code for general error */
#define HK_BAD_MSG_LENGTH_RC  (-2) /**< \brief HK return code for unexpected cmd length */


/************************************************************************
** Type Definitions
*************************************************************************/
/** 
**  \brief HK global data structure
*/
typedef struct
{
    /*
    ** Housekeeping telemetry packet...
    */
    HK_HkPacket_t 			HkPacket;/**< \brief HK Housekeeping Packet */

    /*
    ** Operational data (not reported in housekeeping)...
    */
    CFE_SB_MsgPtr_t         MsgPtr;/**< \brief Pointer to msg received on software bus */

    CFE_SB_PipeId_t         CmdPipe;/**< \brief Pipe Id for HK command pipe */    
    uint8					CmdCounter;/**< \brief Number of valid commands received */
    uint8					ErrCounter;/**< \brief Number of invalid commands received */

    uint16					MissingDataCtr;/**< \brief Number of times missing data was detected */
    uint16					CombinedPacketsSent;/**< \brief Count of combined output msgs sent */    

    uint32                  MemPoolHandle;/**< \brief HK mempool handle for output pkts */
    uint32                  RunStatus;/**< \brief HK App run status */
        
    CFE_TBL_Handle_t        CopyTableHandle;/**< \brief Copy Table handle */
    CFE_TBL_Handle_t        RuntimeTableHandle;/**< \brief Run-time table handle */

    hk_copy_table_entry_t   *CopyTablePtr;/**< \brief Ptr to copy table entry */
    hk_runtime_tbl_entry_t  *RuntimeTablePtr;/**< \brief Ptr to run-time table entry */
        
    uint8                   MemPoolBuffer [HK_NUM_BYTES_IN_MEM_POOL];/**< \brief HK mempool buffer */

} HK_AppData_t;


/*************************************************************************
** Exported data
**************************************************************************/
extern HK_AppData_t             HK_AppData;/**< \brief HK Housekeeping Packet */

/************************************************************************
** Exported Functions
*************************************************************************/
/************************************************************************/
/** \brief CFS Housekeeping (HK) application entry point
**  
**  \par Description
**       Housekeeping application entry point and main process loop.
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
*************************************************************************/
void HK_AppMain(void);


/************************************************************************
** Prototypes for functions defined in hk_app.c
*************************************************************************/
/************************************************************************/
/** \brief Initialize the housekeeping application
**  
**  \par Description
**       Housekeeping application initialization routine. This 
**       function performs all the required startup steps to 
**       get the application registered with the cFE services so
**       it can begin to receive command messages. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register         \endcode
**  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
**  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
**  \endreturns
**
*************************************************************************/
int32 HK_AppInit (void);


/************************************************************************/
/** \brief Initialize the Copy Table and the Runtime Table
**  
**  \par Description
**       Registers the Copy table and Runtime table with cFE Table 
**       Services. Also processes the copy table. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \sa #HK_AppInit
**
*************************************************************************/
int32 HK_TableInit (void);


/************************************************************************/
/** \brief Process a command pipe message
**  
**  \par Description
**       Processes a single software bus command pipe message. Checks
**       the message and command IDs and calls the appropriate routine
**       to handle the command.
**       
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]  MessagePtr   A #CFE_SB_MsgPtr_t pointer that
**                            references the software bus message 
**
**  \sa #CFE_SB_RcvMsg
**
*************************************************************************/
void HK_AppPipe (CFE_SB_MsgPtr_t MessagePtr);


/************************************************************************/
/** \brief Send Combined Housekeeping Message
**  
**  \par Description
**       Processes the command to send a combined housekeeping message
**
**  \par Assumptions, External Events, and Notes:
**       This command does not affect the command execution counter, but 
**       this command will increment the cmd error counter if an invalid cmd
**       length is detected.
**
**  \param [in]  MessagePtr   A #CFE_SB_MsgPtr_t pointer that
**                            references the software bus message 
**
*************************************************************************/
void HK_SendCombinedHKCmd(CFE_SB_MsgPtr_t MessagePtr);


/************************************************************************/
/** \brief Process housekeeping request
**  
**  \par Description
**       Processes an on-board housekeeping request message.
**
**  \par Assumptions, External Events, and Notes:
**       This command does not affect the command execution counter, but 
**       this command will increment the cmd error counter if an invalid cmd
**       length is detected.
**
**  \param [in]  MessagePtr   A #CFE_SB_MsgPtr_t pointer that
**                            references the software bus message 
**
*************************************************************************/
void HK_HousekeepingCmd (CFE_SB_MsgPtr_t MessagePtr);


/************************************************************************/
/** \brief Process noop command
**  
**  \par Description
**       Processes a noop ground command.
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]  MessagePtr   A #CFE_SB_MsgPtr_t pointer that
**                            references the software bus message 
**
**  \sa #HK_NOOP_CC
**
*************************************************************************/
void HK_NoopCmd (CFE_SB_MsgPtr_t MessagePtr);


/************************************************************************/
/** \brief Process reset counters command
**  
**  \par Description
**       Processes a reset counters ground command which will reset
**       the memory manager commmand error and command execution counters
**       to zero.
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   MessagePtr   A #CFE_SB_MsgPtr_t pointer that
**                             references the software bus message 
**
**  \sa #HK_RESET_CC
**
*************************************************************************/
void HK_ResetCtrsCmd (CFE_SB_MsgPtr_t MessagePtr);


/************************************************************************/
/** \brief Reset housekeeping data
**  
**  \par Description
**       Function called in response to a Reset Counters Command. This
**       function will reset the HK housekeeping data.
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \sa #HK_RESET_CC
**
*************************************************************************/
void HK_ResetHkData (void);


/************************************************************************/
/** \brief  Verify length of HK commands
**  
**  \par Description
**       Function called when an HK command is received. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   MessagePtr   A #CFE_SB_MsgPtr_t pointer that
**                             references the software bus message 
**  \param [in]   ExpectedLength   The expected lenght of the command
**
**  \returns
**  \retcode #HK_SUCCESS  if actual cmd length is same as expected
**  \retcode #HK_BAD_MSG_LENGTH_RC if actual cmd length is not as expected
**  \endreturns
**
**  \sa 
**
*************************************************************************/
int32 HK_VerifyCmdLength (CFE_SB_MsgPtr_t MessagePtr,uint32 ExpectedLength);



#endif /* _hk_app_ */

/************************/
/*  End of File Comment */
/************************/
