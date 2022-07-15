/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-21     jing       the first version
 */
#ifndef APPLICATIONS_MANAGER_H_
#define APPLICATIONS_MANAGER_H_
#include <rtthread.h>
#include "rtconfig.h"
#include "gpio_common.h"
#include <rtdevice.h>
#include <board.h>
#include "drv_common.h"
#include "adc.h"
#include "lora.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
extern int throttle1;//  油门1AD采集值
extern int throttle2;//  油门1AD采集值
extern int key1;   //按键 1  按下为1 松开为0
extern int key2;   //按键 2  按下为1 松开为0
extern int key3;   //按键 3  按下为1 松开为0
extern int key4;   //按键 4
extern int SA;     // 拨杆 A
extern int SB;     // 拨杆 B
extern int SC;     // 拨杆 C
extern int SD;     // 拨杆 D
extern int dump_energy;
extern float dump_energy_list[5];
extern int dump_energy_count;
extern unsigned int ship_id;
extern unsigned int set_ship_id;
extern int b_set_ok;
int thread_manager(void);
#endif /* APPLICATIONS_MANAGER_H_ */
