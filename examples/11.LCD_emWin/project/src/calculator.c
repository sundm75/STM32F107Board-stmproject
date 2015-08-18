
/*************************** Nanjing Tech University *************************** 
**************** College of Automation and Electrical Engineering **************
****************************** Copyright Reserved ****************************** 

FileName: calculator.c 
Description:    emWIN 移植 触屏计算器   
Version:          0.0.2 
History:  
Author: 苗成诗

       <Version>        <Time>                 <What's New>
        V0.1           2014/7/2           实现基本计算，显示功能

        V1.0           2014/7/3           1.添加最大化，最小化和关闭按钮,删减了部分无用变量和程序;
                                          2.增加按钮的立体化显示;
                                          3.修复了等号后没有换行的问题；
                                          4.增加连续计算功能，可以使用上一次结果直接进行下一步运算；
                                          5.修正了连续按符号键会显示多符号的问题。
*******************************************************************************/

#include "lcddevice.h" 
#include "GUI.h"
#include "guidemo.h"
#include "timer5.h"
#include "touch.h"
#include <stdlib.h>
#include <string.h>
#include "global_includes.h"
/*
  *+---------------------------------------------------------------------------+
  /   Variable   /                      Descriptions
  *+---------------------------------------------------------------------------+
  /      i_      /   用于区分按键button。
  *+---------------------------------------------------------------------------+
  /    count2    /  用于输入数字的小数部分位数的记录。
  *+---------------------------------------------------------------------------+
  /    flag      /  用于标志目前输入状态，如果在输入整数部分，则flag为默认值1。
  *+---------------------------------------------------------------------------+
  /    flag1     /  用于标志等号输入状态，如果已经输入等号，flag1置1，后面输入
                    换行显示。
  *+---------------------------------------------------------------------------+
  /    flag2     /  用于标志符号状态，0表示无符号输入，1表示输入运算符“+”，
  /                2表示输入“-”，3表示输入“*”，4表示输入“/”，在“=”相应程
  /                序中根据flag2的值选择不同计算规则。
 *+----------------------------------------------------------------------------+
  /    cal1      /  记录输入的整数部分。
  *+---------------------------------------------------------------------------+
  /    cal2      /  记录输入的小数部分。
  *+---------------------------------------------------------------------------+
  /    cal       /  记录最终结果。
  *+---------------------------------------------------------------------------+
  /   flag_add   /  符号标志，用于修正符号连续显示的问题。
  *+---------------------------------------------------------------------------+

*/

uint8_t GUI_Initialized   = 0;
uint16_t i_1,i_2,i_3,i_4,i_5,i_6,i_7,i_8,i_9;
uint16_t i_10,i_11,i_12,i_13,i_14,i_15,i_16,i_17,i_18;
uint16_t count2=0;
uint16_t flag=1,flag1=0,flag2=0;
uint16_t flag_add=0;
long float cal1=0,cal2=0,cal=0,temp=1;
int i,j,m,n;
char str[];
char* str1,*str2;

