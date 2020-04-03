# stm32l011xx-cmake

## About

That project is based on STM32 MCU Support Package and provide basic environment setup of STM32L011xx based on cmake.

## Requirements
    cmake >= 3.0
    GCC toolchain with newlib (optional).

## Configuration

    TOOLCHAIN_PREFIX - where toolchain is located, default: /usr
    TARGET_TRIPLET - toolchain target triplet, default: arm-none-eabi

## Usage with Clion:

Open cmake file as a project in clion.
Configure OpenOCD in clion.

## Note:
You can use this cmake file to setup environment for other stm32 controllers by downloading corresponding STM32 MCU Support Package and replacing it with current files.
