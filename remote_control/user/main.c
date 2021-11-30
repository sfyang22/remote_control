/* Includes */
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "rtthread.h"
#include "rthw.h"
#include "board.h"
#include "stdio.h"

/* 宏定义 */
#define PHOTO_SIZE					(1024 * 20)


/* 外部变量声明 */
extern U8 button_flag;                //代表按钮按下次数
extern U8 freeback_flag;              //震动反馈设置标志位
extern U8 lcd_blk;                    //LCD背光亮度
extern U8 led_value;                  //LED前照灯亮度
extern U8 led_open;                   //前照灯开关
extern U8 air_set;                    //空气质量传感器阈值
extern U8 co1_set;                    //CO1传感器阈值
extern U8 control_power;              //遥控电量
extern U8 car_power;                  //小车电量
extern U8 wifi_connected;             //WIFI连接状态
extern U16 air;						//空气质量百分比
extern U16 co1;						//CO浓度百分比

extern float mcu_temp;					//遥控器mcu温度
extern float car_temp;					//车上传感器温度
extern float car_humidity;				//车上湿度
/* 外部函数声明 */
extern WM_HWIN CreateWindow(void);

/* 定义结构体 */
//触摸屏结构体
//static TOUCH touch;

/* 定义线程控制块 */
static rt_thread_t led_thread = RT_NULL;
static rt_thread_t lcd_thread = RT_NULL;
static rt_thread_t touch_thread = RT_NULL;
static rt_thread_t lcd_blk_thread = RT_NULL;
static rt_thread_t ch_input_thread = RT_NULL;
static rt_thread_t mcu_temp_thread = RT_NULL;
static rt_thread_t wifi_thread = RT_NULL;
static rt_thread_t MT8266_thread = RT_NULL;
static rt_thread_t wifi_rec_thread = RT_NULL;
static rt_thread_t freeback_thread = RT_NULL;
static rt_thread_t buzz_thread = RT_NULL;

/* 定义互斥量 */
//static rt_mutex_t video_mux = RT_NULL;
//static rt_mutex_t lcd_blk_mux = RT_NULL;

/* 定义线程 */
static void led_thread_entry(void *par);
static void lcd_thread_entry(void *par);
static void touch_thread_entry(void *par);
static void lcd_blk_thread_entry(void *par);
static void ch_input_thread_entry(void *par);
static void mcu_temp_thread_entry(void *par);
static void wifi_thread_entry(void *par);
static void MT8266_thread_entry(void *par);
static void wifi_rec_thread_entry(void *par);
static void freeback_thread_entry(void *par);
static void buzz_thread_entry(void *par);

/* 定义邮箱控制块 */
rt_mailbox_t buzz_mb = RT_NULL;

/* 定义消息队列 */
static rt_mq_t menu_mq = RT_NULL;			//菜单控制指令
static rt_mq_t ctl_mq = RT_NULL;			//遥控器控制指令
static rt_mq_t video_mq = RT_NULL;			//视频消息
static rt_mq_t jpeg_len_mq = RT_NULL;			//图片大小消息

/* 定义内存池控制块 */
//static rt_mp_t video_mp = RT_NULL;				//图像内存
/* 定义申请内存指针 */
//static rt_uint8_t *video = RT_NULL;				//图像内存指针

//rt_uint8_t video[PHOTO_SIZE];



__ALIGNED(4)			//4字节对齐

