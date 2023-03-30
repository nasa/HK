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
 * Auto-Generated stub implementations for functions defined in hk_dispatch header
 */

#include "hk_dispatch.h"
#include "utgenstub.h"

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
 * Generated stub function for HK_VerifyCmdLength()
 * ----------------------------------------------------
 */
int32 HK_VerifyCmdLength(const CFE_SB_Buffer_t *BufPtr, size_t ExpectedLength)
{
    UT_GenStub_SetupReturnBuffer(HK_VerifyCmdLength, int32);

    UT_GenStub_AddParam(HK_VerifyCmdLength, const CFE_SB_Buffer_t *, BufPtr);
    UT_GenStub_AddParam(HK_VerifyCmdLength, size_t, ExpectedLength);

    UT_GenStub_Execute(HK_VerifyCmdLength, Basic, NULL);

    return UT_GenStub_GetReturnValue(HK_VerifyCmdLength, int32);
}
