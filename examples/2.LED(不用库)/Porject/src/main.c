/******************************2012-2014, NJTECH, Edu.*************************** 
FileName: led/main.c
Author:  孙冬梅       Version :  1.0        Date: 2014.01.30
Description:    不用库，在学习板上实现1个LED闪烁
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    14/01/30     1.0     文件创建   
*******************************************************************************/ 

#include <stm32f10x.h> 	 

void Delay(unsigned int t);	

main()
{  
	RCC -> APB2ENR = 1 << 5;//1左移5位 [2]P71   IOPDEN：IO端口D时钟使能 (I/O port D clock enable) 位5 由软件置’1’或清’0’ 0：IO端口D时钟关闭； 1：IO端口D时钟开启。
	GPIOD -> CRL = 2 << 8;  //2左移8位(D口) [2]P113 01：推挽输出模式 10：输出模式，最大速度2MHz
 	while(1)
	{
		GPIOD ->BSRR = 1 << 2;
		Delay(500000); 		
		GPIOD ->BRR  = 1 << 2;  
		Delay(500000); 
	}
}

void Delay(unsigned int t)
{ 
	while(t--) ;	
}

