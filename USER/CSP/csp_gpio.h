#include <stdbool.h>
#include "sys.h"
/*
 * GPIO define for Bambo Sorting Machine
 * 
 * 接近开关两路
 * 
 * 手动PWM 输出20KHZ PWM GPIO
 * 
 */

#define SOFTWARE_PWM_OUTPUT_PORT  GPIOB
#define SOFTWARE_PWM_OUTPUT_PIN   GPIO_Pin_9

#define POS_MOTOR_DIR_PORT GPIOG
#define POS_MOTOR_DIR_PIN  GPIO_Pin_12

#define PROXIMITY_SWITCH_PORT_0		GPIOB
#define PROXIMITY_SWITCH_PIN_0		GPIO_Pin_12

#define PROXIMITY_SWITCH_PROT_1   GPIOE
#define PROXIMITY_SWITCH_PIN_1    GPIO_Pin_1

void csp_gpio_init(void);
void csp_gpio_handle(void);

bool get_proximity_switch_v(uint8_t id);
void set_software_pwm_v(bool sw);
void set_pos_motor_dir(bool sw);