int main()
{
	/**************** buzz_thread ***********************/
    buzz_thread = rt_thread_create("buzz_thread",
                                  buzz_thread_entry,
                                  RT_NULL,
                                  512,
                                  29,
                                  5);

    if(RT_NULL != buzz_thread)
        rt_thread_startup(buzz_thread);
    else
        return -1;
	
	/**************** freeback_thread ***********************/
    freeback_thread = rt_thread_create("freeback_thread",
                                  freeback_thread_entry,
                                  RT_NULL,
                                  512,
                                  23,
                                  10);

    if(RT_NULL != freeback_thread)
        rt_thread_startup(freeback_thread);
    else
        return -1;
	/**************** wifi_rec_thread ***********************/
    wifi_rec_thread = rt_thread_create("wifi_rec_thread",
                                  wifi_rec_thread_entry,
                                  RT_NULL,
                                  512,
                                  26,
                                  10);

    if(RT_NULL != wifi_rec_thread)
        rt_thread_startup(wifi_rec_thread);
    else
        return -1;

    /**************** led thread ***********************/
    led_thread = rt_thread_create("led",
                                  led_thread_entry,
                                  RT_NULL,
                                  128,
                                  27,
                                  10);

    if(RT_NULL != led_thread)
        rt_thread_startup(led_thread);
    else
        return -1;

    #if 1
    /**************** lcd thread ***********************/

    lcd_thread = rt_thread_create("lcd",
                                  lcd_thread_entry,
                                  RT_NULL,
                                  PHOTO_SIZE + 1024*5,
                                  20,
                                  10);

    if(RT_NULL != lcd_thread)
        rt_thread_startup(lcd_thread);
    else
        return -1;

    /**************** lcd_touch thread ***********************/
    touch_thread = rt_thread_create("touch",
                                    touch_thread_entry,
                                    RT_NULL,
                                    1024,
                                    25,
                                    5);

    if(RT_NULL != touch_thread)
        rt_thread_startup(touch_thread);
    else
        return -1;

    /**************** lcd_blk thread ***********************/
    lcd_blk_thread = rt_thread_create("lcd_blk",
                                      lcd_blk_thread_entry,
                                      RT_NULL,
                                      128,
                                      29,
                                      5);

    if(RT_NULL != lcd_blk_thread)
        rt_thread_startup(lcd_blk_thread);
    else
        return -1;

    /**************** ch_input thread ***********************/
    ch_input_thread = rt_thread_create("ch_input",
                                       ch_input_thread_entry,
                                       RT_NULL,
                                       1024,
                                       22,
                                       5);

    if(RT_NULL != ch_input_thread)
        rt_thread_startup(ch_input_thread);
    else
        return -1;

    /**************** mcu_temp thread ***********************/
    mcu_temp_thread = rt_thread_create("mcu_temp",
                                       mcu_temp_thread_entry,
                                       RT_NULL,
                                       1024,
                                       28,
                                       5);

    if(RT_NULL != mcu_temp_thread)
        rt_thread_startup(mcu_temp_thread);
    else
        return -1;



    /**************** wifi thread ***********************/
    wifi_thread = rt_thread_create("wifi",
                                   wifi_thread_entry,
                                   RT_NULL,
                                   1024*5,
                                   21,
                                   5);

    if(RT_NULL != wifi_thread)
        rt_thread_startup(wifi_thread);
    else
        return -1;

    /**************** MT8266 thread ***********************/
    MT8266_thread = rt_thread_create("MT8266",
                                     MT8266_thread_entry,
                                     RT_NULL,
                                     PHOTO_SIZE + 10*1024,
                                     30,
                                     50);

    if(RT_NULL != MT8266_thread)
        rt_thread_startup(MT8266_thread);
    else
        return -1;

    #endif

	/**************** menu_mq ***********************/
    buzz_mb = rt_mb_create("buzz_mb", 
							2, 
							RT_IPC_FLAG_FIFO);

    if(RT_NULL == buzz_mb)
        return -1;

    /**************** menu_mq ***********************/
    menu_mq = rt_mq_create("menu_mq",
                           2,
                           3,
                           RT_IPC_FLAG_FIFO);

    if(RT_NULL == menu_mq)
        return -1;

    /**************** ctl_mq ***********************/
    ctl_mq = rt_mq_create("ctl_mq",
                          4,
                          1,
                          RT_IPC_FLAG_FIFO);

    if(RT_NULL == ctl_mq)
        return -1;
	
	/**************** video_mq ***********************/
    video_mq = rt_mq_create("video_mq",
                          PHOTO_SIZE,
                          3,
                          RT_IPC_FLAG_FIFO);

    if(RT_NULL == video_mq)
	{
		rt_kprintf("video_mq fail!\r\n");
		return -1;
	}
	
	/**************** jpeg_len_mq ***********************/
    jpeg_len_mq = rt_mq_create("jpeg_len_mq",
                          2,
                          3,
                          RT_IPC_FLAG_FIFO);

    if(RT_NULL == jpeg_len_mq)
	{
		rt_kprintf("jpeg_len_mq fail!\r\n");
		return -1;
	}

}


