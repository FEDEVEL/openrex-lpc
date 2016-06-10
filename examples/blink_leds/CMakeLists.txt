##############################################################################
# override compiler settings and use compiler defined by 
# CROSS_COMPILE environment variable 
##############################################################################
include(CMakeForceCompiler)
set(CMAKE_SYSTEM_NAME Generic)
set(CROSS_COMPILE "$ENV{CROSS_COMPILE}")
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_C_COMPILER  ${CROSS_COMPILE}gcc)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER ${CROSS_COMPILE}g++)

##############################################################################
# project settings
##############################################################################
set(PROJECT_NAME blink_leds.elf)
cmake_minimum_required(VERSION 2.8.11)
project(${PROJECT_NAME})
enable_language(ASM)

set(LPC_OPEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../lpc_open")

##############################################################################
# source files
##############################################################################
add_executable(
    ${PROJECT_NAME}
# chip definition files
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx/config_1347/cmsis_1347.h"
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx/config_1347/sys_config.h"
# spi
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx/ssp_13xx.c"
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx/ssp_13xx.h"
# clocks
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx/clock_13xx.c"
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx/clock_13xx.h"
# sysinit
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx/chip.h"
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx/chip_13xx.c"
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx/sysctl_13xx.c"
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx/sysinit_13xx.c"
# iocon
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx/iocon_13xx.h"
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx/iocon_13xx.c"
# board
    "${LPC_OPEN_DIR}/lpc_core/lpc_board/boards_13xx/fedevel_openrex/board.c"
    "${LPC_OPEN_DIR}/lpc_core/lpc_board/boards_13xx/fedevel_openrex/board.h"
    "${LPC_OPEN_DIR}/lpc_core/lpc_board/boards_13xx/fedevel_openrex/board_sysinit.c"
# gpio
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx/gpio_13xx_1.c"
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx/gpio_13xx_1.h"
# usart
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx/uart_13xx.c"
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx/uart_13xx.h"
# ring buffer for usart
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_common/ring_buffer.c"
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_common/ring_buffer.h"
# startup
    "${LPC_OPEN_DIR}/init/sysinit.c"
    "${LPC_OPEN_DIR}/init/cr_startup_lpc13xx.c"
    "${LPC_OPEN_DIR}/init/utils.c"
# app
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

##############################################################################
# include paths
##############################################################################
include_directories(
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx/config_1347"
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_13xx"
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip"
    "${LPC_OPEN_DIR}/lpc_core/lpc_chip/chip_common"
    "${LPC_OPEN_DIR}/CMSIS/CMSIS/Include"
    "${LPC_OPEN_DIR}/lpc_core/lpc_board/board_common"
    "${LPC_OPEN_DIR}/lpc_core/lpc_board/boards_13xx/fedevel_openrex"
)
set(LINKER_SCRIPT "${LPC_OPEN_DIR}/lpc_core/lpc_board/boards_13xx/fedevel_openrex/gcc/flash-target.ld")
add_custom_command(OUTPUT ${PROJECT_NAME}.bin COMMAND POST_BUILD ${CROSS_COMPILE}objcopy -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin DEPENDS ${PROJECT_NAME})
add_custom_target(${PROJECT_NAME}_bin ALL DEPENDS ${PROJECT_NAME}.bin)

##############################################################################
# common toolchain flags
##############################################################################
# produce debug symbols
set(COMMON_FLAGS "${COMMON_FLAGS} -g")
# use c99 standard with gnu extensions
set(COMMON_FLAGS "${COMMON_FLAGS} -std=gnu99")
# cpu is 'cortex-m3' architecture
set(COMMON_FLAGS "${COMMON_FLAGS} -mcpu=cortex-m3")
# some compiler distributions require to specify also instruction set
# even if the cpu does not support any other
set(COMMON_FLAGS "${COMMON_FLAGS} -mthumb")
# add defines - required by LPC code
set(COMMON_FLAGS "${COMMON_FLAGS} -DCORE_M3")
set(COMMON_FLAGS "${COMMON_FLAGS} -DDEBUG_ENABLE")


##############################################################################
# specific flags for toolchain utils
##############################################################################
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${COMMON_FLAGS}")
# don't include 'crt.o' and default startup
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostartfiles")
# don't include default libraries, for instance - gcc.a 
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nodefaultlibs")
# don't include stdlib - for instance libc.a
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostdlib")
# add memory layout
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker -T -Xlinker ${LINKER_SCRIPT}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker -Map=mapfile.txt")


