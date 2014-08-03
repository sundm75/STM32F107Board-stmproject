/******************************2012-2014, NJTECH, Edu.*************************** 
FileName: led/main.c
Author:  孙冬梅       Version :  1.0        Date: 2014.01.30
Description:    USB HID FS 设备例程
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    14/01/30     1.0     文件创建   
*******************************************************************************/ 

#include "stm32f10x.h"
#include <stdio.h>
#include "led.h"
#include "delay.h"

#include  "usbd_hid_core.h"
#include  "usbd_usr.h"
#include  "usbd_desc.h"

__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

int main(void)
{
  LED_Init();
  USBD_Init(&USB_OTG_dev,
            USB_OTG_FS_CORE_ID,
            &USR_desc, 
            &USBD_HID_cb, 
            &USR_cb);
  while (1)
  {
    LEDTog(LED1);
    LEDTog(LED2);
    LEDTog(LED3);
    LEDTog(LED4);
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



