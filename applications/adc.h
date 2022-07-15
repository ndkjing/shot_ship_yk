/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-21     jing       the first version
 */
#ifndef APPLICATIONS_ADC_H_
#define APPLICATIONS_ADC_H_
int init_adc();
void read_adc();
void read_energy();
int start_thread_adc(void);
#endif /* APPLICATIONS_ADC_H_ */
