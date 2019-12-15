#ifndef __CSP_PWM__
#define __CSP_PWM__

#include <stdint.h>
#include <stdbool.h>

void csp_pwm_init(void);
void csp_pwm_handle(void);


/*
 * 此驱动旨在提供两路步进电机控制
 * 1 路 20KHZ %50占空比输出，采用软件count的方式输出  PB9
 * 1 路 160KHZ定时器输出，实现方式采用计时打开关闭定时器的方式实现 PB7
 * 
 * 电机方向控制抽象到periph的 periph_motor层实现
 * 本驱动最终输出API
 * 
 * set_pluse(id , pluses)
 * get_pluse_maker_status(id)
 */

typedef enum{
    pluse_make_done=0,
    pluse_make_busy,
}pluse_maker_status_t;

void set_pluse(uint8_t id , uint32_t pluse);
pluse_maker_status_t get_pluse_maker(uint8_t id);
void stop_pluse(uint8_t id );

void open_pos_motor_pwm(void);
void close_pos_motor_pwm(void);


typedef void (*motor_run_done_func_t) (void);
void set_motor_rundone_func(uint8_t id , motor_run_done_func_t func);


#endif
