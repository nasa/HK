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
 *
 * Auto-Generated stub implementations for functions defined in hk_app header
 */

#include "hk_app.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for HK_AppInit()
 * ----------------------------------------------------
 */
int32 HK_AppInit(void)
{
    UT_GenStub_SetupReturnBuffer(HK_AppInit, int32);

    UT_GenStub_Execute(HK_AppInit, Basic, NULL);

    return UT_GenStub_GetReturnValue(HK_AppInit, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_AppMain()
 * ----------------------------------------------------
 */
void HK_AppMain(void)
{

    UT_GenStub_Execute(HK_AppMain, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_AppPipe()
 * ----------------------------------------------------
 */
void HK_AppPipe(const CFE_SB_Buffer_t *BufPtr)
{
    UT_GenStub_AddParam(HK_AppPipe, const CFE_SB_Buffer_t *, BufPtr);

    UT_GenStub_Execute(HK_AppPipe, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_HousekeepingCmd()
 * ----------------------------------------------------
 */
void HK_HousekeepingCmd(const CFE_MSG_CommandHeader_t *Msg)
{
    UT_GenStub_AddParam(HK_HousekeepingCmd, const CFE_MSG_CommandHeader_t *, Msg);

    UT_GenStub_Execute(HK_HousekeepingCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_NoopCmd()
 * ----------------------------------------------------
 */
void HK_NoopCmd(const CFE_SB_Buffer_t *BufPtr)
{
    UT_GenStub_AddParam(HK_NoopCmd, const CFE_SB_Buffer_t *, BufPtr);

    UT_GenStub_Execute(HK_NoopCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_ResetCtrsCmd()
 * ----------------------------------------------------
 */
void HK_ResetCtrsCmd(const CFE_SB_Buffer_t *BufPtr)
{
    UT_GenStub_AddParam(HK_ResetCtrsCmd, const CFE_SB_Buffer_t *, BufPtr);

    UT_GenStub_Execute(HK_ResetCtrsCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_ResetHkData()
 * ----------------------------------------------------
 */
void HK_ResetHkData(void)
{

    UT_GenStub_Execute(HK_ResetHkData, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_SendCombinedHKCmd()
 * ----------------------------------------------------
 */
void HK_SendCombinedHKCmd(const CFE_SB_Buffer_t *BufPtr)
{
    UT_GenStub_AddParam(HK_SendCombinedHKCmd, const CFE_SB_Buffer_t *, BufPtr);

    UT_GenStub_Execute(HK_SendCombinedHKCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_TableInit()
 * ----------------------------------------------------
 */
int32 HK_TableInit(void)
{
    UT_GenStub_SetupReturnBuffer(HK_TableInit, int32);

    UT_GenStub_Execute(HK_TableInit, Basic, NULL);

    return UT_GenStub_GetReturnValue(HK_TableInit, int32);
}
