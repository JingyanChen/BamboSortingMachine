#include "csp_pwm.h"
#include "csp_timer.h"
#include "csp_gpio.h"
#include "sys.h"
#include "app.h"

pluse_maker_status_t pluse_maker_status[2];

uint32_t pluse_need_make = 0;
uint32_t pluse_made=0;
bool software_pluse_bool=false;

uint32_t pluse_make_tim_25us = 0;
uint32_t now_pluse_make_tim = 0;

void null_func(void){

}

motor_run_done_func_t motor_run_done_func[2]={
    null_func,
    null_func,
};

void set_motor_rundone_func(uint8_t id , motor_run_done_func_t func){
    motor_run_done_func[id % 2] = func;
}

void csp_pwm_init(void){

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //

    TIM_TimeBaseStructure.TIM_Period = 4499; //16khz
    TIM_TimeBaseStructure.TIM_Prescaler = 0; //
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //
    TIM_OCInitStructure.TIM_Pulse = 0; //
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //

    TIM_OC2Init(TIM4, &TIM_OCInitStructure); //

    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //

    TIM_ARRPreloadConfig(TIM4, ENABLE); //
    TIM_Cmd(TIM4, ENABLE); //

    TIM_SetCompare2(TIM4,0);

    set_software_pwm_v(false);
    pluse_need_make = 0 ;
    software_pluse_bool = false;
    pluse_made = 0;


}


void open_pos_motor_pwm(void){
    TIM_SetCompare2(TIM4,2249);
}

void close_pos_motor_pwm(void){
    TIM_SetCompare2(TIM4,0);
}


void set_pluse(uint8_t id , uint32_t pluse){

    float make_tim=0.0;

    if(id == 0){
        pluse_need_make = pluse;
        pluse_made = 0;
        pluse_maker_status[id] = pluse_make_busy ;
        software_pluse_bool = false;
    }else{
			
        //16KHZ的方波计算生成pluse个方波需要多少时间 单位25us

        make_tim = (float)pluse / 16000.0; //算出来make_tim是需要的时间 单位是ms
        make_tim = make_tim * 400.0 ;//换算成 25us 为单位

        pluse_make_tim_25us = (uint32_t)make_tim;
        pluse_maker_status[id] = pluse_make_busy ;

        open_pos_motor_pwm(); //打开PWM 直到时间到达 pluse_make_tim_25us 个25us
    }
}

void stop_pluse(uint8_t id ){

    if(id == 0){
        pluse_need_make = 0;
        pluse_made = 0;
        pluse_maker_status[id] = pluse_make_done ;
        software_pluse_bool = false;
    }else{
        close_pos_motor_pwm();
        //硬件输出固定脉冲数结束
        pluse_maker_status[id] = pluse_make_done;
        now_pluse_make_tim = 0;
        pluse_make_tim_25us = 0;       
    }    
}

pluse_maker_status_t get_pluse_maker(uint8_t id){
    return pluse_maker_status[id % 2];
}


void csp_pwm_handle(void){
    uint8_t i=0;

    //if(_PLUSE_MAKER_FLAG == false)
        //return ;

    //_PLUSE_MAKER_FLAG = false;

    for(i=0;i<2;i++){
        if(pluse_maker_status[i] == pluse_make_busy){                                            
            if(i == 0){
                if(software_pluse_bool==false){
                    set_software_pwm_v(true);
                    software_pluse_bool = true;
                }else{
                    set_software_pwm_v(false);
                    software_pluse_bool = false;   
                    pluse_made ++;
                    if(pluse_made > pluse_need_make - 1){
                        //软件脉冲输出结束
                        pluse_maker_status[i] = pluse_make_done;
                        motor_run_done_func[0]();
                    }                 
                }
            }else{
                now_pluse_make_tim ++;

                if(now_pluse_make_tim > pluse_make_tim_25us){
                    close_pos_motor_pwm();
                    //硬件输出固定脉冲数结束
                    pluse_maker_status[i] = pluse_make_done;
                    now_pluse_make_tim = 0;
                    pluse_make_tim_25us = 0;
                    motor_run_done_func[1]();
                }
            }
        }
    }
}
