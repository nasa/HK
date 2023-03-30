project(CFS_HK C)

set(APP_SRC_FILES
  fsw/src/hk_app.c
  fsw/src/hk_dispatch.c
  fsw/src/hk_utils.c
)

# Create the app module
add_cfe_app(hk ${APP_SRC_FILES})

# This permits direct access to public headers in the fsw/inc directory
target_include_directories(hk PUBLIC fsw/inc)

set(APP_TABLE_FILES
  fsw/tables/hk_cpy_tbl.c
)

add_cfe_tables(hk ${APP_TABLE_FILES})

# If UT is enabled, then add the tests from the subdirectory
# Note that this is an app, and therefore does not provide
# stub functions, as other entities would not typically make
# direct function calls into this application.
if(ENABLE_UNIT_TESTS)
  add_subdirectory(unit-test)
endif()
