#include "csp_pwm.h"
#include "periph_motor.h"
#include "csp_gpio.h"

void set_motor(bool dir , uint32_t pluse){
    if(dir){
        set_pos_motor_dir(true);
    }else{
        set_pos_motor_dir(false);
    }
    set_pluse(1,pluse);
}
