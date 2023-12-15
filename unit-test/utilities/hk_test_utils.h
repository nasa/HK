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

#ifndef HK_TEST_UTILS_H
#define HK_TEST_UTILS_H

#include "hk_app.h"
#include "hk_msg.h"
#include "hk_events.h"
#include "hk_version.h"
#include "hk_msgids.h"
#include "hk_tbldefs.h"

#include "cfe_msgids.h"

#include "utstubs.h"

extern HK_AppData_t HK_AppData;

/*
 * Global context structures
 */
typedef struct
{
    uint16 EventID;
    uint16 EventType;
    char   Spec[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
} CFE_EVS_SendEvent_context_t;

typedef struct
{
    char Spec[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
} CFE_ES_WriteToSysLog_context_t;

extern CFE_EVS_SendEvent_context_t    context_CFE_EVS_SendEvent[];
extern CFE_ES_WriteToSysLog_context_t context_CFE_ES_WriteToSysLog;

/* Unit test ids */
#define HK_UT_MID_100   CFE_SB_ValueToMsgId(100)
#define HK_UT_MEMPOOL_1 CFE_ES_MEMHANDLE_C(CFE_RESOURCEID_WRAP(1))

void HK_Test_InitGoodCopyTable(hk_copy_table_entry_t *CpyTbl);

void HK_Test_InitOverflowCopyTable(hk_copy_table_entry_t *CpyTbl);

void HK_Test_InitEmptyCopyTable(hk_copy_table_entry_t *CpyTbl);

void HK_Test_InitGoodRuntimeTable(hk_runtime_tbl_entry_t *RtTbl);

void HK_Test_InitEmptyRuntimeTable(hk_runtime_tbl_entry_t *RtTbl);

/*
 * Setup function prior to every test
 */
void HK_Test_Setup(void);

/*
 * Teardown function after every test
 */
void HK_Test_TearDown(void);

#endif
