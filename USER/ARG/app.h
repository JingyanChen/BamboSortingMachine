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
 */

typedef enum{
    /*
     * 开机处于未初始化状态，爪子运动，一直到传感器置位，继续旋转270度，
     * 进入READY_STATUS
     */
    UNINITIALIZED=0,
    /*
     * 爪子复位操作成功后，进入ready状态
     */
    READY_STATUS,
    /*
     * 等待外部信号传入，目前测试版本是一个电容按键事件
     * 爪子电机正转90度，或者一步一步转一直转到传感器置位
     * 进入夹合态，目前暂时实现开环的举例控制，不使用传感器作为夹合态的依据
     */
    CLAMPING_STATUS,
    /*
     * 进入夹合态后，等待可编程事件T S
     * 等待抬起竹条的气缸下落事件
     * 系统会等待一段时间不做任何事情
     */
    WAIT_LIFT_DOWN_DELAY_STATUS,

    /*
     * 位置电机运动，正式版本由通讯完成从八个脉冲选择一组，进行运动
     * 测试版本随机从八个脉冲中选择1个作为运动脉冲，八个脉冲四个一组，方向相反
     */
    POS_MOTOR_RUN_STATUS,

    /*
     * 运动结束后，爪子立刻正转180度进入打开状态
     * 
     */
    OPEN_STATUS,
    
    /*
     * 打开动作结束后，伺服电机归位，同时爪子正转90度回到中立状态
     * 注意状态机归位的时机是在位置电机运动结束后发生的。
     */
    POS_MOTOR_RETURN_STATUS,
		
		NOP__,

}app_status_t;

typedef struct{
    bool dir;
    uint32_t pluse;
}pos_motor_data_t;

void set_app_status(app_status_t app_status_);
app_status_t get_app_status(void);


void arg_app_init(void);
void arg_app_handle(void);

#endif
