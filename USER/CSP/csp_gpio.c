#include "csp_gpio.h"
#include "sys.h"
#include <stdbool.h>

GPIO_TypeDef *OUTPUT_PORT_LIST[] = {
    SOFTWARE_PWM_OUTPUT_PORT,
		POS_MOTOR_DIR_PORT,
};

uint16_t OUTPUT_PIN_LIST[] = {
    SOFTWARE_PWM_OUTPUT_PIN,
		POS_MOTOR_DIR_PIN,
};

GPIO_TypeDef *INPUT_PORT_LIST[] = {
    PROXIMITY_SWITCH_PORT_0,
    PROXIMITY_SWITCH_PROT_1,
};

uint16_t INPUT_PIN_LIST[] = { 
    PROXIMITY_SWITCH_PIN_0,
    PROXIMITY_SWITCH_PIN_1,
};

void csp_gpio_init(void){

    GPIO_InitTypeDef GPIO_InitStructure;
    uint8_t i = 0;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);

    //output set

    for (i = 0; i < 2; i++)
    {

        GPIO_InitStructure.GPIO_Pin = OUTPUT_PIN_LIST[i];
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(OUTPUT_PORT_LIST[i], &GPIO_InitStructure);
        GPIO_ResetBits(OUTPUT_PORT_LIST[i], OUTPUT_PIN_LIST[i]);
    }

    //input set


    GPIO_InitStructure.GPIO_Pin = INPUT_PIN_LIST[1];
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(INPUT_PORT_LIST[1], &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = INPUT_PIN_LIST[0];
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(INPUT_PORT_LIST[0], &GPIO_InitStructure);
}


void csp_gpio_handle(void){
}

bool get_proximity_switch_v(uint8_t id){
    return GPIO_ReadInputDataBit(INPUT_PORT_LIST[ id % 2],INPUT_PIN_LIST[ id % 2]);
}

void set_software_pwm_v(bool sw){
    if(sw){
        GPIO_SetBits(SOFTWARE_PWM_OUTPUT_PORT , SOFTWARE_PWM_OUTPUT_PIN);
    }else{
        GPIO_ResetBits(SOFTWARE_PWM_OUTPUT_PORT , SOFTWARE_PWM_OUTPUT_PIN);
    }   
}

void set_pos_motor_dir(bool sw){
    if(sw){
        GPIO_SetBits(POS_MOTOR_DIR_PORT , POS_MOTOR_DIR_PIN);
    }else{
        GPIO_ResetBits(POS_MOTOR_DIR_PORT , POS_MOTOR_DIR_PIN);
    }       
}