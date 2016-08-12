/*********************2015-2016, NJUT, Edu.********************* 
FileName: cpal_user_i2c.c 
Author:  孙冬梅       Version :  1.0        Date: 2016.03.30
Description:    Cpal 调用测试用例      
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    16/05/30     1.0     文件创建   
***************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include "cpal_user_i2c.h"
#include "stdio.h"


/* CPAL local transfer structures */
CPAL_TransferTypeDef  sRxStructure, sTxStructure;

/* Buffers tables declarations */
uint8_t tTxBuffer[MAX_BUFF_SIZE]  = "STM32 CPAL 2xBoards Example: Signal State  ";

uint8_t tRxBuffer[MAX_BUFF_SIZE];
uint32_t BufferSize = BUFFER_SIZE;


/*******************************************************************************
* Function Name  : CPAL_I2c_Config
* Description    : CPAL_I2c 初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CPAL_I2c_Config(void)
{
  /* Start CPAL communication configuration ***********************************/
  /* Initialize local Reception structures */
  sRxStructure.wNumData = BufferSize;       /* Maximum Number of data to be received */
  sRxStructure.pbBuffer = tRxBuffer;        /* Common Rx buffer for all received data */
  sRxStructure.wAddr1 = 0;                  /* Not needed */
  sRxStructure.wAddr2 = 0;                  /* Not needed */
  
  /* Initialize local Transmission structures */
  sTxStructure.wNumData = BufferSize;       /* Maximum Number of data to be received */
  sTxStructure.pbBuffer = (uint8_t*)tTxBuffer;     /* Common Rx buffer for all received data */
  sTxStructure.wAddr1 = OWN_ADDRESS;        /* The own board address */
  sTxStructure.wAddr2 = 0;                  /* Not needed */  
 
  /* Configure the device structure */
  CPAL_I2C_StructInit(&I2C_DevStructure);      /* Set all fields to default values */
  I2C_DevStructure.CPAL_Mode = CPAL_MODE_SLAVE;
#ifdef CPAL_I2C_DMA_PROGMODEL
  I2C_DevStructure.wCPAL_Options =  CPAL_OPT_NO_MEM_ADDR;
  I2C_DevStructure.CPAL_ProgModel = CPAL_PROGMODEL_DMA;
#elif defined (CPAL_I2C_IT_PROGMODEL)
  I2C_DevStructure.wCPAL_Options =  CPAL_OPT_NO_MEM_ADDR;
  I2C_DevStructure.CPAL_ProgModel = CPAL_PROGMODEL_INTERRUPT;
#else
 #error "Please select one of the programming model (in main.h)"
#endif
  I2C_DevStructure.pCPAL_I2C_Struct->I2C_ClockSpeed = I2C_SPEED;
  I2C_DevStructure.pCPAL_I2C_Struct->I2C_OwnAddress1 = OWN_ADDRESS;
  I2C_DevStructure.pCPAL_TransferRx = &sRxStructure;
  I2C_DevStructure.pCPAL_TransferTx = &sTxStructure;
  
  /* Initialize CPAL device with the selected parameters */
  CPAL_I2C_Init(&I2C_DevStructure);       
}

/*******************************************************************************
* Function Name  : CPAL_I2c_Write
* Description    : CPAL_I2c 写入
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void User_I2c_Write(void)
{
         /* Configure the device mode to master */
        I2C_DevStructure.CPAL_Mode = CPAL_MODE_MASTER;
        /* Force the CPAL state to ready (in case a read operation has been initiated) */
        I2C_DevStructure.CPAL_State = CPAL_STATE_READY;
        
              /* Check if the current CPAL device state allows write operation */
      if ((I2C_DevStructure.CPAL_State == CPAL_STATE_READY) || \
        (I2C_DevStructure.CPAL_State == CPAL_STATE_READY_RX) ||\
          (I2C_DevStructure.CPAL_State == CPAL_STATE_DISABLED))
      {
        if (CPAL_I2C_Write(&I2C_DevStructure) != CPAL_PASS)
          {
            printf("\r\nWrite I2C failed!!\r\n");
          }
         else
            printf("\r\nWrite I2C success!!\r\n");
        printf("\r\n当前I2C 状态:%d\r\n",I2C_DevStructure.CPAL_State);   
      }
}

/*******************************************************************************
* Function Name  : User_I2c_Read
* Description    : CPAL_I2c 读取
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void User_I2c_Read(void)
{
      sRxStructure.wNumData = BufferSize;       /* Maximum Number of data to be received */
      sRxStructure.pbBuffer = tRxBuffer;        /* Common Rx buffer for all received data */
      
      /* Reconfigure device for slave receiver mode */
      I2C_DevStructure.CPAL_Mode = CPAL_MODE_SLAVE;
      I2C_DevStructure.CPAL_State = CPAL_STATE_READY;
      
      /* Start waiting for data to be received in slave mode */
      if (CPAL_I2C_Read(&I2C_DevStructure) == CPAL_PASS)
      {
        printf("\r\n当前I2C 状态:%d, 接收缓冲区数据：\r\n",I2C_DevStructure.CPAL_State);  
        printf(tRxBuffer);
      }
}