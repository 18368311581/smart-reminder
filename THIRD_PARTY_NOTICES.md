# 第三方组件与随附资源

根目录 `LICENSE` 的 MIT 授权适用于 Jiang Peichen (Inseven) 拥有版权的原创内容，不覆盖第三方已有权利。所有保留的源码版权声明继续有效；单个文件中的版权和许可条款优先适用。

| 组件或资源 | 主要位置 | 版权与许可说明 |
| --- | --- | --- |
| STMicroelectronics HAL、STM32 设备文件与启动文件 | `firmware/source/Drivers/STM32F4xx_HAL_Driver/`、CMSIS 设备文件、启动汇编 | 原文件保留 ST 版权声明；可识别的文件注明 BSD 3-Clause。许可证副本见 [BSD-3-Clause-ST.txt](LICENSES/BSD-3-Clause-ST.txt)。各文件具体版权年份与说明以原文件为准 |
| Arm CMSIS 内核头文件 | `firmware/source/Drivers/CMSIS/Inc/cmsis_*.h`、`core_cm4.h`、`mpu_armv7.h` | 保留 Arm Limited 的版权和 Apache-2.0 声明；许可证副本见 [Apache-2.0.txt](LICENSES/Apache-2.0.txt) |
| FatFs R0.14b | `firmware/source/Drivers/Middlewares/Src/ff.c` 及相关接口 | 保留 Copyright (C) 2021, ChaN 及原许可；说明副本见 [FatFs.txt](LICENSES/FatFs.txt) |
| 中文字库与 Unicode/GBK 资源 | `sdcard/smart_remd_stat/GBK12.FON`、`GBK16.FON`、`GBK24.FON`、`UNIGBK.BIN` | 按作者要求，以历史项目中的原始内容随附。原始字体/编码资源的具体来源及独立授权记录未随项目保留；不将这些文件重新标为 MIT，也不声称拥有第三方字体的版权 |
| 制作说明中的产品图片、采购截图与商标 | `docs/硬件清单与安装.pdf` | 保留原说明中的硬件选型与历史参考信息；相关第三方图片、商标不因本仓库 MIT 授权而转移权利 |

预编译 HEX 包含工程使用的基础组件，其分发同时附带本文件及 `LICENSES/` 下的相应说明。

部分项目适配/中间层文件没有单独的第三方来源标注；本次保留原始文件，不据此删除任何现有声明或推定其他作者放弃权利。如果发现来源或署名遗漏，可通过本仓库 Issue 提供具体文件及出处，便于补充记录。
