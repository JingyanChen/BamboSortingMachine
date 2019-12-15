#include "periph_key.h"
#include "csp_uart.h"
#include "csp_gpio.h"
#include "csp_timer.h"
#include "app.h"
#include "csp_pwm.h"

static void CLAMPING_STATUS_DONE(void){
    set_app_status(WAIT_LIFT_DOWN_DELAY_STATUS);
    
}

static void default_key_event_0(void){
    debug_sender_str((uint8_t*)"key 0 event happend!\r\n");
    if(get_app_status() == READY_STATUS){
        set_app_status(CLAMPING_STATUS);
        set_pluse(0,200);//正转90度
        set_motor_rundone_func(0,CLAMPING_STATUS_DONE);
    }
    }

static key_event_t key_event_func[KEY_NUM]={
    default_key_event_0,

};

static event_type_t key_event_v[KEY_NUM]={
    negative,
};

static bool last_key_v[KEY_NUM]={
    false,
};

void periph_key_init(void){
    uint8_t i=0;

    for(i=0;i<KEY_NUM;i++){
        last_key_v[i] = get_proximity_switch_v(i % 6);
    }
}

void periph_key_handle(void){
    uint8_t i=0;

    if(_KEY_EVENT_CHECK_FLAG == false)
        return;

    _KEY_EVENT_CHECK_FLAG = false;

    for(i=0;i<KEY_NUM;i++){
        if(key_event_v[i] == postive){

            if(last_key_v[i] == false && get_proximity_switch_v(i % 6) == true){
                key_event_func[i]();
            }

        }else if(key_event_v[i] == negative){

            if(last_key_v[i] == true && get_proximity_switch_v(i % 6) == false){
                key_event_func[i]();
            } 
                       
        }else{

            if(last_key_v[i] == true && get_proximity_switch_v(i % 6) == false){
                key_event_func[i]();
            } 

            if(last_key_v[i] == false && get_proximity_switch_v(i % 6) == true){
                key_event_func[i]();
            }
                
        }

        last_key_v[i] = get_proximity_switch_v(i % 6);
    }
}

void register_key_press_event(uint8_t key_id , key_event_t key_event){
    key_event_func[key_id] = key_event;
}
