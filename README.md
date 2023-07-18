# TM4C_with_ROS


## 目标
整体上采用上位机（树莓派，Raspberry）控制下位机（Tiva™ C Series TM4C123G LaunchPad），再通过下位机控制电机驱动模块（AT8236）和直流电机。最终实现网联、闭环控制小车，使其具备基础但是完整的轮式移动机器人整体架构。
在功能上可以实现联网远程键盘控制小车进行：前进后退、左右转、加减速和停止的功能。
在实现细节上主要有四个目标：
1. 使用TM4C控制电机驱动模块（AT8236）通过PWM驱动直流电机，并通过编码器读取电机实际转速，对电机实现PID闭环控制；
2. 实现上下位机之间的UART（Universal Asynchronous Receiver/Transmitter）通信；
3. 在上位机部署ROS系统，并成功向下位机读取/发送信息；
4. 实现键盘控制节点。

## 整体结构
![总体](https://github.com/Tnn-001/TM4C_with_ROS/assets/74281743/30595b4c-e8c6-43ce-9aa1-084bc854af54)

## 参考文献
https://zhuanlan.zhihu.com/p/495247904

https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/

