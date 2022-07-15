/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-21     jing       the first version
 */
#include "manager.h"

static const int  draw_time = 10 ; //抽水时间
//全局变量
int throttle1=50;//  油门1AD采集值  右侧垂直通道
int throttle2=50;//  油门1AD采集值  右侧水平通道
int key1=0X0;   //按键 1
int key2=0X0;   //按键 2
int key3=0X0;   //按键 3
int key4=0X0;   //按键 4
int SA=0X00;     // 拨杆 A
int SB=0X00;     // 拨杆 B
int SC=0X00;     // 拨杆 C
int SD=0X00;     // 拨杆 D
int dump_energy=1;
float dump_energy_list[5]={};
int dump_energy_count=0;
// 接收上位机指令
/*
 * S1,2Z
 * P1500,1500Z
 * */
int step_motor[2]={3,3}; //步进电机运动指令 1 正转  2 反转  3 停止
int pwm_motor[2]={1500,1500}; //主侧电机运动指令
int line_step_motor=3;   //拉线步进电机 1 正转放线  2 反转收线  3 停止
int relay1=2;   //继电器数据  1开  2关
static rt_thread_t t_manager = RT_NULL;
static unsigned int joy_send_interval=30;   //摇杆发送时间间隔
static unsigned int key_send_interval=180;   //按键值发送时间间隔
static unsigned int energy_interval=100;   //电量检测时间间隔
unsigned int joy_tick=0;   //获取摇杆时间片  1ms一个时间片
unsigned int key_tick=0;   //获取按键时间片  1ms一个时间片
unsigned int energy_tick=0;   //获取按键时间片  1ms一个时间片
unsigned int current_tick=0;   //当前时间片
unsigned int ship_id=3;  // 本地存储船号
unsigned int set_ship_id=3;  // 设置船号
int b_set_ok=0;
unsigned char crc_high_first(unsigned char *ptr, unsigned char len)
{
    unsigned char i;
    unsigned char crc=0x00; /* 计算的初始crc值 */

    while(len--)
    {
        crc ^= *ptr++;  /* 每次先与需要计算的数据异或,计算完指向下一数据 */
        for (i=8; i>0; --i)   /* 下面这段计算过程与计算一个字节crc一样 */
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x07;
            else
                crc = (crc << 1);
        }
    }

    return (crc);
}
void manager(void *parameter)
{
    int start=1;
    while(1)
    {
        // 上电先设置到默认
        if (start==1)
        {
            rt_thread_mdelay(30);
            // 设置引脚为高电平
            lora_high();
            rt_thread_mdelay(200);
            send_lora_data(ship_id);
            // 获取模块参数配置
            rt_thread_mdelay(200);  //
            if (b_set_ok==1)
            {
                ship_id=set_ship_id;
                lora_low();
                rt_thread_mdelay(300);
                start -=1;
            }
        }
        if (ship_id==set_ship_id)
        {
            current_tick=rt_tick_get();
            // 1 判断时间是否需要发送摇杆数据，到时间就读取数据然后发送数据
            if ((current_tick-joy_tick)>joy_send_interval)
            {
                read_adc();   // 读取数据
                rt_kprintf("A%d,%dZ\n",throttle1,throttle2); // 通过Lora发送数据
                joy_tick=rt_tick_get();// 重新赋值时间
            }

            // 2 判断时间是否需要发送按键数据，到时间就读取数据然后发送数据
            if ((current_tick-key_tick)>key_send_interval)
            {
                // 读取按键数据
                read_key();
                //将是数字转为16进制
                int crc_chr1=0X00;
                int crc_chr2=0X00;
                crc_chr1 |=(key1<<3);
                crc_chr1|=(key2<<2);
                crc_chr1|=(key3<<1);
                crc_chr1|=key4;
                crc_chr2|=(SA<<6);
                crc_chr2|=(SB<<4);
                crc_chr2|=(SC<<2);
                crc_chr2|=SD;
                // 构成校验数据 添加校验 使用CRC-8 校验
                unsigned char res[]={crc_chr1,crc_chr2};
                unsigned char crc_result = crc_high_first(res,2);
                int result=0X000000;
                result|=(key1<<19);
                result|=(key2<<18);
                result|=(key3<<17);
                result|=(key4<<16);
                result|=(SA<<14);
                result|=(SB<<12);
                result|=(SC<<10);
                result|=(SD<<8);
                result|=crc_result;
                // 发送数据
    //            rt_kprintf("CRC:%XZ\n",crc_result);
    //            read_adc();
    //            rt_kprintf("中文Hello RT-Thread!H%05XZ\n",result);
    //            rt_kprintf("中文Hello RT-Thread!");
    //            printf("H%05XZ\n",result);  // 通过Lora发送按键数据
    //            rt_kprintf("H%d,%d,%d,%d,%d,%d,%d,%dZ\n",SA,SB,SC,SD,key1,key2,key3,key4);
                key_tick=rt_tick_get();// 重新赋值时间
            }
        }
        // 判断是否需要设置lora
        else
        {
//            rt_kprintf("ship_id:%d,set_ship_id:%d",ship_id,set_ship_id);
            rt_thread_mdelay(30);
            // 设置引脚为高电平
            lora_high();
            rt_thread_mdelay(300);
            send_lora_data(set_ship_id);
            // 获取模块参数配置
            rt_thread_mdelay(2000);  //
            if (b_set_ok==1)
            {
                ship_id=set_ship_id;
                lora_low();
                rt_thread_mdelay(300);
            }
        }

        // 读取电量
        if ((current_tick-energy_tick)>energy_interval)
        {
            read_energy();   // 读取剩余电量数据
            energy_tick=rt_tick_get();// 重新赋值时间
            //根据电量显示颜色  60 以上绿灯 30以上紫灯    30以下红灯
            if (dump_energy>=60)
            {
                control_gpio(3);
            }
            else if(dump_energy>=30)
            {
                control_gpio(2);
            }
            else {
                control_gpio(1);
            }
        }




        // 4 根据数据判断控制遥控器灯颜色和蜂鸣器
        //control_gpio(5);
//        rt_kprintf("pwm_motor[0] :%d,pwm_motor[1] :%d\n",pwm_motor[0],pwm_motor[1]);
//        rt_kprintf("line_step_motor:%d\n",line_step_motor);
        rt_thread_mdelay(3);  //让出线程给其他线程执行
    }
}



/* 线程 */
int thread_manager(void)
{
    /* 创建线程 */
    t_manager = rt_thread_create("thread_manager",
                                  manager,
                                  RT_NULL,
                                  2048,
                                  1,
                                  5);

    /* 如果获得线程控制块，启动这个线程 */
    if (t_manager != RT_NULL)
        rt_thread_startup(t_manager);
    return 0;
}
