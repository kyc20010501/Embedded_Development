#include "hc6.h"
#include "sys.h"
#include "delay.h"
#include "wheel.h"
#include "usart.h"



void HCSR04_Init(u16 arr,u16 psc)
{
 	RCC->APB1ENR|=1<<2;    //TIM4时钟使能
  	RCC->APB2ENR|=1<<2;    //使能PORTA时钟
	RCC->APB2ENR|=1<<3;	 //使能PORTB时钟
	RCC->APB2ENR|=1<<4; 	//使能PORTC时钟
   	GPIOB->CRL&=0X00FFFFFF;//PB6~9清除之前设置
    GPIOB->CRL|=0X88000000;//PB6~9浮空输入
		GPIOB->CRH&=0XF000FF00;
	  GPIOB->CRH|=0X03330088;
		GPIOB->ODR|=0<<6;
    GPIOB->ODR|=0<<7;      //PB7下拉
		GPIOB->ODR|=0<<8;
		GPIOB->ODR|=0<<9;
	
    GPIOB->ODR|=1<<12;      //PB4 输出高
    GPIOB->ODR|=1<<13;
		GPIOB->ODR|=1<<14;
	
    TIM4->ARR=arr;         //设定计数器自动重装值
    TIM4->PSC=psc;         //预分频器
	
	TIM4->CCMR1|=1<<0;     //CC1S=01 选择输入端IC1映射到TI1
    TIM4->CCMR1|=1<<4;     //IC1F=0001 配置滤波器 以Fck_int采样，两个事件后有效
    TIM4->CCMR1|=0<<2;    //IC1PS=00 配置输入分频，不分频
		
    TIM4->CCER|=0<<1;      //CC1P=0 上升沿捕获
    TIM4->CCER|=1<<0;      //CC1E=1 允许捕获计数器的值到捕获寄存器中
	
    TIM4->CCMR1|=1<<8;     //CC2S=01 选择输入端IC1映射到TI1
    TIM4->CCMR1|=1<<12;     //IC2F=0001 配置滤波器 以Fck_int采样，两个事件后有效
    TIM4->CCMR1|=0<<10;    //IC2PS=00 配置输入分频，不分频
	
    TIM4->CCER|=0<<5;      //CC2P=0 上升沿捕获
    TIM4->CCER|=1<<4;      //CC2E=1 允许捕获计数器的值到捕获寄存器中
		
	TIM4->CCMR2|=1<<0;     //CC3S=01 选择输入端IC1映射到TI1
    TIM4->CCMR2|=1<<4;     //IC3F=0001 配置滤波器 以Fck_int采样，两个事件后有效
    TIM4->CCMR2|=0<<2;    //IC3PS=00 配置输入分频，不分频
	
    TIM4->CCER|=0<<9;      //CC3P=0 上升沿捕获
    TIM4->CCER|=1<<8;      //CC3E=1 允许捕获计数器的值到捕获寄存器中
		
	TIM4->CCMR2|=1<<8;     //CC4S=01 选择输入端IC1映射到TI1
    TIM4->CCMR2|=1<<12;     //IC4F=0001 配置滤波器 以Fck_int采样，两个事件后有效
    TIM4->CCMR2|=0<<10;    //IC4PS=00 配置输入分频，不分频
	
    TIM4->CCER|=0<<13;      //CC4P=0 上升沿捕获
    TIM4->CCER|=1<<12;      //CC4E=1 允许捕获计数器的值到捕获寄存器中
		
	TIM4->DIER|=1<<1;
    TIM4->DIER|=1<<2;      //允许捕获中断
	TIM4->DIER|=1<<3;      //允许捕获中断
	TIM4->DIER|=1<<4;      //允许捕获中断
    TIM4->DIER|=1<<0;      //允许更新中断
    TIM4->CR1|=0X01;       //使能定时器2
    MY_NVIC_Init(2,0,TIM4_IRQn,2);//抢占2，子优先级0，组2
}

