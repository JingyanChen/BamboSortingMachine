#include "stm32f10x.h"
#include "csp_gpio.h"

int main(void){	

	csp_gpio_init();
	
	while(1){
		csp_gpio_handle();
	}
	
}
