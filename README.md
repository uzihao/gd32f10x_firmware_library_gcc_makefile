# 支持 GCC 和 Makefile 的 GD32F10x 固件库

[Chinese](README.md)

本项目由兆易创新官方发布的 `GD32F10x_Firmware_Library_V2.2.4.rar` 预先转换而来，提供 GCC 和 Makefile 构建支持。

有关该固件库使用方法的更多信息，请参阅[此教程](https://github.com/cjacker/opensource-toolchain-stm32)。

该固件库支持兆易创新的 GD32F103、GD32F105 和 GD32F107 系列芯片。

默认芯片型号设置为 `gd32f103rct6`，适用于 [GD32F103RCT6 GD32F303RCT6 STM32F103RCT6 ARM系统板核心板开发](https://item.taobao.com/item.htm?id=679397098297)。

## FreeRTOS 支持

本项目在 `Utilities/Third_Party/FreeRTOS-Kernel` 中包含 FreeRTOS-Kernel V11.3.0。内核目录作为第三方源代码，与位于 `User/config/FreeRTOSConfig.h` 的开发板和应用配置分开管理。

默认构建会启用 FreeRTOS（`Makefile` 中的 `FREERTOS_ENABLED ?= 1`），使用 GCC Cortex-M3 移植层和 `heap_4.c` 内存分配器。Makefile 中包含以下内核源文件：

- `tasks.c`、`queue.c`、`list.c`
- `timers.c`、`event_groups.c`、`stream_buffer.c`
- `portable/GCC/ARM_CM3/port.c`
- `portable/MemMang/heap_4.c`

### 构建

如果 Arm GNU Toolchain 的可执行文件目录尚未加入 `PATH`，可以在执行 make 时通过 `GCC_PATH` 指定其 `bin` 目录：

```sh
make GCC_PATH=/opt/arm-gnu-toolchain-15.3.rel1-x86_64-arm-none-eabi/bin
```

生成的 `.elf`、`.hex` 和 `.bin` 固件文件位于 `build/` 目录中。

### 应用程序目录结构

应用相关代码按功能组织在 `User/` 目录下，避免新增外设和 RTOS 任务不断堆积到 `main.c` 中：

- `app/`：应用程序启动和任务注册。
- `bsp/`：开发板引脚连接以及可复用的板级外设，例如 LED 和调试串口。
- `platform/gd32f10x/`：GD32F10x 特定的启动支持、SysTick 和异常处理函数。
- `chips/`：外部芯片驱动，例如 W25Q Flash、传感器和显示屏。
- `config/`：构建配置，包括 `FreeRTOSConfig.h`。
- `tasks/`：各个 RTOS 任务模块。新增任务应放在此目录，并在 `app/app_startup.c` 中注册。

`User/main.c` 被有意限制为只包含 C 程序入口。开发板引脚映射统一放在 `User/bsp/board.c` 中；当前开发板使用 PA0 驱动 LED，并使用 USART0 的 PA9 作为调试串口发送引脚。

### FreeRTOS 配置

`User/config/FreeRTOSConfig.h` 已针对当前 Cortex-M3 目标进行配置：

- `configCPU_CLOCK_HZ` 跟随 `SystemCoreClock`。
- SysTick 频率为 1000 Hz（`configTICK_RATE_HZ = 1000`）。
- `heap_4.c` 提供 16 KiB FreeRTOS 堆空间。
- NVIC 实现了 4 个优先级位。调用 `...FromISR()` API 的中断必须使用逻辑优先级 5～15；优先级 0～4 的中断不能调用 FreeRTOS API。
- 已启用任务栈溢出检查。`vApplicationStackOverflowHook()` 会在关闭中断后停止 CPU，以便通过调试器检查故障。

### SysTick 和异常处理函数

FreeRTOS 接管 `SVC`、`PendSV` 和 `SysTick` 异常。在 FreeRTOS 模式下，`User/platform/gd32f10x/gd32f10x_it.c` 会将这些异常处理函数转发给 Cortex-M3 FreeRTOS 移植层，同时 `User/platform/gd32f10x/systick.c` 不再配置 SysTick。应用任务必须使用 `vTaskDelay()` 和 `pdMS_TO_TICKS()`，不能使用裸机模式下的 `delay_1ms()`。

同一套源文件也可以在不使用 FreeRTOS 的情况下构建。使用以下命令构建原始裸机程序：

```sh
make FREERTOS_ENABLED=0 GCC_PATH=/opt/arm-gnu-toolchain-15.3.rel1-x86_64-arm-none-eabi/bin
```

在裸机模式下，`systick_config()`、`delay_1ms()` 和 `delay_decrement()` 会重新参与编译，`SysTick_Handler()` 使用原始的延时计数器。

# 支持其他型号

如需支持其他 GD32F10x 型号，需要完成以下修改：

- 修改 `Firmware/Ld/Link.ld`，根据 MCU 容量设置 FLASH 和 RAM 大小。
- 选择正确的启动汇编文件，并修改 `Makefile` 中的 `ASM_SOURCES`：
  + `Firmware/CMSIS/GD/GD32F10x/Source/GCC/startup_gd32f10x_cl.S`：适用于 GD32F105 和 GD32F107。
  + `Firmware/CMSIS/GD/GD32F10x/Source/GCC/startup_gd32f10x_hd.S`：适用于 Flash 容量为 256K～512K 的型号。
  + `Firmware/CMSIS/GD/GD32F10x/Source/GCC/startup_gd32f10x_md.S`：适用于 Flash 容量为 16K～128K 的型号。
  + `Firmware/CMSIS/GD/GD32F10x/Source/GCC/startup_gd32f10x_xd.S`：适用于 Flash 容量大于 512K 的型号。
- 修改 `Makefile` 中的 `ASM_SOURCES`，使其包含正确的启动汇编文件。
- 根据 MCU 型号，将 `Makefile` 的 `C_DEFS` 中的 `-DGD32F10X_MD` 修改为 `MD`、`HD`、`XD` 或 `CL`。
- 修改 `Makefile` 中的 `TARGET`。
- 将 CPU 设置为 `CPU = -mcpu=cortex-m3`。

# Ubuntu环境下需要安装的软件

```sh
apt install openocd
apt install gdb-multiarch
```
