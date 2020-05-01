set (CROSS_PREFIX           "arm-none-eabi-" CACHE STRING "")
set (CMAKE_SYSTEM_PROCESSOR "arm" CACHE STRING "")
set (MACHINE "metal" CACHE STRING "")

set (CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -ffunction-sections -I/home/nicolas/work/siema/vendors/ST/stm32mp1_m4_trial/OpenAMP/libmetal/build/output/usr/local/include -c" CACHE STRING "")
set (CMAKE_FIND_ROOT_PATH /home/nicolas/work/siema/vendors/ST/stm32mp1_m4_trial/OpenAMP/libmetal/build/output/usr/local/lib )


set (LIBMETAL_INCLUDE_DIR /home/nicolas/work/siema/vendors/ST/stm32mp1_m4_trial/OpenAMP/libmetal/build/output/usr/local/lib )
include (cross_generic_gcc)

# vim: expandtab:ts=2:sw=2:smartindent
