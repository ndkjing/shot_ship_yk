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
#include <board.h>
#include  <stdbool.h>
#include "drv_common.h"
#include "manager.h"
#include "gpio_common.h"

void control_gpio(int j1)
{

   //
    switch(j1)
    {
    case 1:
        rt_pin_write(LED1, PIN_HIGH);
        rt_pin_write(LED2, PIN_LOW);
        rt_pin_write(LED3, PIN_LOW);
        break;
    case 2:
        rt_pin_write(LED1, PIN_LOW);
        rt_pin_write(LED2, PIN_HIGH);
        rt_pin_write(LED3, PIN_LOW);
        break;
    case 3:
        rt_pin_write(LED1, PIN_LOW);
        rt_pin_write(LED2, PIN_LOW);
        rt_pin_write(LED3, PIN_HIGH);
        break;
    case 4:
        rt_pin_write(LED1, PIN_LOW);
        rt_pin_write(LED2, PIN_LOW);
        rt_pin_write(LED3, PIN_LOW);
        break;
    case 5:
        rt_pin_write(BUZZER, PIN_HIGH);
        break;
    case 6:
        rt_pin_write(BUZZER, PIN_LOW);
        break;
    default:
        rt_pin_write(BUZZER, PIN_LOW);
        rt_pin_write(LED1, PIN_LOW);
        rt_pin_write(LED2, PIN_LOW);
        rt_pin_write(LED3, PIN_LOW);
    }

}

void read_key()
{
    // ����ȡ����ƽʱ���50ms�ٴμ�⿴�Ƿ���ͬ��ͬ�͸�ֵ����ͬ������
    if (rt_pin_read(KEY1)==PIN_LOW)
    {
        if (key1!=0X0)
        {
            rt_thread_mdelay(20);
            if (rt_pin_read(KEY1)==PIN_LOW)
            {
                key1=0X0;
            }
        }

    }
    else
    {
        if (key1!=0X1)
        {
            rt_thread_mdelay(20);
            if (rt_pin_read(KEY1)==PIN_HIGH)
            {
                key1=0X1;
            }
        }
    }
    if (rt_pin_read(KEY2)==PIN_LOW)
    {
        if (key2!=0X0)
        {
        rt_thread_mdelay(20);
        if (rt_pin_read(KEY2)==PIN_LOW)
        {
            key2=0X0;
        }
        }

    }
    else
    {
        if (key2!=0X1)
        {
        rt_thread_mdelay(20);
        if (rt_pin_read(KEY2)==PIN_HIGH)
        {
            key2=0X1;
        }
        }

    }
    if (rt_pin_read(KEY3)==PIN_LOW)
    {
        if (key3!=0X0)
        {
        rt_thread_mdelay(20);
        if (rt_pin_read(KEY3)==PIN_LOW)
        {
            key3=0X0;
        }
        }

    }
    else
    {
        if (key3!=0X1)
        {
        rt_thread_mdelay(20);
        if (rt_pin_read(KEY3)==PIN_HIGH)
        {
            key3=0X1;
        }
        }
    }
    if (rt_pin_read(KEY4)==PIN_LOW)
    {
        if (key4!=0X0)
        {
        rt_thread_mdelay(20);
       if (rt_pin_read(KEY4)==PIN_LOW)
       {
           key4=0X1;
       }
        }
    }
    else
    {
        if (key4!=0X1)
        {
            rt_thread_mdelay(20);
            if (rt_pin_read(KEY4)==PIN_HIGH)
            {
                key4=0X0;
            }
        }
    }
    if (rt_pin_read(SA1)==PIN_HIGH)
    {
        if (SA!=0X02)
        {
            rt_thread_mdelay(20);
            if (rt_pin_read(SA1)==PIN_HIGH)
            {
                SA=0X02;
            }
        }
    }
    else if(rt_pin_read(SA2)==PIN_HIGH)
    {
        if (SA!=0X01)
        {
            rt_thread_mdelay(20);
            if (rt_pin_read(SA2)==PIN_HIGH)
            {
                SA=0X01;
            }
        }
    }
    else {
        SA=0X00;
    }

    if (rt_pin_read(SB1)==PIN_HIGH)
    {
        if (SB!=0X02)
        {
            rt_thread_mdelay(20);
            if (rt_pin_read(SB1)==PIN_HIGH)
            {
                SB=0X02;
            }
        }

    }
    else if(rt_pin_read(SB2)==PIN_HIGH)
    {
        if (SB!=0X01)
        {
            rt_thread_mdelay(20);
            if (rt_pin_read(SB2)==PIN_HIGH)
            {
                SB=0X01;
            }
        }

    }
    else {
        SB=0X00;
    }

    if (rt_pin_read(SC1)==PIN_HIGH)
    {
        if (SC!=0X02)
        {
            rt_thread_mdelay(20);
            if (rt_pin_read(SC1)==PIN_HIGH)
            {
                SC=0X02;
            }
        }

    }
    else if(rt_pin_read(SC2)==PIN_HIGH)
    {
        if (SC!=0X01)
        {
            rt_thread_mdelay(20);
            if (rt_pin_read(SC2)==PIN_HIGH)
            {
                SC=0X01;
            }
        }

    }
    else {
        SC=0X00;
    }

    if (rt_pin_read(SD1)==PIN_HIGH)
    {
        if (SD!=0X02)
        {
            rt_thread_mdelay(20);
            if (rt_pin_read(SD1)==PIN_HIGH)
            {
                SD=0X02;
            }
        }

    }
    else if(rt_pin_read(SD2)==PIN_HIGH)
    {
        if (SD!=0X01)
        {
            rt_thread_mdelay(20);
            if (rt_pin_read(SD2)==PIN_HIGH)
            {
                SD=0X01;
            }
        }

    }
    else {
        SD=0X00;
    }
}

