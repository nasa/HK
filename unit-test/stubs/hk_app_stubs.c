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
