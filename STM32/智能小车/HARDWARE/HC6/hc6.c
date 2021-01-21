#include "hc6.h"
#include "sys.h"
#include "delay.h"
#include "wheel.h"
#include "usart.h"



void HCSR04_Init(u16 arr,u16 psc)
{
 	RCC->APB1ENR|=1<<2;    //TIM4ʱ��ʹ��
  	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��
	RCC->APB2ENR|=1<<3;	 //ʹ��PORTBʱ��
	RCC->APB2ENR|=1<<4; 	//ʹ��PORTCʱ��
   	GPIOB->CRL&=0X00FFFFFF;//PB6~9���֮ǰ����
    GPIOB->CRL|=0X88000000;//PB6~9��������
		GPIOB->CRH&=0XF000FF00;
	  GPIOB->CRH|=0X03330088;
		GPIOB->ODR|=0<<6;
    GPIOB->ODR|=0<<7;      //PB7����
		GPIOB->ODR|=0<<8;
		GPIOB->ODR|=0<<9;
	
    GPIOB->ODR|=1<<12;      //PB4 �����
    GPIOB->ODR|=1<<13;
		GPIOB->ODR|=1<<14;
	
    TIM4->ARR=arr;         //�趨�������Զ���װֵ
    TIM4->PSC=psc;         //Ԥ��Ƶ��
	
	TIM4->CCMR1|=1<<0;     //CC1S=01 ѡ�������IC1ӳ�䵽TI1
    TIM4->CCMR1|=1<<4;     //IC1F=0001 �����˲��� ��Fck_int�����������¼�����Ч
    TIM4->CCMR1|=0<<2;    //IC1PS=00 ���������Ƶ������Ƶ
		
    TIM4->CCER|=0<<1;      //CC1P=0 �����ز���
    TIM4->CCER|=1<<0;      //CC1E=1 �������������ֵ������Ĵ�����
	
    TIM4->CCMR1|=1<<8;     //CC2S=01 ѡ�������IC1ӳ�䵽TI1
    TIM4->CCMR1|=1<<12;     //IC2F=0001 �����˲��� ��Fck_int�����������¼�����Ч
    TIM4->CCMR1|=0<<10;    //IC2PS=00 ���������Ƶ������Ƶ
	
    TIM4->CCER|=0<<5;      //CC2P=0 �����ز���
    TIM4->CCER|=1<<4;      //CC2E=1 �������������ֵ������Ĵ�����
		
	TIM4->CCMR2|=1<<0;     //CC3S=01 ѡ�������IC1ӳ�䵽TI1
    TIM4->CCMR2|=1<<4;     //IC3F=0001 �����˲��� ��Fck_int�����������¼�����Ч
    TIM4->CCMR2|=0<<2;    //IC3PS=00 ���������Ƶ������Ƶ
	
    TIM4->CCER|=0<<9;      //CC3P=0 �����ز���
    TIM4->CCER|=1<<8;      //CC3E=1 �������������ֵ������Ĵ�����
		
	TIM4->CCMR2|=1<<8;     //CC4S=01 ѡ�������IC1ӳ�䵽TI1
    TIM4->CCMR2|=1<<12;     //IC4F=0001 �����˲��� ��Fck_int�����������¼�����Ч
    TIM4->CCMR2|=0<<10;    //IC4PS=00 ���������Ƶ������Ƶ
	
    TIM4->CCER|=0<<13;      //CC4P=0 �����ز���
    TIM4->CCER|=1<<12;      //CC4E=1 �������������ֵ������Ĵ�����
		
	TIM4->DIER|=1<<1;
    TIM4->DIER|=1<<2;      //�������ж�
	TIM4->DIER|=1<<3;      //�������ж�
	TIM4->DIER|=1<<4;      //�������ж�
    TIM4->DIER|=1<<0;      //��������ж�
    TIM4->CR1|=0X01;       //ʹ�ܶ�ʱ��2
    MY_NVIC_Init(2,0,TIM4_IRQn,2);//��ռ2�������ȼ�0����2
}

u8 TIM4CH1_CAPTURE_STA=0; //���벶��״̬
u16 TIM4CH1_CAPTURE_VAL;  //���벶��ֵ

