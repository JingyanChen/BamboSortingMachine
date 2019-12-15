#include "stm32f10x.h"
#include "delay.h"
#include "csp_gpio.h"
#include "csp_pwm.h"
#include "csp_wtd.h"
#include "csp_uart.h"
#include "csp_timer.h"
#include "arg_debug_pro.h"
#include "periph_key.h"
#include "app.h"

int main(void){	
	
	delay_init();

	csp_gpio_init();
	csp_uart_init();
	csp_pwm_init();
	csp_wtd_init();
	csp_timer_init();
	periph_key_init();

	arg_debug_pro_init();
	arg_app_init();

	while(1){
		csp_timer_handle();
		csp_gpio_handle();
		csp_uart_handle();
		//csp_pwm_handle();
		csp_wtd_handle();

		periph_key_handle();
		arg_debug_pro_handle();

		arg_app_handle();
	}
	
}
