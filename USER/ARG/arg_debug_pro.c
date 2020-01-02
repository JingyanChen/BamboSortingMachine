#include <string.h>
#include <stdio.h>
#include "csp_uart.h"
#include "csp_gpio.h"
#include "csp_timer.h"
#include "arg_debug_pro.h"
#include "delay.h"
#include "csp_pwm.h"
#include "csp_wtd.h"
#include "periph_motor.h"
#include "arg_version.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "app.h"

void arg_debug_pro_init(void){
    uint8_t welcom_string[200];

    sprintf((char *)welcom_string,"\r\nWelcome to comegene debug instruction systems.Version %d.%d.%d [make_time:%s_%s] \r\nType ""help"",""?"",""copyright"" or ""author"" for more information.\r\n",MAIN_VERSION,SECOND_VERSION,IS_RELEASE,__DATE__, __TIME__);
    debug_sender_str(welcom_string);

}

static void debug_send_nop(void){
    csp_wtd_handle();
    delay_ms(10);
}

static void help(void){
    debug_sender_str("\r\n\r\n\r\n help cmd list >>>>>>>\r\n");debug_send_nop();
    debug_sender_str(" 1  make_zhuazi_pluse pluse \r\n");debug_send_nop();
    debug_sender_str(" 2  make_pos_pluse pulse\r\n");debug_send_nop();
    debug_sender_str(" 3  set_pos_motor dir pulse\r\n");debug_send_nop();
    debug_sender_str(" 4  get_sw \r\n");debug_send_nop();
    debug_sender_str(" 5  get_app_status \r\n");debug_send_nop();
}

static void make_zhuazi_pluse(void){
    uint8_t i=0;
    uint8_t k_pos[10];
    uint8_t k=0;
    uint8_t j=0;

    uint8_t pra_str1[20];

    uint8_t send_buf[100];
    uint16_t pra1=0;

    //确定空格符号的位置
    for(i=0;i<debug_uart_rec_len;i++){
        if(debug_uart_rx_buf[i] == ' '){
            k_pos[k] = i;
            k++;
        }
    }

    if(k != 1){
        debug_sender_str("command error\r\n");
        return ;
    }
        
    
    for(i=k_pos[0]+1;i<debug_uart_rec_len-2;i++){
        pra_str1[j] = debug_uart_rx_buf[i];
        if(pra_str1[j] <'0' || pra_str1[j] >'9'){
            debug_sender_str("command error\r\n");
            return ;            
        }
        j++;
    }
		pra_str1[j] = '\0';
		
    pra1 = atoi((const char *)pra_str1);

    set_pluse(0,pra1);

    sprintf((char *)send_buf,"set zhuazi pluses  %d  success\r\n",pra1);
    
    debug_sender_str(send_buf);

}
static void make_pos_pluse(void){
    uint8_t i=0;
    uint8_t k_pos[10];
    uint8_t k=0;
    uint8_t j=0;

    uint8_t pra_str1[20];

    uint8_t send_buf[100];
    uint16_t pra1=0;

    //确定空格符号的位置
    for(i=0;i<debug_uart_rec_len;i++){
        if(debug_uart_rx_buf[i] == ' '){
            k_pos[k] = i;
            k++;
        }
    }

    if(k != 1){
        debug_sender_str("command error\r\n");
        return ;
    }
        
    
    for(i=k_pos[0]+1;i<debug_uart_rec_len-2;i++){
        pra_str1[j] = debug_uart_rx_buf[i];
        if(pra_str1[j] <'0' || pra_str1[j] >'9'){
            debug_sender_str("command error\r\n");
            return ;            
        }
        j++;
    }
		pra_str1[j] = '\0';
		
    pra1 = atoi((const char *)pra_str1);

    set_pluse(1,pra1);

    sprintf((char *)send_buf,"set pos pluses  %d  success\r\n",pra1);
    
    debug_sender_str(send_buf);
}
static void get_sw(void){
    if(get_proximity_switch_v(1)){
        debug_sender_str("\r\n sw = 1 \r\n");
    }else{
        debug_sender_str("\r\n sw = 0 \r\n");
    }
}

