# raspberry
学习raspberry的代码，
1. avoidance_sensor.c 配置红外避障传感器，通过回调函数实现避障功能
2. blink.c            控制led灯闪烁，简单的GPIO输出demo
3. callservo.c        调用servoblaster进程，使GPIO口输出电调信号（周期20ms，高电平在1-2ms之间）
4. getdistance.c      配置超声波测距传感器，通过回调函数输出测距结果
5. wheel.c            控制L298N模块，实现车轮前进、后退、左转、右转




