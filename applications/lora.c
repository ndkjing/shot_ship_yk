/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-21     jing       the first version
 */
#include <lora.h>
#include <manager.h>


#define THREAD_PRIORITY         10
#define THREAD_STACK_SIZE       2048
#define THREAD_TIMESLICE        5

static rt_thread_t thread_lora_rec1 = RT_NULL;
//static rt_thread_t thread_lora_send = RT_NULL;

#define LORA_UART_NAME       "uart1"
static rt_device_t lora_serial1;
struct serial_configure lora_config1 = RT_SERIAL_CONFIG_DEFAULT;

struct rt_semaphore lora_sem1;


rt_err_t rx_callback1(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&lora_sem1);
    return RT_EOK;
}

void init_lora()
{
    lora_serial1 = rt_device_find(LORA_UART_NAME);
    lora_config1.baud_rate = BAUD_RATE_9600;
    lora_config1.data_bits = DATA_BITS_8;
    lora_config1.stop_bits = STOP_BITS_1;
    lora_config1.bufsz     = 128;
    lora_config1.parity    = PARITY_NONE;
    rt_device_control(lora_serial1, RT_DEVICE_CTRL_CONFIG, &lora_config1);
    rt_device_open(lora_serial1, RT_DEVICE_FLAG_INT_RX);
    rt_sem_init(&lora_sem1, "lora_sem", 0, RT_IPC_FLAG_FIFO);
    rt_device_set_rx_indicate(lora_serial1, rx_callback1);
}

void send_lora_data(int chan)
{
    rt_uint8_t test[6] = {0xC0,0x00,0x24,0x1C,0X00,0x40};
    test[4]+=chan;
    rt_device_write(lora_serial1, 0, test, 6);
}

static void lora_rec1(void *parameter)
{
    char lora_char;
    char lora_buffer[10]={0};
    int lora_buffer_index=0;
    char step1_s[3]={};
    while (1)
    {
        while(rt_device_read(lora_serial1,0, &lora_char, 1) != 1)
        {
           rt_sem_take(&lora_sem1, RT_WAITING_FOREVER);
        }
        if(lora_char=='O')
        {
            lora_buffer[lora_buffer_index]=lora_char;
            lora_buffer_index+=1;
            b_set_ok=1;
        }
        else if (lora_buffer_index !=0) {
            lora_buffer[lora_buffer_index]=lora_char;
            lora_buffer_index+=1;
            lora_buffer_index=0;
        }
        if (lora_buffer_index>=10)
        {
            lora_buffer_index=0;
        }
    }
}


int thread_lora(void)
{
    init_lora();
    thread_lora_rec1 = rt_thread_create("lora_rec",
            lora_rec1, RT_NULL,
                            THREAD_STACK_SIZE,
                            1, THREAD_TIMESLICE);
    if (thread_lora_rec1 != RT_NULL)
        rt_thread_startup(thread_lora_rec1);

//    thread_lora_send = rt_thread_create("lora_send",
//        lora_send, RT_NULL,
//                                1025,
//                                5, THREAD_TIMESLICE);
//    if (thread_lora_send != RT_NULL)
//        rt_thread_startup(thread_lora_send);

    return 0;
}
