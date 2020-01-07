#include "random.h"
#include <stdint.h>
#include "csp_timer.h"

static uint16_t random_seed = 0;

uint16_t get_random_seed(void){
    return random_seed ;
}

void random_init(void){
    random_seed = false;
}

void random_handle(void){
    if(_RANDOM_SEED_UPDATE_FLAG == false)
        return ;

    _RANDOM_SEED_UPDATE_FLAG = false;

    random_seed ++;

    if(random_seed > SEED_MAX)
        random_seed = 0;
}

