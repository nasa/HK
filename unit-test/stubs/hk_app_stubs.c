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
CFE_Status_t HK_AppInit(void)
{
    UT_GenStub_SetupReturnBuffer(HK_AppInit, CFE_Status_t);

    UT_GenStub_Execute(HK_AppInit, Basic, NULL);

    return UT_GenStub_GetReturnValue(HK_AppInit, CFE_Status_t);
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
 * Generated stub function for HK_ResetCountersCmd()
 * ----------------------------------------------------
 */
void HK_ResetCountersCmd(const CFE_SB_Buffer_t *BufPtr)
{
    UT_GenStub_AddParam(HK_ResetCountersCmd, const CFE_SB_Buffer_t *, BufPtr);

    UT_GenStub_Execute(HK_ResetCountersCmd, Basic, NULL);
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
 * Generated stub function for HK_SendCombinedPktCmd()
 * ----------------------------------------------------
 */
void HK_SendCombinedPktCmd(const CFE_SB_Buffer_t *BufPtr)
{
    UT_GenStub_AddParam(HK_SendCombinedPktCmd, const CFE_SB_Buffer_t *, BufPtr);

    UT_GenStub_Execute(HK_SendCombinedPktCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_SendHkCmd()
 * ----------------------------------------------------
 */
void HK_SendHkCmd(const CFE_SB_Buffer_t *BufPtr)
{
    UT_GenStub_AddParam(HK_SendHkCmd, const CFE_SB_Buffer_t *, BufPtr);

    UT_GenStub_Execute(HK_SendHkCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_TableInit()
 * ----------------------------------------------------
 */
CFE_Status_t HK_TableInit(void)
{
    UT_GenStub_SetupReturnBuffer(HK_TableInit, CFE_Status_t);

    UT_GenStub_Execute(HK_TableInit, Basic, NULL);

    return UT_GenStub_GetReturnValue(HK_TableInit, CFE_Status_t);
}
