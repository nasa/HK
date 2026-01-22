/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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

/*
 * Includes
 */

#include "hk_app.h"
#include "hk_cmds.h"
#include "hk_msg.h"
#include "hk_events.h"
#include "hk_utils.h"
#include "hk_dispatch.h"
#include "hk_test_utils.h"
#include "hk_eds_dispatcher.h"
#include <unistd.h>
#include <stdlib.h>

/* UT includes */
#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

/*
**********************************************************************************
**          TEST CASE FUNCTIONS
**********************************************************************************
*/

void Test_HK_AppPipe(void)
{
    /*
     * Test Case For:
     * void HK_AppPipe
     */
    CFE_SB_Buffer_t UtBuf;

    UT_SetDeferredRetcode(UT_KEY(CFE_EDSMSG_Dispatch), 1, CFE_SUCCESS);

    memset(&UtBuf, 0, sizeof(UtBuf));
    UtAssert_VOIDCALL(HK_AppPipe(&UtBuf));
}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    UtTest_Add(Test_HK_AppPipe, HK_Test_Setup, HK_Test_TearDown, "Test_HK_AppPipe");
}
