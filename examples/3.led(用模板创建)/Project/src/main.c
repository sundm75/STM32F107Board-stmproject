/******************************2012-2014, NJTECH, Edu.*************************** 
FileName: led/main.c
Author:  孙冬梅       Version :  1.0        Date: 2014.01.30
Description:    用库函数生成led工程，主程序 ，进行led显示测试
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    14/01/30     1.0     文件创建   
*******************************************************************************/ 

#include "stm32f10x.h"
#include <stdio.h>
#include "led.h"
#include "delay.h"



int main(void)
{

  LED_Init();
  /* led.c中的函数测试 
  LEDOn(LED1);
  LEDOn(LED2);
  LEDOn(LED3);
  LEDOn(LED4);
  LEDOff(LED1);
  LEDOff(LED2);
  LEDOff(LED3);
  LEDOff(LED4);
  LEDTog(LED1);
  LEDTog(LED2);
  LEDTog(LED3);
  LEDTog(LED4);
  LEDTog(LED1);
  LEDTog(LED2);
  LEDTog(LED3);
  LEDTog(LED4);
  LEDOn(LEDALL);
  LEDOff(LEDALL);
  LEDOn(LED2);
  LEDOn(LED3);
  LEDTog(LEDALL);*/
  while (1)
  {
    GPIO_SetBits( GPIOD, GPIO_Pin_2 );
    Delay(1000000);
    GPIO_ResetBits( GPIOD, GPIO_Pin_2 );
    Delay(1000000);
    
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */



