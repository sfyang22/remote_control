/* Includes */
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "rtthread.h"
#include "rthw.h"
#include "board.h"
#include "stdio.h"

/* �궨�� */
#define PHOTO_SIZE					(1024 * 20)


/* �ⲿ�������� */
extern U8 button_flag;                //����ť���´���
extern U8 freeback_flag;              //�𶯷������ñ�־λ
extern U8 lcd_blk;                    //LCD��������
extern U8 led_value;                  //LEDǰ�յ�����
extern U8 led_open;                   //ǰ�յƿ���
extern U8 air_set;                    //����������������ֵ
extern U8 co1_set;                    //CO1��������ֵ
extern U8 control_power;              //ң�ص���
extern U8 car_power;                  //С������
extern U8 wifi_connected;             //WIFI����״̬
extern U16 air;						//���������ٷֱ�
extern U16 co1;						//COŨ�Ȱٷֱ�

extern float mcu_temp;					//ң����mcu�¶�
extern float car_temp;					//���ϴ������¶�
extern float car_humidity;				//����ʪ��
/* �ⲿ�������� */
extern WM_HWIN CreateWindow(void);

/* ����ṹ�� */
//�������ṹ��
//static TOUCH touch;

/* �����߳̿��ƿ� */
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

/* ���廥���� */
//static rt_mutex_t video_mux = RT_NULL;
//static rt_mutex_t lcd_blk_mux = RT_NULL;

/* �����߳� */
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

/* ����������ƿ� */
rt_mailbox_t buzz_mb = RT_NULL;

/* ������Ϣ���� */
static rt_mq_t menu_mq = RT_NULL;			//�˵�����ָ��
static rt_mq_t ctl_mq = RT_NULL;			//ң��������ָ��
static rt_mq_t video_mq = RT_NULL;			//��Ƶ��Ϣ
static rt_mq_t jpeg_len_mq = RT_NULL;			//ͼƬ��С��Ϣ

/* �����ڴ�ؿ��ƿ� */
//static rt_mp_t video_mp = RT_NULL;				//ͼ���ڴ�
/* ���������ڴ�ָ�� */
//static rt_uint8_t *video = RT_NULL;				//ͼ���ڴ�ָ��

//rt_uint8_t video[PHOTO_SIZE];



__ALIGNED(4)			//4�ֽڶ���

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


/***************** �̺߳��� ********************/

/* �����������߳� */
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