static void set_pos_motor(void){
//继续分析数据包，debug_uart_rx_buf,debug_uart_rec_len
    //获得 set_motor_pwm 1 200 0x0d 0x0a 解析出 1 200这两个数据出来
    uint8_t i=0;
    uint8_t k_pos[10];
    uint8_t k=0;
    uint8_t j=0;

    uint8_t pra_str1[20];
    uint8_t pra_str2[20];

    uint8_t send_buf[100];
    uint16_t pra1=0,pra2=0;

    //确定空格符号的位置
    for(i=0;i<debug_uart_rec_len;i++){
        if(debug_uart_rx_buf[i] == ' '){
            k_pos[k] = i;
            k++;
        }
    }

    if(k != 2){
        debug_sender_str("command error\r\n");
        return ;
    }
        
    
    for(i=k_pos[0]+1;i<k_pos[1];i++){
        pra_str1[j] = debug_uart_rx_buf[i];
        if(pra_str1[j] <'0' || pra_str1[j] >'9'){
            debug_sender_str("command error\r\n");
            return ;            
        }
        j++;
    }
		pra_str1[j] = '\0';
    j=0;

    for(i=k_pos[1]+1;i<debug_uart_rec_len-2;i++){
        pra_str2[j] = debug_uart_rx_buf[i];
        if(pra_str2[j] <'0' || pra_str2[j] >'9'){
            debug_sender_str("command error\r\n");
            return ;            
        }        
        j++;
    }    
		pra_str2[j] = '\0';
		
    pra1 = atoi((const char *)pra_str1);
    pra2 = atoi((const char *)pra_str2);

    if(pra1 > 1){
        debug_sender_str("dir pra error ,please input 0/1");
        return ;
    }
        

    set_motor(pra1,pra2);

    sprintf((char *)send_buf,"set pos motor dir %d pluse %d success\r\n",pra1,pra2);
    
    debug_sender_str(send_buf);
}

debug_func_list_t debug_func_list[] = {

    {help,"help"},{help,"?"},{help,"HELP"},
    {make_zhuazi_pluse,"make_zhuazi_pluse"},{make_zhuazi_pluse,"1"},
    {make_pos_pluse,"make_pos_pluse"},{make_pos_pluse,"2"},
    {set_pos_motor,"set_pos_motor"},{set_pos_motor,"3"},
    {get_sw,"get_sw"},{get_sw,"4"}, 
    {stop_at_a,"stop_at_a"},{stop_at_a,"5"},
    {stop_at_b,"stop_at_b"},{stop_at_b,"6"},
    {go_on,"go_on"}, {go_on,"7"}, 
};


static void arg_debug_packet_decode(uint8_t * buf , uint16_t len){
    uint8_t cmd_buf[50];
    uint8_t i=0;
    //对接受的到的数据包进行解析，从而做出对应的动作

    //如果单单收到的是空格符号，或者只有回车符号，那么返回comegene commander:

    if(len ==  2 && buf[0] == 0x0d && buf[1] == 0x0a){
        debug_sender_str("comegene command:");
        return ;
    }

    //判断子集，从而选择指令,从第一个字符，一直到' ' 或者0x0d为止判断完全子集
    //获得有效的数据子集

    for(i=0;i<len;i++){
        if(buf[i] == 0x0d || buf[i] == ' ')
            break;

        cmd_buf[i] = buf[i];
    }
    
    cmd_buf[i] = '\0';

    //获得了有效的指令字符串 cmd_buf 判断是否有与其完全相等的预定字符串

    for(i=0;i<sizeof(debug_func_list)/sizeof(debug_func_list_t);i++){
        if(strcmp((char *)cmd_buf,debug_func_list[i].cmd_str) == 0){
            debug_func_list[i].func();
            return ;
        }
    }

    debug_sender_str("invaild command\r\n");

}

void arg_debug_pro_handle(void){
    if(_UPLOAD_TEMP_GUI_FLAG == false)
        return;
    _UPLOAD_TEMP_GUI_FLAG = false;


    if(debug_buf_is_ready_check()){
        //data is in debug_uart_rx_buf.
        //len is debug_uart_rec_len - 1
        //debug_sender(debug_uart_rx_buf,debug_uart_rec_len);
        arg_debug_packet_decode(debug_uart_rx_buf,debug_uart_rec_len);
        //after used we must clear rx
        clear_debug_uart();
    } 
}
