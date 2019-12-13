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

#define SOFTWARE_PWM_OUTPUT_PORT  GPIOF
#define SOFTWARE_PWM_OUTPUT_PIN   GPIO_Pin_13

#define PROXIMITY_SWITCH_PORT_0		GPIOF
#define PROXIMITY_SWITCH_PIN_0		GPIO_Pin_14

#define PROXIMITY_SWITCH_PROT_1   GPIOF
#define PROXIMITY_SWITCH_PIN_1    GPIO_Pin_15

void csp_gpio_init(void);
void csp_gpio_handle(void);

bool get_proximity_switch_v(uint8_t id);
void set_software_pwm_v(bool sw);
