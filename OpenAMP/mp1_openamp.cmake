set (CROSS_PREFIX           "arm-none-eabi-" CACHE STRING "")
set (CMAKE_SYSTEM_PROCESSOR "arm" CACHE STRING "")
set (MACHINE "metal" CACHE STRING "")

set (LIBMETAL_INCLUDE_DIR ../../libmetal/build/output/usr/local/lib )
set (LIBMETAL_HDR_DIR ../../libmetal/build/output/usr/local/include )
set (CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -ffunction-sections -I${LIBMETAL_HDR_DIR} -c" CACHE STRING "")
set (CMAKE_FIND_ROOT_PATH ${LIBMETAL_INCLUDE_DIR} )

include (cross_generic_gcc)

# vim: expandtab:ts=2:sw=2:smartindent
