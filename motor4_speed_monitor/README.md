# 四电机控制与速度监测

## 项目目标

使用 STM32F103ZET6 完成四个直流电机的 PWM 控制、方向控制、编码器测速，并在 FreeRTOS 任务中组织控制逻辑。

## 软件环境

- STM32CubeMX 6.18.1
- STM32CubeIDE
- STM32 HAL
- FreeRTOS

## 工程入口

- CubeMX 配置：[motor4_speed_monitor.ioc](motor4_speed_monitor.ioc)
- 主程序：[Core/Src/main.c](Core/Src/main.c)
- 电机控制：[Core/Src/motor.c](Core/Src/motor.c)
- 电机控制接口：[Core/Inc/motor.h](Core/Inc/motor.h)

## 已完成内容

- [x] 四路 PWM 输出
- [x] 四个电机的方向控制
- [x] 编码器初始化和计数
- [x] 基础速度计算
- [x] 串口启动提示或数据输出
- [x] FreeRTOS 任务框架

## 硬件记录

请补充实际使用的电机驱动板、电机型号、电源电压、编码器类型和接线图。

```text
MCU：STM32F103ZET6
电机驱动：待补充
电机：待补充
电源：待补充
编码器：待补充
```

## 测试记录

| 日期 | 测试内容 | 结果 | 备注 |
| --- | --- | --- | --- |
| 待补充 | 四路 PWM 输出 | 待补充 | 记录 PWM 范围和占空比 |
| 待补充 | 四电机方向 | 待补充 | 记录每个电机方向是否一致 |
| 待补充 | 编码器测速 | 待补充 | 记录转速、计数和误差 |

## 已知问题

- 电机方向和编码器方向符号需要结合实际接线确认。
- PID 参数、控制周期和速度单位需要继续整理并记录实测结果。

## 下一步

- [ ] 记录完整接线图
- [ ] 记录 PWM 频率和定时器时钟参数
- [ ] 记录每个电机的编码器计数方向
- [ ] 完善速度闭环测试
- [ ] 增加串口调参

## 重新导入工程

在 STM32CubeIDE 中选择 `File -> Import -> Existing Projects into Workspace`，选择当前目录，导入 `.project` 文件对应的工程即可。重新生成 CubeMX 代码前，确认自己的代码位于 `USER CODE` 区域或独立的业务文件中。
