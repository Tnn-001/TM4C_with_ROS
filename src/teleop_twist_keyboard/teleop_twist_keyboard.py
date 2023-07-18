#!/usr/bin/env python3
# -*- coding: utf-8 -*

from std_msgs.msg import UInt32
import rospy
import os
import sys
import tty
import termios
import roslib


# 全局变量
cmd = UInt32()
pub = rospy.Publisher('desire_speed', UInt32,queue_size=1)


def keyboardLoop():
    # 初始化
    rospy.init_node('smartcar_teleop')
    rate = rospy.Rate(rospy.get_param('~hz', 1))

    # # 速度变量
    # yaw_rate_ = rospy.get_param('yaw_rate', 1.0)
    # yaw_rate_run_ = rospy.get_param('yaw_rate_run', 1.5)
    speed = 20

    # 显示提示信息
    print("Reading from keyboard")
    print("Use WASD keys to control the robot")
    print("Press Caps to move faster")
    print("Press q to quit")
    v = 0
    turn = 0
    stop = 1
    # 读取按键循环
    while not rospy.is_shutdown():
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        # 不产生回显效果
        old_settings[3] = old_settings[3] & ~termios.ICANON & ~termios.ECHO
        try:
            tty.setraw(fd)
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)

        if ch == 'w':
            v = speed
            turn = 0
            stop = 0
        elif ch == 's':
            v = -1 * speed
            turn = 0
            stop = 0
        elif ch == 'a':
            v = speed
            turn = 1
            stop=0
        elif ch == 'd':
            v = speed
            turn = -1
            stop=0
        elif ch == 'e':
            speed += 15
        elif ch == 'c':
            speed -= 5
        elif ch == 'z':
            exit()  
        elif ch == 'q':
            stop_robot()
            v = 0
            turn = 0
            speed = 20
            stop=1
        else:
            pass
        if speed > 50:
            speed = 50
        if speed < 20:
            speed = 20
        print("speed: %i ",speed)
        print("turn: %i ",turn)
        print("stop: %i ",stop)
        if stop==1:
            speed = 0

        if turn == 0:
            if v >= 0:
                cmd.data = (speed << 16) | (speed)
            else:
                cmd.data = 0xff00ff00 | ((speed << 16) | (speed))
        elif turn == 1:
            if v >= 0:
                cmd.data =0xff000000| (speed << 16) | (speed)
            else:
                cmd.data = 0x0000ff00 | ((speed << 16) | (speed))
        else:
            if v >= 0:
                cmd.data =0x0000ff00| (speed << 16) | (speed)
            else:
                cmd.data = 0xff000000 | ((speed << 16) | (speed))

            # 发送消息


        pub.publish(cmd)
        rate.sleep()
def stop_robot():
    cmd.data = 0
    pub.publish(cmd)


if __name__ == '__main__':
    try:
        keyboardLoop()
    except rospy.ROSInterruptException:
        pass