static void hdr_callback4()
{
//    rt_pin_irq_enable(KEY4, PIN_IRQ_DISABLE);
    rt_thread_mdelay(200);
//    rt_kprintf("hdr_callback23435\n");
    if (rt_pin_read(KEY4)==PIN_LOW)
    {
        key4=0;
    }
    else
    {
        key4=1;
    }
//    rt_pin_irq_enable(KEY4, PIN_IRQ_ENABLE);
}

static void hdr_callback1()
{
//    rt_pin_irq_enable(KEY1, PIN_IRQ_DISABLE);
    rt_thread_mdelay(200);
//    rt_kprintf("hdr_callback4545\n");
    if (rt_pin_read(KEY1)!=PIN_LOW)
    {
        key1=1;
    }
    else
    {
        key1=0;
    }
//    rt_pin_irq_enable(KEY1, PIN_IRQ_ENABLE);
}

void init_gpio(void)
{
    rt_pin_mode(MD0, PIN_MODE_OUTPUT);
    rt_pin_mode(MD1, PIN_MODE_OUTPUT);
    rt_pin_mode(BUZZER, PIN_MODE_OUTPUT);
    rt_pin_mode(LED1, PIN_MODE_OUTPUT);
    rt_pin_mode(LED2, PIN_MODE_OUTPUT);
    rt_pin_mode(LED3, PIN_MODE_OUTPUT);
    rt_pin_mode(KEY1, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(KEY2, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(KEY3, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(KEY4, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(SA1, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(SA2, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(SB1, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(SB2, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(SC1, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(SC2, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(SD1, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(SD2, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_write(MD0, PIN_LOW);
    rt_pin_write(MD1, PIN_LOW);
    rt_pin_write(BUZZER, PIN_LOW);
    rt_pin_write(LED1, PIN_LOW);
    rt_pin_write(LED2, PIN_LOW);
    rt_pin_write(LED3, PIN_LOW);

    rt_err_t key1_status = rt_pin_attach_irq(KEY1, PIN_IRQ_MODE_RISING_FALLING, hdr_callback1,NULL);
    rt_err_t key4_status = rt_pin_attach_irq(KEY4, PIN_IRQ_MODE_RISING_FALLING, hdr_callback4,NULL);
    if (key1_status != RT_EOK)
    {
        rt_kprintf("key1_status %d failed!\n", key1_status);
    }
    if (key4_status != RT_EOK)
    {
        rt_kprintf("key4_status %d failed!\n", key4_status);
    }
//    rt_pin_irq_enable(KEY1, PIN_IRQ_ENABLE);
//    rt_pin_irq_enable(KEY4, PIN_IRQ_ENABLE);
}

void lora_high()
{
    rt_pin_write(MD0, PIN_HIGH);
    rt_pin_write(MD1, PIN_HIGH);
}

void lora_low()
{
    rt_pin_write(MD0, PIN_LOW);
    rt_pin_write(MD1, PIN_LOW);
}