int calculator_main(void)
{ 

	FRAMEWIN_Handle hFrame ;
	WM_HWIN hClose;
	WM_HWIN hMax;
	WM_HWIN hMin;
	MULTIEDIT_HANDLE hWIN;
	
BUTTON_Handle hButton1;
BUTTON_Handle hButton2;
BUTTON_Handle hButton3;
BUTTON_Handle hButton4;
BUTTON_Handle hButton5;
BUTTON_Handle hButton6;
BUTTON_Handle hButton7;
BUTTON_Handle hButton8;
BUTTON_Handle hButton9;
BUTTON_Handle hButton10;
BUTTON_Handle hButton11;
BUTTON_Handle hButton12;
BUTTON_Handle hButton13;
BUTTON_Handle hButton14;
BUTTON_Handle hButton15;
BUTTON_Handle hButton16;
BUTTON_Handle hButton17;
BUTTON_Handle hButton18;

  WM_SetCreateFlags(WM_CF_MEMDEV);
  GUI_Clear();
  GUI_Initialized = 1;
 
/*设置窗体、按钮和滚动条的显示方式*/  
  FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
  BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
  SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
  
/*界面，窗口，字体，设计,最大化按钮，关闭按钮，最小化按钮*/ 
  hFrame = FRAMEWIN_Create("Calculator V 1.0",NULL,WM_CF_SHOW,2,2,320,240);
  hClose=FRAMEWIN_AddCloseButton(hFrame,FRAMEWIN_BUTTON_RIGHT, 0);
  hMax=FRAMEWIN_AddMaxButton(hFrame,FRAMEWIN_BUTTON_RIGHT, 1);
  hMin=FRAMEWIN_AddMinButton(hFrame,FRAMEWIN_BUTTON_RIGHT,1);
  FRAMEWIN_SetFont(hFrame,&GUI_Font16B_ASCII);
  FRAMEWIN_SetBarColor(hFrame,1,GUI_DARKBLUE);
  FRAMEWIN_SetBarColor(hFrame,0,GUI_DARKGRAY);
  FRAMEWIN_SetClientColor(hFrame,GUI_LIGHTGRAY);
   
 /*显示多行文本，更该字体，对齐方式,换行等*/
  hWIN = MULTIEDIT_Create(25,30,260,40,hFrame,GUI_ID_OK,WM_CF_SHOW,MULTIEDIT_CF_AUTOSCROLLBAR_V,NULL,20);
  MULTIEDIT_SetBkColor(hWIN,MULTIEDIT_CI_READONLY,GUI_WHITE);
  MULTIEDIT_SetFont(hWIN,&GUI_Font16B_ASCII);
  MULTIEDIT_SetTextAlign(hWIN,GUI_TA_RIGHT);
  MULTIEDIT_SetWrapWord(hWIN);
  
  /*button设计*/
  hButton1 = BUTTON_CreateEx(10,80,60,30,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
  hButton2 = BUTTON_CreateEx(80,80,60,30,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
  hButton3 = BUTTON_CreateEx(150,80,60,30,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
  hButton4 = BUTTON_CreateEx(10,120,60,30,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
  hButton5 = BUTTON_CreateEx(80,120,60,30,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
  hButton6 = BUTTON_CreateEx(150,120,60,30,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
  hButton7 = BUTTON_CreateEx(10,160,60,30,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
  hButton8 = BUTTON_CreateEx(80,160,60,30,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
  hButton9 = BUTTON_CreateEx(150,160,60,30,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
  hButton10 = BUTTON_CreateEx(10,200,60,30,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
  hButton11 = BUTTON_CreateEx(80,200,60,30,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
  hButton12 = BUTTON_CreateEx(150,200,60,30,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
  hButton13 = BUTTON_CreateEx(220,80,40,40,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
  hButton14 = BUTTON_CreateEx(270,80,40,40,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
  hButton15 = BUTTON_CreateEx(220,130,40,40,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
  hButton16 = BUTTON_CreateEx(270,130,40,40,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
  hButton17 = BUTTON_CreateEx(220,180,40,40,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
  hButton18 = BUTTON_CreateEx(270,180,40,40,hFrame,WM_CF_SHOW,NULL,GUI_ID_OK);
 /*button上字符设计*/
  BUTTON_SetText(hButton1,"1");BUTTON_SetText(hButton2,"2");BUTTON_SetText(hButton3,"3");
  BUTTON_SetText(hButton4,"4");BUTTON_SetText(hButton5,"5");BUTTON_SetText(hButton6,"6");
  BUTTON_SetText(hButton7,"7");BUTTON_SetText(hButton8,"8");BUTTON_SetText(hButton9,"9");
  BUTTON_SetText(hButton10,"0");BUTTON_SetText(hButton11,".");BUTTON_SetText(hButton12,"(-)");
  BUTTON_SetText(hButton13,"+");BUTTON_SetText(hButton14,"-");BUTTON_SetText(hButton15,"*");
  BUTTON_SetText(hButton16,"/");BUTTON_SetText(hButton17,"Clear");BUTTON_SetText(hButton18,"=");
  
  WM_Exec(); 
  
  while(1)
  {
          i_1=BUTTON_IsPressed(hButton1);i_2=BUTTON_IsPressed(hButton2);i_3=BUTTON_IsPressed(hButton3);
          i_4=BUTTON_IsPressed(hButton4);i_5=BUTTON_IsPressed(hButton5);i_6=BUTTON_IsPressed(hButton6);
          i_7=BUTTON_IsPressed(hButton7);i_8=BUTTON_IsPressed(hButton8);i_9=BUTTON_IsPressed(hButton9);
          i_10=BUTTON_IsPressed(hButton10);i_11=BUTTON_IsPressed(hButton11);i_12=BUTTON_IsPressed(hButton12);
          i_13=BUTTON_IsPressed(hButton13);i_14=BUTTON_IsPressed(hButton14);i_15=BUTTON_IsPressed(hButton15);
          i_16=BUTTON_IsPressed(hButton16);i_17=BUTTON_IsPressed(hButton17);i_18=BUTTON_IsPressed(hButton18);
          
          WM_Exec();
          GUI_Delay(100);
          /*按键“1”*/
          if(i_1==1)
            {
              flag_add=0;
              if(flag1==1)
              {
                MULTIEDIT_AddText(hWIN,"\n");
                flag1=0;cal=0;
              }
              MULTIEDIT_AddText(hWIN,"1");WM_Exec();
              if(flag==1)
              {
                cal1=cal1*10+1;
              }
              else
              {
                cal2=cal2*10+1;
                count2=count2+1;
              }
            }
         
          /*按键“2”*/
          if(i_2==1)
            {
              flag_add=0;
              if(flag1==1)
              {
                MULTIEDIT_AddText(hWIN,"\n");
                flag1=0;cal=0;
              }
              MULTIEDIT_AddText(hWIN,"2"); WM_Exec();
              if(flag==1)
              {
                cal1=cal1*10+2;
                
              }
              else
              {
                cal2=cal2*10+2;
                count2=count2+1;
              }
            }
          
          /*按键“3”*/
          if(i_3==1)
            {
              flag_add=0;
              if(flag1==1)
              {
                MULTIEDIT_AddText(hWIN,"\n");
                flag1=0;cal=0;
              }
              MULTIEDIT_AddText(hWIN,"3"); WM_Exec();
              if(flag==1)
              {
                
                cal1=cal1*10+3;
              }
              else
              {
                
                cal2=cal2*10+3;
                count2=count2+1;
              }
            }
          
          /*按键“4”*/
          if(i_4==1)
            {
              flag_add=0;
              if(flag1==1)
              {
                MULTIEDIT_AddText(hWIN,"\n");
                flag1=0;cal=0;
              }
              MULTIEDIT_AddText(hWIN,"4"); WM_Exec();
              if(flag==1)
              {
                
                cal1=cal1*10+4;
              }
              else
              {
                cal2=cal2*10+4;
                count2=count2+1;
              }
            }
          
          /*按键“5”*/
          if(i_5==1)
            {
              flag_add=0;
              if(flag1==1)
              {
                MULTIEDIT_AddText(hWIN,"\n");
                flag1=0;cal=0;
              }
              MULTIEDIT_AddText(hWIN,"5"); WM_Exec();
              if(flag==1)
              {
                
                cal1=cal1*10+5;
              }
              else
              {
                
                cal2=cal2*10+5;
                count2=count2+1;
              }
            }
          
          /*按键“6”*/
          if(i_6==1)
            {
              flag_add=0;
              if(flag1==1)
              {
                MULTIEDIT_AddText(hWIN,"\n");
                flag1=0;cal=0;
              }
              MULTIEDIT_AddText(hWIN,"6"); WM_Exec();
              if(flag==1)
              {
                
                cal1=cal1*10+6;
                
              }
              else
              {
                
                cal2=cal2*10+6;
                count2=count2+1;
              }
            }
          
          /*按键“7”*/
          if(i_7==1)
            {
              flag_add=0;
              if(flag1==1)
              {
                MULTIEDIT_AddText(hWIN,"\n");
                flag1=0;cal=0;
              }
              MULTIEDIT_AddText(hWIN,"7"); WM_Exec();
              if(flag==1)
              {
                
                cal1=cal1*10+7;
              }
              else
              {
                
                cal2=cal2*10+7;
                count2=count2+1;
              }
            }
         
          /*按键“8”*/
          if(i_8==1)
            {
              flag_add=0;
              if(flag1==1)
              {
                MULTIEDIT_AddText(hWIN,"\n");
                flag1=0;cal=0;
              }
              MULTIEDIT_AddText(hWIN,"8"); WM_Exec();
              if(flag==1)
              {
                
                cal1=cal1*10+8;
              }
              else
              {
                
                cal2=cal2*10+8;
                count2=count2+1;
              }
            }
          
          /*按键“9”*/
          if(i_9==1)
            {
              if(flag1==1)
              {
                flag_add=0;
                MULTIEDIT_AddText(hWIN,"\n");
                flag1=0;cal=0;
              }
              MULTIEDIT_AddText(hWIN,"9"); WM_Exec();
              if(flag==1)
              {
                
                cal1=cal1*10+9;
              }
              else
              {
                
                cal2=cal2*10+9;
                count2=count2+1;
              }
            }
          
          /*按键“0”*/
          if(i_10==1)
            {
              flag_add=0;
              if(flag1==1)
              {
                MULTIEDIT_AddText(hWIN,"\n");
                flag1=0;cal=0;
              }
              MULTIEDIT_AddText(hWIN,"0"); WM_Exec();
              if(flag==1)
              {

                cal1=cal1*10+0;
              }
              else
              {
                
                cal2=cal2*10+0;
                count2=count2+1;
              }
            }
          
          /*按键“.”*/
          if(i_11==1)
            {
              flag_add=0;
              if(flag==1)
              {
                MULTIEDIT_AddText(hWIN,"."); WM_Exec();
                flag=0;
              }
            }
          
          
          /*保留键，用于删除*/
          if(i_12==1)
            {
              MULTIEDIT_AddText(hWIN,"NULL"); 
              WM_Exec();
            }
          
          /*按键“+”*/
          if(i_13==1)
            {
              if(flag_add==0)
              {
                  MULTIEDIT_AddText(hWIN,"+"); WM_Exec();
                  flag=1;flag1=0;
                  for(m=0;m<count2;m++)
                  {
                    cal2=cal2*0.1;
                  }
                  cal=cal+(cal1+cal2);
                  flag2=1;flag_add=1;
                  count2=0;cal1=0;cal2=0;
              }
              
             /*
              "%.3f"   转为浮点型，三位小数
              "%d"   转为整型
             */

            }
          
          /*按键“-”*/
          if(i_14==1)
            {
              if(flag_add==0)
              {
                  MULTIEDIT_AddText(hWIN,"-"); WM_Exec();
                  flag=1;flag1=0;
                  for(m=0;m<count2;m++)
                  {
                    cal2=cal2*0.1;
                  }
                  cal=cal+(cal1+cal2);
                  flag2=2;flag_add=1;
                  count2=0;cal1=0;cal2=0;
              }
              
            }
          
          /*按键“*”*/
          if(i_15==1)
            {
              if(flag_add==0)
              {
                  MULTIEDIT_AddText(hWIN,"*"); WM_Exec();
                  flag=1;flag1=0;
                  for(m=0;m<count2;m++)
                  {
                    cal2=cal2*0.1;
                  }
                  cal=cal+(cal1+cal2);
                  flag2=3;flag_add=1;
                  count2=0;cal1=0;cal2=0;

              }
            }
         
          /*按键“/”*/
          if(i_16==1)
            {
              if(flag_add==0)
              {
                  MULTIEDIT_AddText(hWIN,"/"); WM_Exec();
                  flag=1;flag1=0;
                  for(m=0;m<count2;m++)
                  {
                    cal2=cal2*0.1;
                  }
                  cal=cal+(cal1+cal2);
                  flag2=4;flag_add=1;
                  count2=0;cal1=0;cal2=0;

              }
            }
          
          /*清除键*/
          if(i_17==1)
            {
          //    MULTIEDIT_AddText(hWIN,"Clear"); 
              hWIN = MULTIEDIT_Create(25,30,260,40,hFrame,GUI_ID_OK,WM_CF_SHOW,MULTIEDIT_CF_AUTOSCROLLBAR_V,NULL,20);
              MULTIEDIT_SetFont(hWIN,&GUI_Font16B_ASCII);
              MULTIEDIT_SetTextAlign(hWIN,GUI_TA_RIGHT);
              MULTIEDIT_SetWrapWord(hWIN);
              WM_Exec();
              flag=1;flag1=0;cal1=0;cal2=0;cal=0;count2=0;flag2=0;
            }
          
          /*按键“=”，按下后根据flag1的值选择运算符并换行显示结果*/
          if(i_18==1)
            {
              if(flag1==0)
              {
                  MULTIEDIT_AddText(hWIN,"=\n");
                  /*
                  sprintf(str,"%d",cal1);
                  MULTIEDIT_AddText(hWIN,(const char*)str);
                  MULTIEDIT_AddText(hWIN,".");
                  sprintf(str,"%d",cal2);
                  MULTIEDIT_AddText(hWIN,(const char*)str);
                  */
                  for(m=0;m<count2;m++)
                  {
                    cal2=cal2*0.1;
                  }
                  
                  if(flag2==0)
                  {
                    cal=cal1+cal2;
                  }
                  if(flag2==1)
                  {
                    cal=cal+cal1+cal2;
                  }  
                  if(flag2==2)
                  {
                    cal=cal-cal1+cal2;
                  }   
                  if(flag2==3)
                  {
                    cal=cal*(cal1+cal2);
                  }   
                  if(flag2==4)
                  {
                    cal=cal/(cal1+cal2);
                  }   
                  /*sprintf(str,"%d",count2);*/
                  /*
                  str1=strcpy("%.",(char const*)count2);
                  str2=strcpy(str1,(char const*)"f");
                  sprintf(str,str2,cal);
                  */
                  sprintf(str,"%.10f",cal);
                  MULTIEDIT_AddText(hWIN,(const char*)str);
                  WM_Exec();
                  flag=1;flag1=1;flag2=0;flag_add=0;count2=0;cal1=0;cal2=0;
              }
              
            }
          
  }
}

