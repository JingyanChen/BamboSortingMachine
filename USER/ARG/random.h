#ifndef __RANDOM__
#define __RANDOM__


#include <stdint.h>
#include "csp_timer.h"

void random_init(void);
void random_handle(void);


#define SEED_MAX 10000

/*
 * brief : 获得随机种子
 * return : 随机种子
 * Note : 种子范围 0 - SEED_MAX
 */
uint16_t get_random_seed(void);

#endif