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

/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in hk_cmds header
 */

#include "hk_cmds.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for HK_NoopCmd()
 * ----------------------------------------------------
 */
CFE_Status_t HK_NoopCmd(const HK_NoopCmd_t *BufPtr)
{
    UT_GenStub_SetupReturnBuffer(HK_NoopCmd, CFE_Status_t);

    UT_GenStub_AddParam(HK_NoopCmd, const HK_NoopCmd_t *, BufPtr);

    UT_GenStub_Execute(HK_NoopCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(HK_NoopCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_ResetCountersCmd()
 * ----------------------------------------------------
 */
CFE_Status_t HK_ResetCountersCmd(const HK_ResetCountersCmd_t *BufPtr)
{
    UT_GenStub_SetupReturnBuffer(HK_ResetCountersCmd, CFE_Status_t);

    UT_GenStub_AddParam(HK_ResetCountersCmd, const HK_ResetCountersCmd_t *, BufPtr);

    UT_GenStub_Execute(HK_ResetCountersCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(HK_ResetCountersCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_SendCombinedPktCmd()
 * ----------------------------------------------------
 */
CFE_Status_t HK_SendCombinedPktCmd(const HK_SendCombinedPktCmd_t *BufPtr)
{
    UT_GenStub_SetupReturnBuffer(HK_SendCombinedPktCmd, CFE_Status_t);

    UT_GenStub_AddParam(HK_SendCombinedPktCmd, const HK_SendCombinedPktCmd_t *, BufPtr);

    UT_GenStub_Execute(HK_SendCombinedPktCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(HK_SendCombinedPktCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for HK_SendHkCmd()
 * ----------------------------------------------------
 */
CFE_Status_t HK_SendHkCmd(const HK_SendHkCmd_t *BufPtr)
{
    UT_GenStub_SetupReturnBuffer(HK_SendHkCmd, CFE_Status_t);

    UT_GenStub_AddParam(HK_SendHkCmd, const HK_SendHkCmd_t *, BufPtr);

    UT_GenStub_Execute(HK_SendHkCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(HK_SendHkCmd, CFE_Status_t);
}
