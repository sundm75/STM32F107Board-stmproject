/******************************2012-2014, NJTECH, Edu.*************************** 
FileName: ad/main.c
Author:  孙冬梅       Version :  1.0        Date: 2014.01.30
Description:    ad功能测试主程序 在串口程序上添加形成  key2键按下后显示AD值 
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    14/01/30     1.0     文件创建   
*******************************************************************************/ 
#include "stm32f10x.h"
#include <stdio.h>
#include "led.h"
#include "usart.h"
#include "key.h"
#include "adc.h"



int main(void)
{
  Key_Def key;
  uint8_t str[10];
  uint8_t * p_str = str;   
  
   /*以下显示测量值*/    
  uint16_t Precent = 0;
  uint32_t Voltage = 0;
  uint16_t adcvalue;
  
  LED_Init();
  Key_Init();
  UartConfig();
  ADC_Config();
  
  UartPrintf("abc\n"); //测试字符串
  UartPrintf("%d %d %d\n", 987654321,22,33);//测试整型数据
  UartPrintf("%c%c%d\n" ,0x32,0x38,0x12);//测试16进制数和整型数据
  UartPrintf("abcedg\n");//测试字符串
  
  while (1)
  {
/*  按键查询方式 */  
    key = KEY_Scan();
    switch (key)
    {
      case KEY1:
        LEDTog(LED1);
        printf("\n\rKEY_1");
        printf("please input string: \n");
        scanf("%s",p_str); 
        printf("%s\n",p_str);  	
        break;
      case KEY2:
        LEDTog(LED2);
        printf("\n\rKEY_2 is pressed;\n");
        
        /*以下显示测量值*/    
        getadcvalue(&adcvalue);
        Precent = (adcvalue*100/0x1000);	
        Voltage = adcvalue * 3300/0x1000;		
        printf("\r\n 当前AD转换结果为：0x%X, 百分比为：%d% ，电压值：%d.%dV \n\r", 
              adcvalue, Precent, Voltage/1000, Voltage%1000);
        break;
       case KEYNULL:
        break;
      default :
        break;
    
    }
    
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


