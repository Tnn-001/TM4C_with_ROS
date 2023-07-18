#include "pid.h"
// 外部变量 extern说明改变量已在其它文件定义
extern float Velcity_Kp, Velcity_Ki, Velcity_Kd; // 相关速度PID参数

/**************************************************************************
函数功能：速度闭环PID控制(实际为PI控制)
入口参数：目标速度 当前速度
返回  值：速度控制值
根据增量式离散PID公式
ControlVelocity+=Kp[e(k)- e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差
e(k-1)代表上一次的偏差
ControlVelocity代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
ControlVelocity+=Kp[e(k)- e(k-1)]+Ki*e(k)
**************************************************************************/
int Velocity_FeedbackControl1(int TargetVelocity, int CurrentVelocity)
{
    int Bias;                                                                 // 定义相关变量
    static int ControlVelocity1 = 500, Last_bias1;                            // 静态变量，函数调用结束后其值依然存在
    Bias = TargetVelocity - CurrentVelocity;                                  // 求速度偏差
    ControlVelocity1 += Velcity_Kp * (Bias - Last_bias1) + Velcity_Ki * Bias; // 增量式PI控制器
                                                                              // Velcity_Kp*(Bias-Last_bias) 作用为限制加速度
                                                                              // Velcity_Ki*Bias 速度控制值由Bias不断积分得到 偏差越大加速度越大
    Last_bias1 = Bias;
    // ControlVelocity1 = Velocity_Restrict(ControlVelocity1);
    return ControlVelocity1; // 返回速度控制值
}
int Velocity_FeedbackControl2(int TargetVelocity, int CurrentVelocity)
{
    int Bias;                                                                 // 定义相关变量
    static int ControlVelocity2 = 500, Last_bias2;                            // 静态变量，函数调用结束后其值依然存在
    Bias = TargetVelocity - CurrentVelocity;                                  // 求速度偏差
    ControlVelocity2 += Velcity_Kp * (Bias - Last_bias2) + Velcity_Ki * Bias; // 增量式PI控制器
                                                                              // Velcity_Kp*(Bias-Last_bias) 作用为限制加速度                                                                   // Velcity_Ki*Bias 速度控制值由Bias不断积分得到 偏差越大加速度越大
    Last_bias2 = Bias;
    // ControlVelocity2 = Velocity_Restrict(ControlVelocity2);
    return ControlVelocity2; // 返回速度控制值
}
/**************************************************************************
函数功能：速度(PWM)限幅
入口参数：PWM_P:位置环输出的PWM值 TargetVelocity:目标速度(速度限制值)
返回  值：无
**************************************************************************/
int Velocity_Restrict(int PWM_P)
{
    if (PWM_P > 1000)
        PWM_P = 1000;
    else if (PWM_P < 0)
        PWM_P = 0;
    else
        PWM_P = PWM_P;
    return PWM_P;
}
