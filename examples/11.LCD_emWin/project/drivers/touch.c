/*******************************2012-2014, NJUT, Edu.************************** 
FileName: touchpanel.c 
Author:  孙冬梅       Version :  1.0        Date: 2014.05.30
Description:    触摸屏驱动 采用模拟SPI方式     
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    14/05/30     1.0     文件创建   
  *          STM32F107 Board Key Pin assignment
  *          =============================
  *          +-----------------------------------------------------------+
  *          |                     Pin assignment                        |
  *          +-----------------------------+-----------------------------+
  *          |       TP_Pin                |     Port & Pin              |
  *          +-----------------------------+-----------------------------+
  *          |        CS                   |        C8                   |
  *          |        INT                  |        C6                   |
  *          |        MISO                 |        C11                  |
  *          |        MOSI                 |        C12                  |
  *          |        SCLK                 |        C10                  |
  *          +-----------------------------+-----------------------------+
 ******************************************************************************/ 

#include "touch.h"
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "lcd_ssd1289.h"
#include "usart.h"


#define	CHX 	0x90            //通道Y+的选择控制字	
#define	CHY 	0xD0            //通道X+的选择控制字	


#define TP_SPI         SPI3

/* SPI3 pins definition  */
#define SPI3_PORT   GPIOC
#define SPI3_SCK    GPIO_Pin_10
#define SPI3_MISO   GPIO_Pin_11  
#define SPI3_MOSI   GPIO_Pin_12
#define RCC_SPI3    RCC_APB1Periph_SPI3
#define RCC_SPI3_GPIO    RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO 

/* Touch Panel SSD1289_TouchScreen */
#define RCC_TP_CS       RCC_APB2Periph_GPIOC	| RCC_APB2Periph_AFIO
#define TP_PORT_INT     GPIOC  
#define TP_PIN_INT      GPIO_Pin_6 
#define RCC_TP_INT      RCC_APB2Periph_GPIOC	| RCC_APB2Periph_AFIO
#define TP_PORT_CS      GPIOC 
#define TP_PIN_CS       GPIO_Pin_8 

#define TP_EXTI_LINE       EXTI_Line6
#define TP_EXTI_PORT_SOURCE         GPIO_PortSourceGPIOC
#define TP_EXTI_PIN_SOURCE      GPIO_PinSource6
#define TP_EXTI_IRQn       EXTI9_5_IRQn 


/*管脚置位复位宏定义 */
/* Select TP: ChipSelect pin low  */
#define TP_CS_LOW()     GPIO_ResetBits(TP_PORT_CS, TP_PIN_CS)
/* Deselect TP: ChipSelect pin high */
#define TP_CS_HIGH()    GPIO_SetBits(TP_PORT_CS, TP_PIN_CS)
/* SPI clk pin low  */
#define SPI3_SCK_LOW()     GPIO_ResetBits(SPI3_PORT, SPI3_SCK)
/* SPI clk pin high */
#define SPI3_SCK_HIGH()    GPIO_SetBits(SPI3_PORT, SPI3_SCK)
/* SPI pin read  */
#define SPI3_MISO_READ()     GPIO_ReadInputDataBit(SPI3_PORT, SPI3_MISO)
/* SPI pin low  */
#define SPI3_MOSI_LOW()     GPIO_ResetBits(SPI3_PORT, SPI3_MOSI)
/* SPI pin high */
#define SPI3_MOSI_HIGH()    GPIO_SetBits(SPI3_PORT, SPI3_MOSI)

/*******************************************************************************
*  Function Name  :   TP_CS_config （内部函数）
*  Description    : 触摸屏CS管脚配置
*  Input          : None
*  Output         : None
*  Return         : None
*******************************************************************************/
static void TP_CS_config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOB, GPIOC and AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_TP_CS , ENABLE);  
  
  /* CS pins configuration */
  GPIO_InitStructure.GPIO_Pin = TP_PIN_CS;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(TP_PORT_CS, &GPIO_InitStructure);
}

