#include "wifi_uart.h"
#include <rtthread.h>
#include <stdio.h>

uint8_t rec_dat[REC_COUNT];
uint8_t rec_dat_flag = 0;

uint8_t mpu_dat[REC_COUNT];
uint8_t mpu_dat_flag = 0;

static void AUX_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStrutre;
	
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	GPIO_InitStrutre.Pin = GPIO_PIN_0;
	GPIO_InitStrutre.Mode = GPIO_MODE_INPUT;
	GPIO_InitStrutre.Pull = GPIO_PULLUP;
	GPIO_InitStrutre.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOE, &GPIO_InitStrutre);
	
}

//����WIFI��ʼ��
void WIFI_UART_Init(void)
{
	AUX_GPIO_Config();
	MX_UART4_Init();
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);
	__HAL_UART_CLEAR_IT(&huart4, UART_IT_RXNE);	
}

void WIFI_AUX_Waiting(void)
{
	while(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0) == RESET)		//�ȴ�����WIIF����
	{
		rt_thread_delay(3);
	}
}


//����WIFI��������
//���룺 *dat ��Ҫ������������ len ���ݳ���
//���أ� ��
void WIFI_SendData(uint8_t *dat, uint16_t len)
{
	uint16_t i;
	uint8_t send_dat[len + 2];		
	
	send_dat[0] = 0xAA;		//0xAA ��ʼλ
	
	for(i=0;i<len;i++)		//ƴ������
	{
		send_dat[i+1] = *dat++;
	}
	
	send_dat[len+1] = 0xBB;	//��ӽ���λ
	
	HAL_UART_Transmit(&huart4, send_dat, len+2, 100);		//ͨ�����ڷ���
}


//����4�жϷ����ӳ���
void UART4_IRQHandler(void)
{
    static uint8_t rec_co = 0;
    static uint8_t rec[REC_COUNT+2];

    rt_interrupt_enter();       //�����жϱ���

    if(__HAL_UART_GET_IT(&huart4, UART_IT_RXNE) != RESET)
    {
        __HAL_UART_CLEAR_IT(&huart4, UART_IT_RXNE);	
        rec[rec_co] = huart4.Instance->RDR;

        if(rec[0] == 0xAA)      //�����յ���ʼλ0xAA
        {
            rec_co++;
        }

		if(rec_co == MPU_COUNT + 2)
		{
			if(rec[MPU_COUNT+1] == 0xCC)
			{
				rec_co = 0;
				rt_memcpy(mpu_dat, rec+1, MPU_COUNT);        //�������洢�����û�ȡ��		
                mpu_dat_flag = 1;                     //�����յ���־λ
			}				
		}
        else if(rec_co == (REC_COUNT+2))         //������7���ֽ�
        {
            rec_co = 0;
            if(rec[REC_COUNT+1] == 0xBB)      //�����յ�����λ����������ȷ
            {
                rt_memcpy(rec_dat, rec+1, REC_COUNT);        //�������洢�����û�ȡ��		
                rec_dat_flag = 1;                     //�����յ���־λ
            }
        }

    }

    rt_interrupt_leave();       //�˳��жϱ���
}


