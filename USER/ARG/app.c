#include "app.h"
#include "csp_pwm.h"
#include "csp_timer.h"
#include "csp_gpio.h"
#include "periph_motor.h"
#include "stdlib.h"
#include "csp_wtd.h"
#include "delay.h"

test_AB_status_t test_AB_status;

static bool stop_at_a_bool =false;
static bool stop_at_b_bool =false;

void stop_at_a(void){
    stop_at_a_bool = true;
}

void stop_at_b(void){
    stop_at_b_bool = true;
}

void go_on(void){
  stop_at_a_bool =false;
  stop_at_b_bool =false;
  set_pluse(0,0xffffffff);
  test_AB_status = NOT_B_POS;
}

void not_B_process_done(void){
    test_AB_status = NOT_B_POS;
    delay_ms(A_B_DEALY_TIM_MS);csp_wtd_handle();
    if(stop_at_b_bool == false)
        set_pluse(0,0xffffffff);
}

void EXTI1_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line1) == SET)
    {
        if(test_AB_status == NOT_B_POS){
            //if(get_proximity_switch_v(1)){
                stop_pluse(0);
				test_AB_status = B_POS;
				delay_ms(A_B_DEALY_TIM_MS);csp_wtd_handle();
                if(stop_at_a_bool == false){
				set_pluse(0,400);//旋转180度进入A
				set_motor_rundone_func(0,not_B_process_done);
                }
            }
        //}
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}


void arg_app_init(void){

    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource1);

    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    test_AB_status = NOT_B_POS;

    set_pluse(0,0xffffffff);
}

void arg_app_handle(void){
}