/***************** 线程函数 ********************/

/* 蜂鸣器报警线程 */
static void buzz_thread_entry(void *par)
{
	rt_uint16_t buzz_delay;
	
	while(1)
	{
		rt_mb_recv(buzz_mb, (rt_uint32_t *)&buzz_delay, RT_WAITING_FOREVER);
		
		BUZZ_OPEN();
//		rt_kprintf("%d\r\n", buzz_delay);
		rt_thread_delay(buzz_delay);
		BUZZ_CLOSE();
		rt_thread_delay(3000);
	}
}

/* freeback震动反馈线程 */
static void freeback_thread_entry(void *par)
{
	static rt_int16_t fb_P = 8;		//震动反馈比例
	rt_int8_t roll;
	rt_int8_t pitch;
	
    while(1)
    {
		if(mpu_dat_flag)			//判断反馈是否开启
		{
			mpu_dat_flag = 0;
			GUI_Lock();
			if(!freeback_flag)
			{
				GUI_Unlock();
				pitch = (rt_int8_t)mpu_dat[0];
				roll = (rt_int8_t)mpu_dat[1];
				FREEBACK_Contorl(pitch, roll, fb_P);
				rt_thread_delay(80);
				FREEBACK_Contorl(0, 0, 0);
			}
			else
			{
				GUI_Unlock();
				rt_thread_delay(50);
			}
		}
		else
		{
			
			rt_thread_delay(30);
		}
    }
}

/* 数传wifi接收线程 */
static void wifi_rec_thread_entry(void *par)
{
	rt_uint8_t i;
	rt_uint8_t air_flag = 0, co1_flag = 0, car_power_flag = 0;
	
    while(1)
    {
		if(rec_dat_flag)
		{
			rec_dat_flag = 0;
			GUI_Lock();
			air = (rec_dat[0] << 8) | rec_dat[1];
			co1 = (rec_dat[2] << 8) | rec_dat[3];
			car_power = rec_dat[4];
			car_humidity = (rec_dat[5] << 8) | rec_dat[6];
			car_temp = (rec_dat[7] << 8) | rec_dat[8];	
			GUI_Unlock();
		}
		else
		{
			GUI_Lock();
			if(air >= (air_set * 10))
			{
				GUI_Unlock();
				if(!air_flag)		//若还未报警
				{
					rt_mb_send(buzz_mb, 200);
					air_flag = 1;		//已经报警过了
				}
			}
			else
			{
				GUI_Unlock();
				air_flag = 0;
			}
			
			GUI_Lock();
			if(co1 >= (co1_set * 10))
			{
				GUI_Unlock();
				if(!co1_flag)		//若还未报警
				{
					rt_mb_send(buzz_mb, 200);
					co1_flag = 1;		//已经报警过了
				}
			}
			else
			{
				GUI_Unlock();
				co1_flag = 0;
			}
			
			GUI_Lock();
			if(car_power < 20)
			{
				GUI_Unlock();
				if(!car_power_flag)		//若还未报警
				{
					rt_mb_send(buzz_mb, 200);
					car_power_flag = 1;		//已经报警过了
				}
			}
			else
			{
				GUI_Unlock();
				car_power_flag = 0;
			}
			
			
			rt_thread_delay(500);
		}
		
    }
}


