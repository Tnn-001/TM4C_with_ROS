#ifndef PID_H_
#define PID_H_
#include <stdint.h>
#include "tm4c123.h"
int Velocity_FeedbackControl1(int TargetVelocity, int CurrentVelocity);
int Velocity_FeedbackControl2(int TargetVelocity, int CurrentVelocity);
int Velocity_Restrict(int PWM_P);
#endif
