#include "sys.h"
#include "csp_timer.h"

bamboSortingMachine_t  bamboSortingMachine;


void csp_timer_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 1799; //FRE = 72000000/1800 = 40khz
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM5, ENABLE);
}

void TIM5_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        //25us pass
        _25US_PASS_FLAG = true;
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    }
}

static uint16_t _1ms_tick=0;
static uint16_t _10ms_tick = 0;
static uint16_t _100ms_tick = 0;
static uint16_t _1000ms_tick = 0;
static void csp_25_us_handle(void){

    if(_25US_PASS_FLAG == false)
        return ;

    _25US_PASS_FLAG = false;

    _1ms_tick++;

    if(_1ms_tick > 400){
    // 1 ms code
    _1ms_tick = 0;
    _10ms_tick++;
    _TIMER_1_MS_PASS = true;
    //
    }
}

static void csp_10_ms_handle(void){
    if(_10ms_tick < 10)
        return ;

    _10ms_tick = 0;

    // 10 ms code
    //

    _100ms_tick ++;
}

static void csp_100_ms_handle(void){
    if(_100ms_tick < 10)
        return ;
    
    _100ms_tick = 0;

    // 100 ms code
    //

    _1000ms_tick ++;

}
#define BEEP PBout(8)	 
static void csp_1000_ms_handle(void){
    if(_1000ms_tick < 10)
        return ;
    
    _1000ms_tick = 0;

    // 1000 ms code

    //

}

void csp_timer_handle(void)
{
    csp_25_us_handle();
    csp_10_ms_handle();
    csp_100_ms_handle();
    csp_1000_ms_handle();
}
