#ifndef __APP__
#define __APP__

#include <stdbool.h>
#include <stdint.h>
/*
 * 竹条分练机的业务逻辑
 * 
 * 分两个过程
 * 
 * 1 初始化过程
 * 
 * 2 抓取释放过程
 * 
 * 因此定义如下几种逻辑状态
 * 
 * 1 未初始化状态
 * 2 就位状态READY
 * 3 夹合状态
 * 4 等待气缸下降运动状态
 * 5 等待运动电机执行状态
 * 6 打开状态
 * 7 等待运动电机回归并且回到就位状态，逻辑回到就位状态
 * 
 * 
 * 2020 1.2 新增新的测试逻辑
 * 
 * 1 到A点的逻辑，从任意位置，正转到传感器置1，然后再运行X度
 * 2 到B点的逻辑，从A点，正转180度
 * 
 * 现在的测试逻辑是，电机一直运行 A->B B->A的任务
 * A->B 等待时间 X ms
 * B->A 等待时间 Y ms
 * 
 * 
 */

//定义两种状态，非B状态和A状态，开机时处于非B状态
//开启一次，从非B状态，到达B状态
typedef enum{
    NOT_B_POS=0,
    B_POS_MID,
    B_POS,
}test_AB_status_t;

#define A_B_DEALY_TIM_MS 1
#define B_A_DEALY_TIM_MS 1

void arg_app_init(void);
void arg_app_handle(void);

#endif