/* LED调试用指示灯线程 */
static void led_thread_entry(void *par)
{
    while(1)
    {
        LED_HIGH;
        rt_thread_delay(500);
        LED_LOW;
        rt_thread_delay(500);
    }
}
/* LCD液晶屏显示线程 */
static void lcd_thread_entry(void *par)
{
    static rt_uint8_t delay1 = 0;
	static rt_uint8_t video[PHOTO_SIZE] = {0};
	rt_err_t error = RT_EOK;
	rt_uint16_t jpeg_len;
    rt_uint8_t led_info[2];			//LED灯开关， LED灯值命令
    GUI_MEMDEV_Handle hMem = GUI_MEMDEV_Create(0, 20, 320, 240);
    //GUI_JPEG_INFO jpeg_info;
	
//	 GUI_JPEG_SetpfDrawEx(JPEG_X_Draw);

    CreateWindow();				//创建窗口


    while (1)
    {
        if(delay1++ == 5)
        {
            delay1 = 0;
            GUI_Lock();
            led_info[0] = led_open;			//led开关状态
            led_info[1] = led_value;		//led 亮度百分比
            rt_mq_send(menu_mq, led_info, sizeof(led_info));
            GUI_Unlock();
        }
    
		rt_mq_recv(jpeg_len_mq, &jpeg_len, 2, RT_WAITING_NO);	
		error = rt_mq_recv(video_mq, video, jpeg_len, RT_WAITING_NO);
		
		if(error == RT_EOK)
		{
			//rt_kprintf("size = %d\r\n", jpeg_len);
			GUI_Lock();
			if(!button_flag)
			{
				GUI_Unlock();
				GUI_MEMDEV_Select(hMem);
				GUI_JPEG_Draw(video, jpeg_len, 0, 20);			//显示jpeg图片
			}
			GUI_MEMDEV_Select(0);
			GUI_MEMDEV_CopyToLCDAt(hMem, 0, 0);
		}

        GUI_Delay(130);
    }


}

/* LCD触摸屏控制线程 */
static void touch_thread_entry(void *par)
{
    while(1)
    {
        GUI_TOUCH_Exec();
        rt_thread_delay(25);
    }
}

/* LCD背光调节线程 */
static void lcd_blk_thread_entry(void *par)
{
    while(1)
    {
        GUI_Lock();
        LCD_BLK_Adjust(lcd_blk);
        GUI_Unlock();
        rt_thread_delay(299);
    }

}

/* 输入通道ADC采集线程 */
static void ch_input_thread_entry(void *par)
{
	rt_uint8_t ctl_power_flag = 0;
	static rt_uint16_t delay = 0;
    static rt_uint16_t ch_value[5];			//ADC得出的16bit值
    rt_int8_t ctl_cmd[4];				//遥控控制命令（百分比形式）;{舵机1, 舵机2, 上下摇杆, 左右摇杆}
    CH_INPUT_StartSample(ch_value);		//开启ADC DMA采样
	rt_thread_delay(500);

    while(1)
    {
        ctl_cmd[0] = (ch_value[0] / 65535.0F) * 180 - 90;
		ctl_cmd[1] = (ch_value[1] / 65535.0F) * 180 - 90;
		ctl_cmd[2] = (ch_value[2] / 65535.0F) * 200 - 100;
		ctl_cmd[3] = (ch_value[3] / 65535.0F) * 100 - 50;
		
		delay++;
		if(delay == 100)			//延时
		{
			delay = 0;
			GUI_Lock();
			control_power = (((double)ch_value[4] / 65535.0F) * 3.3 - 2.87) / (3.259241126 - 2.87) * 100;	//计算电量
			if(control_power < 20)
			{
				if(!ctl_power_flag)		//若还未报警
				{
					rt_mb_send(buzz_mb, 500);
					ctl_power_flag = 1;		//已经报警过了
				}
			}
			else
			{
				ctl_power_flag = 0;
			}
			GUI_Unlock();
		}
		
        rt_mq_urgent(ctl_mq, (rt_uint8_t *)ctl_cmd, sizeof(ctl_cmd));			//发送实时控制指令到WIFI发送线程

        rt_thread_delay(40);
    }

}

//void M8266WIFI_Test(void);
/* MCU温度监控线程 */
static void mcu_temp_thread_entry(void *par)
{
    while(1)
    {
        GUI_Lock();
        mcu_temp = MCU_Tempreture();
        GUI_Unlock();
        rt_thread_delay(2000);
    }

}

