#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Author: Farzan Saleem <farzansaleem1@gmail.com>
#

cmake_minimum_required(VERSION 3.0)
# Project title and languages
project (STM32L011XX C CXX)

# Controller Definition
add_definitions(-DSTM32L011xx)
add_definitions(-DUSE_HAL_DRIVER)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR Arm)
set(CMAKE_BUILD_TYPE Debug)

# Toolchain path
set(TOOLCHAIN_PATH  "$ENV{HOME}/opt/gcc-arm-none-eabi/bin")

# Specify C, C++ Compiler
set (CMAKE_ASM_COMPILER ${TOOLCHAIN_PATH}/arm-none-eabi-as)
set (CMAKE_C_COMPILER   ${TOOLCHAIN_PATH}/arm-none-eabi-gcc)
set (CMAKE_CXX_COMPILER  ${TOOLCHAIN_PATH}/arm-none-eabi-g++)
set (CMAKE_OBJCOPY      ${TOOLCHAIN_PATH}/arm-none-eabi-objcopy)
set (CMAKE_OBJDUMP      ${TOOLCHAIN_PATH}/arm-none-eabi-objdump)
set (CMAKE_SIZE         ${TOOLCHAIN_PATH}/arm-none-eabi-size)

# Linker Script path
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/ld.ld")

# Startup file
file (GLOB_RECURSE STARTUP "sys/startup_stm32l011xx.s")
set_property(SOURCE ${STARTUP} PROPERTY LANGUAGE C)

# Map File
set(MAP_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-debug/${PROJECT_NAME}.map)

# Compiler and linker flags for both debug and release build
set(COMMON_FLAGS "-Og \
                  -mcpu=cortex-m0plus \
                  -mthumb \
                  -fdata-sections \
                  -ffunction-sections \
                  -Wall"
)

# Compiler options for Debug build type
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Build Type: Debug")
    set(CMAKE_C_FLAGS_DEBUG "${COMMON_FLAGS} -std=gnu11 -g -gdwarf-3 -gstrict-dwarf" CACHE INTERNAL "c compiler flags")
    set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_FLAGS} -std=c++17 -g -gdwarf-3 -gstrict-dwarf" CACHE INTERNAL "cxx compiler flags")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

# Compiler options for Release build type
if(CMAKE_BUILD_TYPE MATCHES Release)
    message("Build Type: Release")
    set(CMAKE_C_FLAGS_RELEASE "${COMMON_FLAGS} -std=gnu11" CACHE INTERNAL "c compiler flags")
    set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_FLAGS} -std=c++17" CACHE INTERNAL "cxx compiler flags")
endif(CMAKE_BUILD_TYPE MATCHES Release)

# Path for system headers
include_directories(SYSTEM sys/CMSIS/Device/ST/STM32L0xx/Include
                           sys/CMSIS/Include
                           sys/config
                           sys/STM32L0xx_HAL_Driver/Inc
                           sys/STM32L0xx_HAL_Driver/Inc/Legacy
)

# Path for system sources
file (GLOB_RECURSE CMSIS sys/config/*.c
                         sys/STM32L0xx_HAL_Driver/Src/*.c
)

# Path for user headers
include_directories(src
)

# Path for user sources
file (GLOB_RECURSE SOURCE src/*.c
                          src/*.cpp
)

add_executable(${PROJECT_NAME}.elf ${SOURCE} ${CMSIS} ${STARTUP})

set(CMAKE_EXE_LINKER_FLAGS "-mcpu=cortex-m0plus -mthumb -specs=nano.specs -specs=nosys.specs -T${LINKER_SCRIPT} -lc -lm -lnosys -Wl,-Map=${MAP_FILE},--cref -Wl,--gc-sections")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

set(ELF_FILE ${PROJECT_NAME}.elf)
set(BIN_FILE ${PROJECT_NAME}.bin)
set(HEX_FILE ${PROJECT_NAME}.hex)
set(BIN_EEPROM_FILE ${PROJECT_NAME}.eeprom.bin)

add_custom_command(
        TARGET ${PROJECT_NAME}.elf
        COMMENT "Building ${BIN_FILE}"
        COMMAND arm-none-eabi-objcopy -O ihex ${ELF_FILE} ${HEX_FILE} # convert .elf to .hex
        COMMAND arm-none-eabi-objcopy -O binary ${ELF_FILE} ${BIN_FILE} # convert .elf to .bin
        COMMAND arm-none-eabi-size --format=berkeley ${ELF_FILE}
        COMMAND arm-none-eabi-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O binary ${ELF_FILE} ${BIN_EEPROM_FILE}
)
