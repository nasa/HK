#ifndef _HK_APP_TEST_UTILS_H_
#define _HK_APP_TEST_UTILS_H_

#include "hk_app.h"

/*
 * Allow UT access to the global "HK_AppData" object.
 */
extern HK_AppData_t HK_AppData;

typedef struct
{
    uint16      ExpectedEvent;
    uint32      MatchCount;
    const char *ExpectedText;
} UT_CheckEvent_t;

/*
 * Macro to call a function and check its int32 return code
 */
#define UT_TEST_FUNCTION_RC(func, exp)                                                                \
    {                                                                                                 \
        int32 rcexp = exp;                                                                            \
        int32 rcact = func;                                                                           \
        UtAssert_True(rcact == rcexp, "%s (%ld) == %s (%ld)", #func, (long)rcact, #exp, (long)rcexp); \
    }

/*
 * Macro to add a test case to the list of tests to execute
 */
#define ADD_TEST(test) UtTest_Add((Test_##test), HK_UT_Setup, HK_UT_TearDown, #test)

/*
 * Setup function prior to every test
 */
void HK_UT_Setup(void);

/*
 * Teardown function after every test
 */
void HK_UT_TearDown(void);

#endif
