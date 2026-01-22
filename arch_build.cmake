###########################################################
#
# HK App platform build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the app configuration
set(HK_PLATFORM_CONFIG_FILE_LIST
  hk_internal_cfg_values.h
  hk_msgids.h
  hk_msgid_values.h
  hk_platform_cfg.h
)

generate_configfile_set(${HK_PLATFORM_CONFIG_FILE_LIST})
