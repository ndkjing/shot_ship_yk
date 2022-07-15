/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-01     jing       the first version
 */
#include <uart_screen.h>
#include <manager.h>

static rt_thread_t thread_screen = RT_NULL;
static rt_thread_t thread_screen_receive = RT_NULL;
#define SCREEN_UART_NAME       "uart3"    /* 串口设备名称 */
static rt_device_t screen_serial;                /* 串口设备句柄 */
struct serial_configure screen_config = RT_SERIAL_CONFIG_DEFAULT;  /* 初始化配置参数 */

struct rt_semaphore screen_sem;  // 信号量


rt_err_t rx_callback_screen(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&screen_sem);
    return RT_EOK;
}

void init_screen()
{
    /* step1：查找串口设备 */
    screen_serial = rt_device_find(SCREEN_UART_NAME);
    /* step2：修改串口配置参数 */
    screen_config.baud_rate = BAUD_RATE_115200;        //修改波特率为 9600
    screen_config.data_bits = DATA_BITS_8;           //数据位 8
    screen_config.stop_bits = STOP_BITS_1;           //停止位 1
    screen_config.bufsz     = 128;                   //修改缓冲区 buff size 为 128
    screen_config.parity    = PARITY_NONE;           //无奇偶校验位
    /* step3：控制串口设备。通过控制接口传入命令控制字，与控制参数 */
    rt_device_control(screen_serial, RT_DEVICE_CTRL_CONFIG, &screen_config);
    /* step4：打开串口设备。以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(screen_serial, RT_DEVICE_FLAG_INT_RX);
    /* 初始化信号量 */
    rt_sem_init(&screen_sem, "screen_sem", 0, RT_IPC_FLAG_FIFO);
    rt_device_set_rx_indicate(screen_serial, rx_callback_screen);
}
int is_need_data=1;

void send_hex_end()
{
//    rt_thread_mdelay(5);  // 定时检测
    rt_uint8_t test[3] = {0xFF,0xFF,0xFF};
    rt_device_write(screen_serial, 0, test, 3);
//    rt_uint8_t test1 = 0xFF;
//    rt_device_write(screen_serial, 0, &test1, 1);
//    rt_device_write(screen_serial, 0, &test1, 1);
//    rt_device_write(screen_serial, 0, &test1, 1);
}

void screen_receive_data(void *parameter)
{
    int s_int;
    int screen_index = 0;  // 罗盘数据
    int screen_buffer[4] = {0};
    while (1)
    {
        while(rt_device_read(screen_serial, 0, &s_int, 1) != 1)//等待接收完
        {
           rt_sem_take(&screen_sem, RT_WAITING_FOREVER);//释放信息量
        }
//        rt_kprintf("screen  :%d\n",s_int);
        if (s_int>=1 && s_int<=20)
        {
            screen_buffer[screen_index]=s_int;
            screen_index+=1;
        }
        else if (screen_index!=0)
        {
            screen_buffer[screen_index]=s_int;
            screen_index+=1;
        }
        if (s_int==0 && screen_index==4)
        {
            screen_index =0;
            set_ship_id = (int)screen_buffer[0];
//          rt_kprintf("target_ship_id:%d\n",screen_buffer[0]);
         }
        if (screen_index>=4)
        {
            screen_index =0;
        }
    }
}


/* 线程 的入口函数 */
static void screen_send_data(void *parameter)
{
    int start_set=1;
    while (1)
    {
         // 发送摇杆值
//        char str1[] ="t0.txt=\"30\"";
        char str1[15];
        sprintf(str1,"t0.txt=\"%d\"",throttle1);
        rt_device_write(screen_serial, 0, str1, strlen(str1));  //向指定串口发送数据
//        rt_kprintf("len:%d %d ",sizeof(str1),strlen(str1)); // 通过Lora发送数据
        send_hex_end();
        char str2[15];
        sprintf(str2,"t1.txt=\"%d\"",throttle2);
        rt_device_write(screen_serial, 0, str2, strlen(str2));  //向指定串口发送数据
        send_hex_end();
//        // 发送电量
        char str3[15];
        sprintf(str3,"j0.val=%d",dump_energy);
        rt_device_write(screen_serial, 0, str3, strlen(str3));  //向指定串口发送数据
        send_hex_end();
//        // 发送船号
//        当接收到船号不等于本地存储船号时发送
        if (start_set==1)
        {
            char str4[15];
            sprintf(str4,"h0.val=%d",ship_id);
            rt_device_write(screen_serial, 0, str4, strlen(str4));  //向指定串口发送数据
            send_hex_end();
            char str5[15];
            sprintf(str5,"n1.val=%d",ship_id);
            rt_device_write(screen_serial, 0, str5, strlen(str5));  //向指定串口发送数据
            send_hex_end();
            start_set=0;
        }
        rt_thread_mdelay(50);  // 定时检测

    }
}


/* 线程 */
int start_thread_screen(void)
{
    init_screen();
    /* 创建线程 */
    thread_screen = rt_thread_create("screen_send",
            screen_send_data, RT_NULL,
                                1025,
                                20, 10);
    /* 如果获得线程控制块，启动这个线程 */
    if (thread_screen != RT_NULL)
        rt_thread_startup(thread_screen);
    /* 创建线程 */
    thread_screen_receive = rt_thread_create("screen_receive",
            screen_receive_data, RT_NULL,
                                1025,
                                20, 10);
    /* 如果获得线程控制块，启动这个线程 */
    if (thread_screen_receive != RT_NULL)
        rt_thread_startup(thread_screen_receive);
    return 0;
}