#define LED_INFO_COUTN			2
#define CH_CTL_COUNT			4
#define SEND_COUNT				(LED_INFO_COUTN + CH_CTL_COUNT)

/* WIFI数传线程 */
static void wifi_thread_entry(void *par)
{
	rt_uint8_t i;
    rt_uint8_t led_info[LED_INFO_COUTN];
    rt_uint8_t ch_ctl[CH_CTL_COUNT];
	rt_uint8_t send_dat[SEND_COUNT];
	
    while(1)
    {
        rt_mq_recv(ctl_mq, ch_ctl, sizeof(ch_ctl), RT_WAITING_NO);
        rt_mq_recv(menu_mq, led_info, sizeof(led_info), RT_WAITING_NO);
		
		for(i=0;i<CH_CTL_COUNT;i++)
		{
			send_dat[i] = ch_ctl[i];
		}		
		
		for(;i<SEND_COUNT;i++)
		{
			send_dat[i] = led_info[i-CH_CTL_COUNT];
		}	
		
		WIFI_AUX_Waiting();			//等待WIFI空闲
        WIFI_SendData(send_dat, SEND_COUNT);
		
        rt_thread_delay(50);
    }

}

#define REC_PACK_SIZE		1460

/* MT8266图传线程 */
static void MT8266_thread_entry(void *par)
{
    rt_uint16_t i = 0;		
	rt_uint16_t jpeg_size;
	rt_uint8_t temp;
    static rt_uint8_t jpeg_buf[PHOTO_SIZE] = {0};			//接收缓存区
	rt_uint8_t jpeg_dat_len[4] = {0};				//接收长度
	rt_uint16_t jpeg_len = 0;						//jpeg大小

    M8266WIFI_Module_Init_Via_SPI();		//热点连接初始化

    M8266WIFI_SPI_Setup_Connection(CONNECTION_TYPE, LOCAL_PORT, REMOTE_ADDR, REMOTE_PORT, LINK_NO, 3, RT_NULL);		//建立TCP Server port：8000

	while(1)
	{
		if(M8266WIFI_SPI_Has_DataReceived())
		{
			M8266WIFI_SPI_RecvData(jpeg_dat_len, 4, 500, RT_NULL, RT_NULL);
			if(jpeg_dat_len[0] == 0xFF && jpeg_dat_len[1] == 0xFF)
			{
				jpeg_len = jpeg_dat_len[2] << 8;
				jpeg_len |= jpeg_dat_len[3];
				
				//jpeg_len += 4;
				
//				while(!M8266WIFI_SPI_Has_DataReceived());
				
				for(i=0, jpeg_size = jpeg_len;;)
				{
					if(jpeg_len / REC_PACK_SIZE)               //若数据大于1460
					{
						M8266WIFI_SPI_RecvData(jpeg_buf+i*REC_PACK_SIZE, REC_PACK_SIZE, 5000, RT_NULL, RT_NULL);       //接收一个包1460字节
						jpeg_len -= REC_PACK_SIZE;                 //获取剩下未发送字节数
						i++;                                        //计数加一
					}
					else if(jpeg_len % REC_PACK_SIZE)
					{
						M8266WIFI_SPI_RecvData(jpeg_buf+i*REC_PACK_SIZE, jpeg_len, 5000, RT_NULL, RT_NULL);       //接收一个包1460字节
						jpeg_len -= jpeg_len;                 //获取剩下未发送字节数
					}
					else
					{
						rt_mq_send(jpeg_len_mq, &jpeg_size, 2);
						rt_mq_send(video_mq, jpeg_buf, jpeg_size);
						temp = 0xAB;
						M8266WIFI_SPI_Send_Data(&temp, 1, LINK_NO, RT_NULL);
						//M8266WIFI_SPI_Send_BlockData(jpeg_buf, jpeg_size, 1000, LINK_NO, RT_NULL, RT_NULL, RT_NULL);
						break;
					}
				}
			}
		}
		
		
	}
}

    /***************** END ********************/
