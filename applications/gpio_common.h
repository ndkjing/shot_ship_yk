/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-21     jing       the first version
 */
#ifndef APPLICATIONS_GPIO_COMMON_H_
#define APPLICATIONS_GPIO_COMMON_H_

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#define KEY1        GET_PIN(B, 12)
#define KEY2         GET_PIN(B, 13)
#define KEY3        GET_PIN(B, 14)
#define KEY4         GET_PIN(B, 15)
#define SA1         GET_PIN(D, 8)
#define SA2         GET_PIN(D, 9)
#define SB1         GET_PIN(D, 10)
#define SB2         GET_PIN(D, 11)
#define SC1         GET_PIN(D, 12)
#define SC2         GET_PIN(D, 13)
#define SD1         GET_PIN(D, 14)
#define SD2         GET_PIN(D, 15)
#define BUZZER         GET_PIN(B, 4)
#define LED1         GET_PIN(B, 5)
#define LED2         GET_PIN(B, 6)
#define LED3         GET_PIN(B, 7)
#define MD0        GET_PIN(C, 9)
#define MD1         GET_PIN(A, 8)

void control_gpio(int j1);
void init_gpio(void);
void read_gpio();
void read_key();
void lora_high();
void lora_low();
#endif /* APPLICATIONS_GPIO_COMMON_H_ */
