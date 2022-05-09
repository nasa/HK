/************************************************************************
 * NASA Docket No. GSC-18,919-1, and identified as “Core Flight
 * System (cFS) Housekeeping (HK) Application version 2.5.0”
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

#include "hk_app.h"
#include "hk_msg.h"
#include "hk_events.h"
#include "hk_version.h"
#include "hk_msgids.h"
#include "hk_utils.h"

#include <string.h>

/* UT includes */
#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

HK_AppData_t HK_AppData;

void HK_AppMain(void)
{
    UtPrintf("HK_AppMain Stub");
    UT_DEFAULT_IMPL(HK_AppMain);
}

int32 HK_AppInit(void)
{
    return UT_DEFAULT_IMPL(HK_AppInit);
}

int32 HK_TableInit(void)
{
    return UT_DEFAULT_IMPL(HK_TableInit);
}

void HK_AppPipe(const CFE_SB_Buffer_t *BufPtr)
{
    UT_Stub_RegisterContext(UT_KEY(HK_AppPipe), BufPtr);
    UT_DEFAULT_IMPL(HK_AppPipe);
}

void HK_SendCombinedHKCmd(const CFE_SB_Buffer_t *BufPtr)
{
    UT_Stub_RegisterContext(UT_KEY(HK_SendCombinedHKCmd), BufPtr);
    UT_DEFAULT_IMPL(HK_SendCombinedHKCmd);
}

void HK_HousekeepingCmd(const CFE_MSG_CommandHeader_t *Msg)
{
    UT_Stub_RegisterContext(UT_KEY(HK_HousekeepingCmd), Msg);
    UT_DEFAULT_IMPL(HK_HousekeepingCmd);
}

void HK_NoopCmd(const CFE_SB_Buffer_t *BufPtr)
{
    UT_Stub_RegisterContext(UT_KEY(HK_NoopCmd), BufPtr);
    UT_DEFAULT_IMPL(HK_NoopCmd);
}

void HK_ResetCtrsCmd(const CFE_SB_Buffer_t *BufPtr)
{
    UT_Stub_RegisterContext(UT_KEY(HK_ResetCtrsCmd), BufPtr);
    UT_DEFAULT_IMPL(HK_ResetCtrsCmd);
}

void HK_ResetHkData(void)
{
    UT_DEFAULT_IMPL(HK_ResetHkData);
}