u8 TIM4CH2_CAPTURE_STA=0; //���벶��״̬
u16 TIM4CH2_CAPTURE_VAL;  //���벶��ֵ

u8 TIM4CH3_CAPTURE_STA=0; //���벶��״̬
u16 TIM4CH3_CAPTURE_VAL;  //���벶��ֵ

u8 TIM4CH4_CAPTURE_STA=0; //���벶��״̬
u16 TIM4CH4_CAPTURE_VAL;  //���벶��ֵ

//��ʱ��4�жϷ������
void TIM4_IRQHandler(void)
{
    u16 tsr;
    tsr=TIM4->SR;
		if((TIM4CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
    {
        if(tsr&0X01)//���
        {
            if(TIM4CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
            {
                if((TIM4CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
                {
                    TIM4CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
                    TIM4CH1_CAPTURE_VAL=0XFFFF;
                }else TIM4CH1_CAPTURE_STA++;
            }
        }
        if(tsr&0x02)//����1���������¼�
        {
            if(TIM4CH1_CAPTURE_STA&0X40) //����һ���½���
            {
                TIM4CH1_CAPTURE_STA|=0X80; //��ǳɹ�����һ�θߵ�ƽ����
                TIM4CH1_CAPTURE_VAL=TIM4->CCR1;//��ȡ��ǰ�Ĳ���ֵ
                TIM4->CCER&=~(1<<1);    //CC1P=0 ����Ϊ�����ز���
            }else                       //��δ��ʼ����һ�β���������
            {
                TIM4CH1_CAPTURE_VAL=0;
                TIM4CH1_CAPTURE_STA=0X40; //��ǲ�����������
                TIM4->CNT=0;             //���������
                TIM4->CCER|=1<<1;        //CC1P=1 ����Ϊ�½��ز���
            }
        }
    }
    if((TIM4CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
    {
        if(tsr&0X01)//���
        {
            if(TIM4CH2_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
            {
                if((TIM4CH2_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
                {
                    TIM4CH2_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
                    TIM4CH2_CAPTURE_VAL=0XFFFF;
                }else TIM4CH2_CAPTURE_STA++;
            }
        }
        if(tsr&0x04)//����1���������¼�
        {
            if(TIM4CH2_CAPTURE_STA&0X40) //����һ���½���
            {
                TIM4CH2_CAPTURE_STA|=0X80; //��ǳɹ�����һ�θߵ�ƽ����
                TIM4CH2_CAPTURE_VAL=TIM4->CCR2;//��ȡ��ǰ�Ĳ���ֵ
                TIM4->CCER&=~(1<<5);    //CC1P=0 ����Ϊ�����ز���
            }else                       //��δ��ʼ����һ�β���������
            {
                TIM4CH2_CAPTURE_VAL=0;
                TIM4CH2_CAPTURE_STA=0X40; //��ǲ�����������
                TIM4->CNT=0;             //���������
                TIM4->CCER|=1<<5;        //CC1P=1 ����Ϊ�½��ز���
				TIM4->CR1|=0x01;
            }
        }
    }
		
		if((TIM4CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
    {
        if(tsr&0X01)//���
        {
            if(TIM4CH3_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
            {
                if((TIM4CH3_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
                {
                    TIM4CH3_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
                    TIM4CH3_CAPTURE_VAL=0XFFFF;
                }else TIM4CH3_CAPTURE_STA++;
            }
        }
        if(tsr&0x08)//����1���������¼�
        {
            if(TIM4CH3_CAPTURE_STA&0X40) //����һ���½���
            {
                TIM4CH3_CAPTURE_STA|=0X80; //��ǳɹ�����һ�θߵ�ƽ����
                TIM4CH3_CAPTURE_VAL=TIM4->CCR3;//��ȡ��ǰ�Ĳ���ֵ
                TIM4->CCER&=~(1<<9);    //CC1P=0 ����Ϊ�����ز���
            }else                       //��δ��ʼ����һ�β���������
            {
                TIM4CH3_CAPTURE_VAL=0;
                TIM4CH3_CAPTURE_STA=0X40; //��ǲ�����������
                TIM4->CNT=0;  
				TIM4CH3_CAPTURE_VAL=TIM4->CCR3;							//���������
                TIM4->CCER|=1<<9;        //CC1P=1 ����Ϊ�½��ز���
				TIM4->CR1|=0x01;
            }
        }
	}
		
	if((TIM4CH4_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
    {
        if(tsr&0X01)//���
        {
            if(TIM4CH4_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
            {
                if((TIM4CH4_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
                {
                    TIM4CH4_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
                    TIM4CH4_CAPTURE_VAL=0XFFFF;
                }else TIM4CH4_CAPTURE_STA++;
            }
        }
        if(tsr&0x10)//����1���������¼�
        {
            if(TIM4CH4_CAPTURE_STA&0X40) //����һ���½���
            {
                TIM4CH4_CAPTURE_STA|=0X80; //��ǳɹ�����һ�θߵ�ƽ����
                TIM4CH4_CAPTURE_VAL=TIM4->CCR4;//��ȡ��ǰ�Ĳ���ֵ
                TIM4->CCER&=~(1<<13);    //CC1P=0 ����Ϊ�����ز���
            }else                       //��δ��ʼ����һ�β���������
            {
                TIM4CH4_CAPTURE_VAL=0;
                TIM4CH4_CAPTURE_STA=0X40; //��ǲ�����������
                TIM4->CNT=0;             //���������
                TIM4->CCER|=1<<13;        //CC1P=1 ����Ϊ�½��ز���
				TIM4->CR1|=0x01;
            }
        }
    }				
    TIM4->SR=0;//����жϱ�־λ	
}

u32 DIS_Init(u8 *STA,u16 VAL)//���������뺯��
{
		u32 temp;
		u32 lenth;
	  if((*STA)&0X80)//�ɹ�������һ�θߵ�ƽ
    {
        temp=(*STA)&0X3F;
        temp*=65536;               //���ʱ���ܺ�
        temp+=VAL; //�õ��ܵĸߵ�ƽʱ��
        lenth=temp*0.017;           //���㳤��
        *STA=0;     //������һ�β���
    }
		return lenth;
}

#define Trig1 PBout(14) // PB13
#define Trig2 PBout(13)
#define Trig3 PBout(12)

extern u8 TIM4CH2_CAPTURE_STA; //���벶��״̬
extern u16 TIM4CH2_CAPTURE_VAL;  //���벶��ֵ

extern u8 TIM4CH3_CAPTURE_STA; //���벶��״̬
extern u16 TIM4CH3_CAPTURE_VAL;  //���벶��ֵ

extern u8 TIM4CH4_CAPTURE_STA; //���벶��״̬
extern u16 TIM4CH4_CAPTURE_VAL;  //���벶��ֵ

void Ultrasound(void)
{
		u32 lenth1;
		u32 lenth2;
		u32	lenth3;
		Trig1=1;
		delay_us(20);              //����һ��20us�ĸߵ�ƽ
		Trig1=0;
		lenth1=DIS_Init(&TIM4CH2_CAPTURE_STA,TIM4CH2_CAPTURE_VAL);
		delay_us(20);
		Trig2=1;
		delay_us(20);              //����һ��20us�ĸߵ�ƽ
		Trig2=0;
		lenth2=DIS_Init(&TIM4CH3_CAPTURE_STA,TIM4CH3_CAPTURE_VAL);
		delay_us(20);
		Trig3=1;
		delay_us(20);
		Trig3=0;
		lenth3=DIS_Init(&TIM4CH4_CAPTURE_STA,TIM4CH4_CAPTURE_VAL);
		printf("length1:%d\r\n",lenth1);
		printf("length2:%d\r\n",lenth2);
		printf("length3:%d\r\n",lenth3);
		if((lenth2>20)&&(lenth1>10)&&(lenth3>10))
		{
			printf("GO    \r\n");
			GO();
		}
		else if(lenth3<10)
		{
				printf("LEFT    \r\n");
				turnLEFT();
			delay_ms(20);
		}
		else if(lenth1<10)
		{
				printf("RIGHT   \r\n");
				turnRIGHT();
			delay_ms(20);
		}
		else
		{
			printf("BACK   \r\n");
			BACK();
			delay_ms(500);
			turnLEFT();
			delay_ms(300);
		}
		delay_ms(100);
}


