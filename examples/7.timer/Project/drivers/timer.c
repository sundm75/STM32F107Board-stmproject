/*********************2012-2013, NJUT, Edu.********************* 
FileName: timer.c 
Author:  孙冬梅       Version :  1.0        Date: 2013.04.10
Description:    timer驱动      
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    13/04/10     1.0     文件创建   
***************************************************************/ 
#include "stm32f10x.h"
#include "led.h"
	 

//////////////////////////////////////////////////////////////////////
/*******************************************************************************
* Function Name  : Timer5Config
* Description    : Timer5定时器配置函数,定时周期：1S 分频：1/7200 向上计数10000
*                  72M * 1/7200 *10000 = 1S
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#include "timer.h"
void Timer5Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;  	
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

  TIM_TimeBaseStructure.TIM_Period = (10000 - 1);		 	
  TIM_TimeBaseStructure.TIM_Prescaler = (7200 - 1);	
  TIM_TimeBaseStructure.TIM_ClockDivision = 0 ;			
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);		
  
  TIM_ClearITPendingBit(TIM5, TIM_IT_Update);		
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);		  	
  TIM_Cmd(TIM5, ENABLE);				       

  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
  NVIC_Init(&NVIC_InitStructure);	                       
}	

/*******************************************************************************
* Function Name  : TIM5_IRQHandler
* Description    : 定时器5中断服务函数
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void TIM5_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    /*
      这里添加自己的代码
    */
    LEDTog(LED1);
    
  }
}

