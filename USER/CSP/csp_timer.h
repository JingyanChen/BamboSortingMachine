#ifndef __TIMER__
#define __TIMER__

#include <stdint.h>
#include <stdbool.h>


/*
 * 使用timer5 来实现时间片轮询法构建工程的基本时钟
 * 此驱动给出了1ms的计数脉冲
 */

typedef struct {

uint8_t se0 :1;
uint8_t se1 :1;
uint8_t se2 :1;
uint8_t se3 :1;
uint8_t se4 :1;
uint8_t se5 :1;
uint8_t se6 :1;
uint8_t se7 :1;

}SE;

typedef struct {
SE timer_flag;
}bamboSortingMachine_t;

extern bamboSortingMachine_t  bamboSortingMachine;

#define _25US_PASS_FLAG  bamboSortingMachine.timer_flag.se0
#define _TIMER_1_MS_PASS bamboSortingMachine.timer_flag.se1
#define _UPLOAD_TEMP_GUI_FLAG bamboSortingMachine.timer_flag.se2
#define _UART_PT100_10MS_FLAG bamboSortingMachine.timer_flag.se3
#define _UART_LCD_10MS_FLAG bamboSortingMachine.timer_flag.se4
#define _PLUSE_MAKER_FLAG bamboSortingMachine.timer_flag.se5
#define _KEY_EVENT_CHECK_FLAG bamboSortingMachine.timer_flag.se6
void csp_timer_init(void);
void csp_timer_handle(void);


//TODO 
//示波器查询1ms定时器中断时间长度是否准确

#endif
