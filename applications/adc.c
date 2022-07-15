/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-21     jing       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "adc.h"
#include "manager.h"
#define ADC_DEV_NAME        "adc1"
#define ADC_IN1     4
#define ADC_IN2     5
#define ADC_IN3     15
//static rt_thread_t thread_adc = RT_NULL;
rt_adc_device_t adc_dev;
rt_uint32_t value1, value2;
int init_adc()
{
    rt_err_t ret = RT_EOK;
    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
        return RT_ERROR;
    }
    ret = rt_adc_enable(adc_dev, ADC_IN1);
    rt_adc_enable(adc_dev, ADC_IN2);
    rt_adc_enable(adc_dev, ADC_IN3);
    return ret;
}


void adc_read_data(void *parameter)
{
    while (1)
    {
        value1 = rt_adc_read(adc_dev, ADC_IN1);
//        rt_kprintf("the value1 is :%d \n", value1);
        value2 = rt_adc_read(adc_dev, ADC_IN2);
        dump_energy = rt_adc_read(adc_dev, ADC_IN2);
        rt_kprintf("dump_energy is :%d \n", dump_energy);
        throttle1=value1*0.294+847;
        throttle2=value2*0.287+861;
        if(throttle1>1960) throttle1=2000;
        if(throttle1<1040) throttle1=1000;
        if(throttle1<1550&&throttle1>1450) throttle1=1500;
        if(throttle2>1960)  throttle2=2000;
        if(throttle2<1040) throttle2=1000;
        if(throttle2<1550&&throttle2>1450) throttle2=1500;
        throttle1=(int)((throttle1-1000)/10);
        throttle2=(int)((throttle2-1000)/10);
        rt_thread_mdelay(10);
    }
}
void read_adc()
{
    value1 = rt_adc_read(adc_dev, ADC_IN1);
    value2 = rt_adc_read(adc_dev, ADC_IN2);
//    rt_kprintf("value1,value2 is :%d  %d\n", value1,value2);

    if (value1<=430)
    {
        value1=1;
    }
    else {
        value1 -=430;
    }
    if (value2<=280)
    {
        value2=1;
    }
    else {
        value2 -=280;
    }
    throttle1=(int)(value1*0.308)+1000;
    throttle2=(int)(value2*0.302)+1000;
    if(throttle1>1970) throttle1=2000;
    if(throttle1<1030) throttle1=1000;
    if(throttle1<1600&&throttle1>1400) throttle1=1500;
    if(throttle2>1970)  throttle2=2000;
    if(throttle2<1030) throttle2=1000;
    if(throttle2<1600&&throttle2>1400) throttle2=1500;
    throttle1=(int)((throttle1-1000)/10);
    throttle2=(int)((throttle2-1000)/10);
}

void read_energy()
{
    rt_uint32_t energy_adc=rt_adc_read(adc_dev, ADC_IN3);
    int s3=0;
    // 3s 电池
    if (s3==1)
    {
        int temp=(int)((energy_adc/4096.0*3.3*8.5-0.4)*10)-111;  // 3.7*3*10=111
        int temp_dump_energy=0;
        if (temp<0)
        {
            temp_dump_energy = 0;
        }
        else if (temp>=17)
        {
            temp_dump_energy = 100;
        }
        else {
            temp_dump_energy = (int)(temp*100/17);
        }
        dump_energy_list[dump_energy_count]=temp_dump_energy;
        dump_energy_count++;
        if (dump_energy_count==5)
        {
            int sum_e=0;
            for (int i=0;i<dump_energy_count;i++)
            {
                sum_e+=dump_energy_list[i];
            }
            dump_energy=(int)(sum_e/5);
            rt_kprintf("dump_energy is :%d \n", dump_energy);
            dump_energy_count=0;
        }
    }
    // 2s 电池
    else {
        float_t temp=energy_adc/4096.0*3.3*8.5/2+0.3;  // 3.7*3*10=111
//        rt_kprintf("energy_adc is :%d \n", energy_adc);
//        rt_kprintf("temp is :%d \n", (int)(temp*100));
        float temp_dump_energy=0;
        if (temp<3)
        {
            temp_dump_energy = 0;
        }
        else if (temp>=3.8)
        {
            temp_dump_energy = 100;
        }
        else {
            temp_dump_energy = (temp-3.0)*125;
        }
//        rt_kprintf("temp_dump_energy is :%d \n", temp_dump_energy);
        dump_energy_list[dump_energy_count]=temp_dump_energy;
        dump_energy_count++;
        if (dump_energy_count==5)
        {
            float sum_e=0;
            for (int i=0;i<dump_energy_count;i++)
            {
                sum_e+=dump_energy_list[i];
            }
            dump_energy=(int)(sum_e/5.0);
            if (dump_energy>=100)
            {
                dump_energy=99;
            }
//            rt_kprintf("dump_energy is :%d \n", dump_energy);
            dump_energy_count=0;
        }
    }

}
int start_thread_adc(void)
{
    init_adc();
//    thread_adc = rt_thread_create("adc",
//                                adc_read_data,
//                                RT_NULL,
//                                1024,
//                                10,
//                                5);
//    if (thread_adc != RT_NULL)
//        rt_thread_startup(thread_adc);

//    thread_lora_send = rt_thread_create("lora_send",
//        lora_send, RT_NULL,
//                                1025,
//                                5, THREAD_TIMESLICE);
//    if (thread_lora_send != RT_NULL)
//        rt_thread_startup(thread_lora_send);

    return 0;
}
