/* stemwin移植主程序 */
/*
1.触摸屏初始化 串口初始化 
2.定时器初始化，提供10ms定时，运行GUI_TOUCH_Exec,配置OS_TimeMS
3.GUI启动后，定时1ms更新OS_TimeMS 在GUI_Delay(100)自动调用GUI_Exec().
4.标定屏幕；或者运行Demo,在GUIDEMO.h修改要运行的范例;或者运行苗成诗编写的简易计算器
*/

#include "GUI.h"
#include "guidemo.h"
#include "timer5.h"
#include "touch.h"
extern int touch_calibrate();
extern int calculator_main();

int main(void)
{ 
  touch_config();
  Timer5Config();
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
  GUI_Init();
  
  /*触摸屏标定*/
  //touch_calibrate();
  
  /*运行Demo*/

  GUIDEMO_Main();
  
  /*简易计算器*/
  //calculator_main();  

   while(1);
}