/* freeback�𶯷����߳� */
static void freeback_thread_entry(void *par)
{
	static rt_int16_t fb_P = 8;		//�𶯷�������
	rt_int8_t roll;
	rt_int8_t pitch;
	
    while(1)
    {
		if(mpu_dat_flag)			//�жϷ����Ƿ���
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

/* ����wifi�����߳� */
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
				if(!air_flag)		//����δ����
				{
					rt_mb_send(buzz_mb, 200);
					air_flag = 1;		//�Ѿ���������
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
				if(!co1_flag)		//����δ����
				{
					rt_mb_send(buzz_mb, 200);
					co1_flag = 1;		//�Ѿ���������
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
				if(!car_power_flag)		//����δ����
				{
					rt_mb_send(buzz_mb, 200);
					car_power_flag = 1;		//�Ѿ���������
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


/* LED������ָʾ���߳� */
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
/* LCDҺ������ʾ�߳� */
static void lcd_thread_entry(void *par)
{
    static rt_uint8_t delay1 = 0;
	static rt_uint8_t video[PHOTO_SIZE] = {0};
	rt_err_t error = RT_EOK;
	rt_uint16_t jpeg_len;
    rt_uint8_t led_info[2];			//LED�ƿ��أ� LED��ֵ����
    GUI_MEMDEV_Handle hMem = GUI_MEMDEV_Create(0, 20, 320, 240);
    //GUI_JPEG_INFO jpeg_info;
	
//	 GUI_JPEG_SetpfDrawEx(JPEG_X_Draw);

    CreateWindow();				//��������


    while (1)
    {
        if(delay1++ == 5)
        {
            delay1 = 0;
            GUI_Lock();
            led_info[0] = led_open;			//led����״̬
            led_info[1] = led_value;		//led ���Ȱٷֱ�
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
				GUI_JPEG_Draw(video, jpeg_len, 0, 20);			//��ʾjpegͼƬ
			}
			GUI_MEMDEV_Select(0);
			GUI_MEMDEV_CopyToLCDAt(hMem, 0, 0);
		}

        GUI_Delay(130);
    }


}

/* LCD�����������߳� */
static void touch_thread_entry(void *par)
{
    while(1)
    {
        GUI_TOUCH_Exec();
        rt_thread_delay(25);
    }
}

/* LCD��������߳� */
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

/* ����ͨ��ADC�ɼ��߳� */
static void ch_input_thread_entry(void *par)
{
	rt_uint8_t ctl_power_flag = 0;
	static rt_uint16_t delay = 0;
    static rt_uint16_t ch_value[5];			//ADC�ó���16bitֵ
    rt_int8_t ctl_cmd[4];				//ң�ؿ�������ٷֱ���ʽ��;{���1, ���2, ����ҡ��, ����ҡ��}
    CH_INPUT_StartSample(ch_value);		//����ADC DMA����
	rt_thread_delay(500);

    while(1)
    {
        ctl_cmd[0] = (ch_value[0] / 65535.0F) * 180 - 90;
		ctl_cmd[1] = (ch_value[1] / 65535.0F) * 180 - 90;
		ctl_cmd[2] = (ch_value[2] / 65535.0F) * 200 - 100;
		ctl_cmd[3] = (ch_value[3] / 65535.0F) * 100 - 50;
		
		delay++;
		if(delay == 100)			//��ʱ
		{
			delay = 0;
			GUI_Lock();
			control_power = (((double)ch_value[4] / 65535.0F) * 3.3 - 2.87) / (3.259241126 - 2.87) * 100;	//�������
			if(control_power < 20)
			{
				if(!ctl_power_flag)		//����δ����
				{
					rt_mb_send(buzz_mb, 500);
					ctl_power_flag = 1;		//�Ѿ���������
				}
			}
			else
			{
				ctl_power_flag = 0;
			}
			GUI_Unlock();
		}
		
        rt_mq_urgent(ctl_mq, (rt_uint8_t *)ctl_cmd, sizeof(ctl_cmd));			//����ʵʱ����ָ�WIFI�����߳�

        rt_thread_delay(40);
    }

}

//void M8266WIFI_Test(void);
/* MCU�¶ȼ���߳� */
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

/* WIFI�����߳� */
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
		
		WIFI_AUX_Waiting();			//�ȴ�WIFI����
        WIFI_SendData(send_dat, SEND_COUNT);
		
        rt_thread_delay(50);
    }

}

#define REC_PACK_SIZE		1460

/* MT8266ͼ���߳� */
static void MT8266_thread_entry(void *par)
{
    rt_uint16_t i = 0;		
	rt_uint16_t jpeg_size;
	rt_uint8_t temp;
    static rt_uint8_t jpeg_buf[PHOTO_SIZE] = {0};			//���ջ�����
	rt_uint8_t jpeg_dat_len[4] = {0};				//���ճ���
	rt_uint16_t jpeg_len = 0;						//jpeg��С

    M8266WIFI_Module_Init_Via_SPI();		//�ȵ����ӳ�ʼ��

    M8266WIFI_SPI_Setup_Connection(CONNECTION_TYPE, LOCAL_PORT, REMOTE_ADDR, REMOTE_PORT, LINK_NO, 3, RT_NULL);		//����TCP Server port��8000

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
					if(jpeg_len / REC_PACK_SIZE)               //�����ݴ���1460
					{
						M8266WIFI_SPI_RecvData(jpeg_buf+i*REC_PACK_SIZE, REC_PACK_SIZE, 5000, RT_NULL, RT_NULL);       //����һ����1460�ֽ�
						jpeg_len -= REC_PACK_SIZE;                 //��ȡʣ��δ�����ֽ���
						i++;                                        //������һ
					}
					else if(jpeg_len % REC_PACK_SIZE)
					{
						M8266WIFI_SPI_RecvData(jpeg_buf+i*REC_PACK_SIZE, jpeg_len, 5000, RT_NULL, RT_NULL);       //����һ����1460�ֽ�
						jpeg_len -= jpeg_len;                 //��ȡʣ��δ�����ֽ���
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
