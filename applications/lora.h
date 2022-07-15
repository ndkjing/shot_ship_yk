/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-21     jing       the first version
 */
#ifndef APPLICATIONS_LORA_H_
#define APPLICATIONS_LORA_H_

void send_lora_data(int chan);
int thread_lora(void);
#endif /* APPLICATIONS_LORA_H_ */