/*******************************************************************************
*  Function Name  : 　TP_INT_config （内部函数）
*  Description    : 触摸屏INT管脚配置
*  Input          : None
*  Output         : None
*  Return         : None
*******************************************************************************/
static void TP_INT_config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable GPIOB, GPIOC and AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_TP_INT , ENABLE);  
  
  /* INT pins configuration */
  GPIO_InitStructure.GPIO_Pin = TP_PIN_INT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   /*上拉输入，默认是1*/
  GPIO_Init(TP_PORT_INT, &GPIO_InitStructure);


  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6); 

  /* Configure Button EXTI line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line6;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  
  /* Set the Vector Table base address at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
  
  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the EXTI5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TP_EXTI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : SPI3_Config （内部函数）
* Description    : 触摸屏SPI管脚配置
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static  void SPI3_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
    
  /* 开启 外设时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO ,ENABLE);
   
  GPIO_InitStructure.GPIO_Pin = SPI3_SCK | SPI3_MOSI;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SPI3_PORT, &GPIO_InitStructure);
  /*/MISO pins configuration */
  GPIO_InitStructure.GPIO_Pin = SPI3_MISO;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(SPI3_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : TPInit
* Description    : 触摸屏初始化函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void touch_config(void)
{
    TP_CS_config();     
    TP_CS_HIGH() ;     
    SPI3_Config();
    SPI3_MOSI_HIGH();
    SPI3_SCK_LOW();
    /*打开触摸屏中断后，按下屏幕将读数通过串口打印*/
    //TP_INT_config();
}
/*******************************************************************************
* Function Name  : TP_SPI_Start （内部函数）
* Description    : 启动SPI数据传输
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void TP_SPI_Start( void )
{
    TP_CS_HIGH();
    SPI3_MOSI_HIGH();
    SPI3_SCK_LOW();
    TP_CS_LOW();	      		
}

/*******************************************************************************
* Function Name  : SPI_MOSI（内部函数）
* Description    : SPI3_MOSI从高到低变换
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void SPI_MOSI(uint8_t data)
{
    if(data)
        SPI3_MOSI_HIGH();
    else
        SPI3_MOSI_LOW();
}

/*******************************************************************************
* Function Name  : TP_WrCmd （内部函数）
* Description    : 在时钟上升沿将数据打入SPI的MOSI管脚
* Input          : cmd - 输入SPI的数据
* Output         : None
* Return         : None
*******************************************************************************/
static void TP_WrCmd(uint8_t cmd)
{
    
    uint8_t buf, i;

    for( i = 0; i < 8; i++ )
    {
        buf = (cmd >> (7-i)) & 0x1 ;	//MSB在前,LSB在后
        SPI_MOSI(buf);	 	        //时钟上升沿锁存DIN 
        SPI3_SCK_HIGH();                //上升沿发送
        SPI3_SCK_LOW();	                //开始发送命令字
    }
}

/*******************************************************************************
* Function Name  : TP_Read （内部函数）
* Description    : 在时钟下降沿将SPI中的数据读出
* Input          : None
* Output         : None
* Return         : 返回读出的数据
*******************************************************************************/
static uint16_t TP_Read(void)
{

  uint16_t buf = 0 ;
  uint8_t i;

  SPI3_SCK_HIGH();              //wait busy
  SPI3_SCK_LOW();	
  for( i = 0; i < 12; i++ )
  {
          buf = buf << 1 ;
          SPI3_SCK_HIGH();      //下降沿接收
          if ( SPI3_MISO_READ() )	
          {
                  buf = buf + 1 ;
          }
          SPI3_SCK_LOW();			
  }

  for( i = 0; i < 3; i++ )
  {
          SPI3_SCK_HIGH();
          SPI3_SCK_LOW();			
  }

  return( buf ) ;
}

/*******************************************************************************
* Function Name  : touch_read_x
* Description    : 读x轴模拟值
* Input          : None
* Output         : None
* Return         : 模拟值
*******************************************************************************/
#define times 8
 uint16_t touch_read_x( void)
{
  uint16_t      X_Addata,i,j,k,x_addata[times];
  
  for(i=0;i<times;i++)  //采样4次.
  {
    TP_SPI_Start();
    TP_WrCmd( CHX ) ;   // 选择通道X，即返回值是X方向的AD坐标 
    x_addata[i] = TP_Read() ;
    TP_CS_HIGH(); 
  }

  for(i=0;i<times;i++)/* 冒泡法排序 */
  {
      for(j=times;j<times-1;j++)
      {
         if(x_addata[j] > x_addata[i])
          {
              k = x_addata[j];
              x_addata[j] = x_addata[i];
              x_addata[i] = k;
          }
       }
    }

  /* 求平均值*/
  X_Addata=(x_addata[1] + x_addata[2]) >> 1; 
  return X_Addata;
}

/*******************************************************************************
* Function Name  : touch_read_y
* Description    : 读y轴模拟值
* Input          : None
* Output         : None
* Return         : 模拟值
*******************************************************************************/
uint16_t touch_read_y(  void )
{
  uint16_t     Y_Addata, i,j,k,y_addata[times];
  
  for(i=0;i<times;i++)  //采样4次.
  {
    TP_SPI_Start();
    TP_WrCmd( CHY ) ;   // 选择通道Y，即返回的是Y方向的AD坐标 
    y_addata[i] = TP_Read() ;
    TP_CS_HIGH(); 
  }

  for(i=0;i<times;i++)/* 冒泡法排序 */
    {
      for(j=times;j<times-1;j++)
      {
         if(y_addata[j] > y_addata[i])
          {
            k = y_addata[j];
            y_addata[j] = y_addata[i];
            y_addata[i] = k;
          }
       }
    }

    /* 求平均值*/
  Y_Addata=(y_addata[1] + y_addata[2]) >> 1;
  return Y_Addata;
}


/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI9_5_IRQHandler(void) /* TouchScreen */
{
  if(EXTI_GetITStatus(EXTI_Line6) != RESET)
  {
   
    {
      UartPrintf("\r\n x: %d, y: %d",touch_read_x(),touch_read_y());
    }
    /* Clear the EXTI Line 6 */  
    EXTI_ClearITPendingBit(EXTI_Line6);
  }
}
