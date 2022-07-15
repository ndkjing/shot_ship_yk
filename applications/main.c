/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-21     RT-Thread    first version
 */

#include <rtthread.h>
//#include <stdio.h>
#include "lora.h"
#include "gpio_common.h"
#include "manager.h"
//#include "adc.h"
#include "uart_screen.h"
int main(void)
{
    init_gpio();
    init_adc();
    thread_lora();
    start_thread_screen();
    thread_manager();

    return RT_EOK;
}

