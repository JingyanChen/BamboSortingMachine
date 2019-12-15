#ifndef __MOTOR__
#define __MOTOR__

//在csp_pwm的基础上再次封装位置电机的功能，加入方向

#include <stdint.h>
#include <stdbool.h>


void set_motor(bool dir , uint32_t pluse);

#endif