u8 TIM4CH1_CAPTURE_STA=0; //输入捕获状态
u16 TIM4CH1_CAPTURE_VAL;  //输入捕获值

u8 TIM4CH2_CAPTURE_STA=0; //输入捕获状态
u16 TIM4CH2_CAPTURE_VAL;  //输入捕获值

u8 TIM4CH3_CAPTURE_STA=0; //输入捕获状态
u16 TIM4CH3_CAPTURE_VAL;  //输入捕获值

u8 TIM4CH4_CAPTURE_STA=0; //输入捕获状态
u16 TIM4CH4_CAPTURE_VAL;  //输入捕获值

//定时器4中断服务程序
void TIM4_IRQHandler(void)
{
    u16 tsr;
    tsr=TIM4->SR;
		if((TIM4CH1_CAPTURE_STA&0X80)==0)//还未成功捕获
    {
        if(tsr&0X01)//溢出
        {
            if(TIM4CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
            {
                if((TIM4CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
                {
                    TIM4CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
                    TIM4CH1_CAPTURE_VAL=0XFFFF;
                }else TIM4CH1_CAPTURE_STA++;
            }
        }
        if(tsr&0x02)//捕获1发生捕获事件
        {
            if(TIM4CH1_CAPTURE_STA&0X40) //捕获到一个下降沿
            {
                TIM4CH1_CAPTURE_STA|=0X80; //标记成功捕获到一次高电平脉宽
                TIM4CH1_CAPTURE_VAL=TIM4->CCR1;//获取当前的捕获值
                TIM4->CCER&=~(1<<1);    //CC1P=0 设置为上升沿捕获
            }else                       //还未开始，第一次捕获上升沿
            {
                TIM4CH1_CAPTURE_VAL=0;
                TIM4CH1_CAPTURE_STA=0X40; //标记捕获到了上升沿
                TIM4->CNT=0;             //计数器清空
                TIM4->CCER|=1<<1;        //CC1P=1 设置为下降沿捕获
            }
        }
    }
    if((TIM4CH2_CAPTURE_STA&0X80)==0)//还未成功捕获
    {
        if(tsr&0X01)//溢出
        {
            if(TIM4CH2_CAPTURE_STA&0X40)//已经捕获到高电平了
            {
                if((TIM4CH2_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
                {
                    TIM4CH2_CAPTURE_STA|=0X80;//标记成功捕获了一次
                    TIM4CH2_CAPTURE_VAL=0XFFFF;
                }else TIM4CH2_CAPTURE_STA++;
            }
        }
        if(tsr&0x04)//捕获1发生捕获事件
        {
            if(TIM4CH2_CAPTURE_STA&0X40) //捕获到一个下降沿
            {
                TIM4CH2_CAPTURE_STA|=0X80; //标记成功捕获到一次高电平脉宽
                TIM4CH2_CAPTURE_VAL=TIM4->CCR2;//获取当前的捕获值
                TIM4->CCER&=~(1<<5);    //CC1P=0 设置为上升沿捕获
            }else                       //还未开始，第一次捕获上升沿
            {
                TIM4CH2_CAPTURE_VAL=0;
                TIM4CH2_CAPTURE_STA=0X40; //标记捕获到了上升沿
                TIM4->CNT=0;             //计数器清空
                TIM4->CCER|=1<<5;        //CC1P=1 设置为下降沿捕获
				TIM4->CR1|=0x01;
            }
        }
    }
		
		if((TIM4CH3_CAPTURE_STA&0X80)==0)//还未成功捕获
    {
        if(tsr&0X01)//溢出
        {
            if(TIM4CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
            {
                if((TIM4CH3_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
                {
                    TIM4CH3_CAPTURE_STA|=0X80;//标记成功捕获了一次
                    TIM4CH3_CAPTURE_VAL=0XFFFF;
                }else TIM4CH3_CAPTURE_STA++;
            }
        }
        if(tsr&0x08)//捕获1发生捕获事件
        {
            if(TIM4CH3_CAPTURE_STA&0X40) //捕获到一个下降沿
            {
                TIM4CH3_CAPTURE_STA|=0X80; //标记成功捕获到一次高电平脉宽
                TIM4CH3_CAPTURE_VAL=TIM4->CCR3;//获取当前的捕获值
                TIM4->CCER&=~(1<<9);    //CC1P=0 设置为上升沿捕获
            }else                       //还未开始，第一次捕获上升沿
            {
                TIM4CH3_CAPTURE_VAL=0;
                TIM4CH3_CAPTURE_STA=0X40; //标记捕获到了上升沿
                TIM4->CNT=0;  
				TIM4CH3_CAPTURE_VAL=TIM4->CCR3;							//计数器清空
                TIM4->CCER|=1<<9;        //CC1P=1 设置为下降沿捕获
				TIM4->CR1|=0x01;
            }
        }
	}
		
	if((TIM4CH4_CAPTURE_STA&0X80)==0)//还未成功捕获
    {
        if(tsr&0X01)//溢出
        {
            if(TIM4CH4_CAPTURE_STA&0X40)//已经捕获到高电平了
            {
                if((TIM4CH4_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
                {
                    TIM4CH4_CAPTURE_STA|=0X80;//标记成功捕获了一次
                    TIM4CH4_CAPTURE_VAL=0XFFFF;
                }else TIM4CH4_CAPTURE_STA++;
            }
        }
        if(tsr&0x10)//捕获1发生捕获事件
        {
            if(TIM4CH4_CAPTURE_STA&0X40) //捕获到一个下降沿
            {
                TIM4CH4_CAPTURE_STA|=0X80; //标记成功捕获到一次高电平脉宽
                TIM4CH4_CAPTURE_VAL=TIM4->CCR4;//获取当前的捕获值
                TIM4->CCER&=~(1<<13);    //CC1P=0 设置为上升沿捕获
            }else                       //还未开始，第一次捕获上升沿
            {
                TIM4CH4_CAPTURE_VAL=0;
                TIM4CH4_CAPTURE_STA=0X40; //标记捕获到了上升沿
                TIM4->CNT=0;             //计数器清空
                TIM4->CCER|=1<<13;        //CC1P=1 设置为下降沿捕获
				TIM4->CR1|=0x01;
            }
        }
    }				
    TIM4->SR=0;//清除中断标志位	
}

u32 DIS_Init(u8 *STA,u16 VAL)//定义计算距离函数
{
		u32 temp;
		u32 lenth;
	  if((*STA)&0X80)//成功捕获到了一次高电平
    {
        temp=(*STA)&0X3F;
        temp*=65536;               //溢出时间总和
        temp+=VAL; //得到总的高电平时间
        lenth=temp*0.017;           //计算长度
        *STA=0;     //开启下一次捕获
    }
		return lenth;
}

#define Trig1 PBout(14) // PB13
#define Trig2 PBout(13)
#define Trig3 PBout(12)

extern u8 TIM4CH2_CAPTURE_STA; //输入捕获状态
extern u16 TIM4CH2_CAPTURE_VAL;  //输入捕获值

extern u8 TIM4CH3_CAPTURE_STA; //输入捕获状态
extern u16 TIM4CH3_CAPTURE_VAL;  //输入捕获值

extern u8 TIM4CH4_CAPTURE_STA; //输入捕获状态
extern u16 TIM4CH4_CAPTURE_VAL;  //输入捕获值

void Ultrasound(void)
{
		u32 lenth1;
		u32 lenth2;
		u32	lenth3;
		Trig1=1;
		delay_us(20);              //输入一个20us的高电平
		Trig1=0;
		lenth1=DIS_Init(&TIM4CH2_CAPTURE_STA,TIM4CH2_CAPTURE_VAL);
		delay_us(20);
		Trig2=1;
		delay_us(20);              //输入一个20us的高电平
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


