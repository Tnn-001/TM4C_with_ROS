#include "pid.h"
// �ⲿ���� extern˵���ı������������ļ�����
extern float Velcity_Kp, Velcity_Ki, Velcity_Kd; // ����ٶ�PID����

/**************************************************************************
�������ܣ��ٶȱջ�PID����(ʵ��ΪPI����)
��ڲ�����Ŀ���ٶ� ��ǰ�ٶ�
����  ֵ���ٶȿ���ֵ
��������ʽ��ɢPID��ʽ
ControlVelocity+=Kp[e(k)- e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ��
e(k-1)������һ�ε�ƫ��
ControlVelocity�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
ControlVelocity+=Kp[e(k)- e(k-1)]+Ki*e(k)
**************************************************************************/
int Velocity_FeedbackControl1(int TargetVelocity, int CurrentVelocity)
{
    int Bias;                                                                 // ������ر���
    static int ControlVelocity1 = 500, Last_bias1;                            // ��̬�������������ý�������ֵ��Ȼ����
    Bias = TargetVelocity - CurrentVelocity;                                  // ���ٶ�ƫ��
    ControlVelocity1 += Velcity_Kp * (Bias - Last_bias1) + Velcity_Ki * Bias; // ����ʽPI������
                                                                              // Velcity_Kp*(Bias-Last_bias) ����Ϊ���Ƽ��ٶ�
                                                                              // Velcity_Ki*Bias �ٶȿ���ֵ��Bias���ϻ��ֵõ� ƫ��Խ����ٶ�Խ��
    Last_bias1 = Bias;
    // ControlVelocity1 = Velocity_Restrict(ControlVelocity1);
    return ControlVelocity1; // �����ٶȿ���ֵ
}
int Velocity_FeedbackControl2(int TargetVelocity, int CurrentVelocity)
{
    int Bias;                                                                 // ������ر���
    static int ControlVelocity2 = 500, Last_bias2;                            // ��̬�������������ý�������ֵ��Ȼ����
    Bias = TargetVelocity - CurrentVelocity;                                  // ���ٶ�ƫ��
    ControlVelocity2 += Velcity_Kp * (Bias - Last_bias2) + Velcity_Ki * Bias; // ����ʽPI������
                                                                              // Velcity_Kp*(Bias-Last_bias) ����Ϊ���Ƽ��ٶ�                                                                   // Velcity_Ki*Bias �ٶȿ���ֵ��Bias���ϻ��ֵõ� ƫ��Խ����ٶ�Խ��
    Last_bias2 = Bias;
    // ControlVelocity2 = Velocity_Restrict(ControlVelocity2);
    return ControlVelocity2; // �����ٶȿ���ֵ
}
/**************************************************************************
�������ܣ��ٶ�(PWM)�޷�
��ڲ�����PWM_P:λ�û������PWMֵ TargetVelocity:Ŀ���ٶ�(�ٶ�����ֵ)
����  ֵ����
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
