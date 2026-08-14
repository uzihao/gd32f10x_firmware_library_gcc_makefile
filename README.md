# gd32f10x firmware library with gcc and makefile support

This is pre-converted gd32f10x firmware library with gcc and makefile support from GigaDevice official 'GD32F10x_Firmware_Library_V2.2.4.rar'.

For more information about how to use this library, refer to [this tutorial](https://github.com/cjacker/opensource-toolchain-stm32).

This firmware library support gd32f103/105/107 parts from GigaDevice:

The default part is set to 'gd32f103cbt6' for [WeAct GD32 Bluepill Plus](https://github.com/WeActStudio/WeActStudio.BluePill-Plus-GD32).

The default 'User' codes is blinking the LED connect to PB2.

To build the project, type `make`.

## FreeRTOS support

This project includes FreeRTOS-Kernel V11.3.0 in
`Utilities/Third_Party/FreeRTOS-Kernel`.  The kernel directory is third-party
source code and is kept separate from the board/application configuration in
`User/config/FreeRTOSConfig.h`.

The default build enables FreeRTOS (`FREERTOS_ENABLED ?= 1` in `Makefile`). It
uses the GCC Cortex-M3 port and the `heap_4.c` allocator. The following kernel
sources are included by the Makefile:

- `tasks.c`, `queue.c`, `list.c`
- `timers.c`, `event_groups.c`, `stream_buffer.c`
- `portable/GCC/ARM_CM3/port.c`
- `portable/MemMang/heap_4.c`

### Build

If the Arm GNU Toolchain is not already in `PATH`, pass its `bin` directory to
make:

```sh
make GCC_PATH=/opt/arm-gnu-toolchain-15.3.rel1-x86_64-arm-none-eabi/bin
```

The generated firmware is placed in `build/` as `.elf`, `.hex`, and `.bin`.

### FreeRTOS configuration

`User/config/FreeRTOSConfig.h` is configured for this Cortex-M3 target:

- `configCPU_CLOCK_HZ` follows `SystemCoreClock`.
- SysTick runs at 1000 Hz (`configTICK_RATE_HZ = 1000`).
- `heap_4.c` provides a 16 KiB FreeRTOS heap.
- NVIC implements four priority bits. Interrupts using `...FromISR()` APIs must
  have logical priority 5 through 15; priorities 0 through 4 must not call
  FreeRTOS APIs.
- Stack-overflow checking is enabled. `vApplicationStackOverflowHook()` stops
  the CPU with interrupts disabled so the failure can be inspected with a
  debugger.

### SysTick and exception handlers

FreeRTOS owns the `SVC`, `PendSV`, and `SysTick` exceptions. In FreeRTOS mode,
`User/gd32f10x_it.c` forwards these handlers to the Cortex-M3 FreeRTOS port and
`User/systick.c` does not configure SysTick. Application tasks must use
`vTaskDelay()` / `pdMS_TO_TICKS()` instead of the bare-metal `delay_1ms()`.

The same source files can also be used without FreeRTOS. Build the original
bare-metal behavior with:

```sh
make FREERTOS_ENABLED=0 GCC_PATH=/opt/arm-gnu-toolchain-15.3.rel1-x86_64-arm-none-eabi/bin
```

In bare-metal mode `systick_config()`, `delay_1ms()`, and
`delay_decrement()` are compiled back in, and `SysTick_Handler()` uses the
original delay counter.


# to support other parts
To support other GD32F10x parts, you need:

- change 'Firmware/Ld/Link.ld', set FLASH and RAM size according to your MCU.
- choose correct startup asm file and change the 'ASM_SOURCES' in 'Makefile'
  + Firmware/CMSIS/GD/GD32F10x/Source/GCC/startup_gd32f10x_cl.S : for 105 and 107
  + Firmware/CMSIS/GD/GD32F10x/Source/GCC/startup_gd32f10x_hd.S : for flash size range from 256K to 512K
  + Firmware/CMSIS/GD/GD32F10x/Source/GCC/startup_gd32f10x_md.S : for flash size range from 16K to 128K 
  + Firmware/CMSIS/GD/GD32F10x/Source/GCC/startup_gd32f10x_xd.S : for flash size > 512K
- change 'ASM_SOURCES' in 'Makefile' to include the correct startup asm file.
- change `-DGD32F10X_MD` C_DEFS in 'Makefile' to `MD`, `HD`, `XD` or `CL` according to your MCU.
- change the 'TARGET' in 'Makefile'
- change CPU = -mcpu=cortex-m3

# 需要安装的软件
apt install openocd
apt install gdb-multiarch
