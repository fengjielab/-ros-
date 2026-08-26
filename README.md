# STM32 四电机学习历程

这是我的 STM32 学习与四电机底盘控制记录仓库。

仓库中的每个阶段都保留可以重新导入 STM32CubeIDE 的工程文件，并通过 Git 提交记录代码变化，通过项目 README 和学习日志记录硬件测试、问题和下一步计划。

## 当前进度

- [x] STM32CubeMX / STM32CubeIDE 工程搭建
- [x] PWM 输出
- [x] 四路电机方向控制
- [x] 编码器输入与测速基础
- [x] FreeRTOS 任务框架
- [ ] 完善速度闭环和 PID 参数记录
- [ ] 增加串口调参
- [ ] 增加遥控或上位机控制

## 当前项目

### motor4_speed_monitor

基于 STM32F103ZET6 的四电机控制与速度监测工程。

- 工程配置：[motor4_speed_monitor.ioc](motor4_speed_monitor/motor4_speed_monitor.ioc)
- 项目说明：[motor4_speed_monitor/README.md](motor4_speed_monitor/README.md)
- 学习日志：[docs/learning-log.md](docs/learning-log.md)

## 开发环境

- MCU：STM32F103ZET6
- 配置工具：STM32CubeMX 6.18.1
- IDE：STM32CubeIDE
- 库：STM32 HAL、FreeRTOS

## Git 使用方式

每完成一个可以验证的小功能，就在 VSCode 的源代码管理中检查 diff、暂存相关文件、提交并推送。

建议的提交信息：

```text
feat: add four motor PWM control
feat: add encoder speed measurement
fix: correct motor direction sign
docs: update wiring and test result
```

完成重要阶段后创建版本标签，例如：

```text
v0.1.0  PWM 输出
v0.2.0  编码器测速
v0.3.0  四电机控制
```

## 注意事项

`Debug/`、`Release/`、`.metadata/` 和编译中间文件已经加入 `.gitignore`，不会作为正常提交内容上传。CubeMX 的 `.ioc` 文件、CubeIDE 工程文件、源码、驱动和 FreeRTOS 文件需要保留，保证工程可以重新导入和编译。
