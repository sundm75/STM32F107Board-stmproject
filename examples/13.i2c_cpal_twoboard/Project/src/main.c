/******************************2016-2016, NJTECH, Edu.*************************** 
FileName: i2c_cpal/main.c
Author:  孙冬梅       Version :  1.0        Date: 2016.07.30
Description:   使用CPAL库进行I2C的 two_board 通讯
 使用方法： 1. 双板的 I2C 总线：SDA SCL GND　互连
            2. 双板上电后，一板按键key1后，发送数据。另一板可接收。
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    16/07/30     1.0     文件创建   
*******************************************************************************/ 

#include "stm32f10x.h"
#include <stdio.h>
#include "led.h"
#include "key.h"
#include "delay.h"
#include "cpal_user_i2c.h"
#include "usart.h"

int main(void)
{
  Key_Def key;
  LED_Init();
  Key_Init();
  //Set_Keyint();//设置按键中断方式
  UartConfig();
  /*1. 用CPAL库 进行双板通讯,用 CPAL_I2c_Config 初始化*/
  CPAL_I2c_Config();
  
  while (1)
  {
/*  按键查询方式   */
    key = KEY_Scan();
    switch (key)
    {
      case KEY1:
        LEDTog(LED1);
        printf("\n\rKEY_1");

        /* Write operations CPAL的I2C -----------------------------------------*/
   
        {
          User_I2c_Write();
        }

        break;
      case KEY2:
        LEDTog(LED2);
        printf("\n\rKEY_2");
        break;
      case KEYNULL:
        break;
      default :
        break;
    
    }
    
      /* 1. Read Operations CPAL的I2C  ----------------------------------------*/
      if ((I2C_DevStructure.CPAL_State == CPAL_STATE_READY) || \
        (I2C_DevStructure.CPAL_State == CPAL_STATE_DISABLED)) 
      {
        User_I2c_Read();
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


