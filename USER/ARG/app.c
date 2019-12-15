#include "app.h"
#include "csp_pwm.h"
#include "csp_timer.h"
#include "csp_gpio.h"
#include "periph_motor.h"
#include "stdlib.h"

static app_status_t app_status;
static uint32_t delay_tick=0;//延时变量
static uint8_t ran_data=0; //获得的随机竹条位置，后期改为通讯值


pos_motor_data_t pos_motor_data[8]={
    {0,1600000},
    {0,3200000},
    {0,4800000},
    {0,7200000},
    {1,1600000},
    {1,3200000},
    {1,4800000},
    {1,7200000},
};

void set_app_status(app_status_t app_status_){
    app_status = app_status_;
}

app_status_t get_app_status(void){
    return app_status;
}

void arg_app_init(void){
    set_app_status(UNINITIALIZED);//初始状态为未初始化状态
    //尝试初始化
    set_pluse(0,0xffffffff);
}

static void initial_done_func(void){
    set_app_status(READY_STATUS);
}
static void null_func(void){
}
static void ALL_DONE(void){
    set_app_status(READY_STATUS);
    stop_pluse(1);stop_pluse(0);
	  set_motor_rundone_func(0,null_func);
	  set_motor_rundone_func(1,null_func);
}
static void OPEN_STATUS_DONE(void){
    
    set_pluse(0,200);//继续旋转90度

    //回归电机
    if(pos_motor_data[ran_data%8].dir == 1){
        set_motor(0,pos_motor_data[ran_data%8].pluse);
    }
    else{
        set_motor(1,pos_motor_data[ran_data%8].pluse);
    }
		set_motor_rundone_func(0,null_func);
    set_motor_rundone_func(1,ALL_DONE);

}
static void POS_MOTOR_RUN_STATUS_DONE(void){
    set_app_status(OPEN_STATUS);
    set_pluse(0,400);//继续旋转180度
    set_motor_rundone_func(0,OPEN_STATUS_DONE);
}





void arg_app_handle(void){

    if(_APP_UPDATE_FLAG == false)
        return ;
    _APP_UPDATE_FLAG = false;

    switch(app_status){
        case UNINITIALIZED: 
            if(get_proximity_switch_v(1)){
                //set_app_status(READY_STATUS);
                //stop_pluse(0);
                //正转270度
                set_pluse(0,600);//正转270度
                set_motor_rundone_func(0,initial_done_func);
            }
            break;
        case READY_STATUS: break;
        case CLAMPING_STATUS: break;
        case WAIT_LIFT_DOWN_DELAY_STATUS: 
            //为快速部署，先给定一个固定的代码修改的常量作为延时时间
            delay_tick++;
            if(delay_tick > 80000) {
                //等待2S
                delay_tick = 0;
                set_app_status (POS_MOTOR_RUN_STATUS);
            }

             break;
        case POS_MOTOR_RUN_STATUS: 

            //随机从八组数据中随机获得一个数，作为运动电机运动依据
						
            ran_data = rand() % 8;
            set_motor(pos_motor_data[ran_data%8].dir,pos_motor_data[ran_data%8].pluse);

            //注册运动结束函数
            set_motor_rundone_func(1,POS_MOTOR_RUN_STATUS_DONE);
						set_app_status(NOP__);
            break;
        case OPEN_STATUS: break;
        case POS_MOTOR_RETURN_STATUS: break;
				
				default:break;
    }

}
