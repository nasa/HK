/*************************************************************************
** File: hk_test_utils.c 
**
** NASA Docket No. GSC-18449-1, and identified as "Core Flight System (cFS)
** Housekeeping (HK) Application version 2.4.3” 
**
** Copyright © 2019 United States Government as represented by the Administrator of
** the National Aeronautics and Space Administration.  All Rights Reserved. 
**
** Licensed under the Apache License, Version 2.0 (the "License"); 
** you may not use this file except in compliance with the License. 
** You may obtain a copy of the License at 
** http://www.apache.org/licenses/LICENSE-2.0 
** Unless required by applicable law or agreed to in writing, software 
** distributed under the License is distributed on an "AS IS" BASIS, 
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
** See the License for the specific language governing permissions and 
** limitations under the License. 
**
** Purpose: 
**   This file contains unit test utilities for the HK application.
**
** References:
**   Flight Software Branch C Coding Standard Version 1.2
**   CFS Development Standards Document
**
** Notes:
**
*************************************************************************/

/*
 * Includes
 */

#include "hk_test_utils.h"
#include "hk_msg.h"

extern HK_AppData_t     HK_AppData;

/*
 * Function Definitions
 */

void HK_Test_Setup(void)
{
    /* initialize test environment to default state for every test */

    CFE_PSP_MemSet(&HK_AppData, 0, sizeof(HK_AppData_t));
    
    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();
} /* end HK_Test_Setup */

void HK_Test_TearDown(void)
{
    /* cleanup test environment */
} /* end HK_Test_TearDown */

/************************/
/*  End of File Comment */
/************************/
