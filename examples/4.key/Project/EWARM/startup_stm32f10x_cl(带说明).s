;******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
;* File Name          : startup_stm32f10x_cl.s
;* Author             : MCD Application Team
;* Version            : V3.5.0
;* Date               : 11-March-2011
;* Description        : STM32F10x Connectivity line devices vector table for 
;*                      EWARM toolchain.
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Configure the clock system
;*                      - Set the initial PC == __iar_program_start,
;*                      - Set the vector table entries with the exceptions ISR 
;*                        address.
;*                      After Reset the Cortex-M3 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;********************************************************************************
;* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
;* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
;* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
;* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
;* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
;* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;*******************************************************************************
;
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;
//主要参考《IAR_EWARM_user_manual.pdf》以下简称《IAR手册》
        MODULE  ?cstartup//定义一个库模块的起点 见《IAR手册》P66
//SECTION-段声明(也可用RSEG)  .intvec-复位及中断向量  CODE-代码段  DATA-数据段
//NOROOT表示如果这个段里的标号没引用就被linker舍弃，ROOT则一定不舍弃
//(2)表示字节对齐数，为2的幂，(2)表示4字节对齐2^2，(3)表示8字节对齐2^3
//定义各种模式STACK，具体大小位置是由linker文件(相当于ADS中的分散加载)决定的，之后在详细介绍。
//这些STACK都定义在内部RAM中，以CSTACK为例，地址为0x40000040
        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3) //《IAR手册》P63 定义堆栈段数据(向下生长) DATA 属性NOROOT (3）8字节对齐

        SECTION .intvec:CODE:NOROOT(2)//定义复位及中断向量代码 CODE  4字节对齐

        EXTERN  __iar_program_start//当前符号在其它源文件中定义，是一个系统定义的标号，在自带的C库启动代码当中，IAR会自动连接
        EXTERN  SystemInit        //当前符号在其它源文件中定义 在system_stm32f10x.c中
        PUBLIC  __vector_table//定义一个可以在其他文件中使用的全局标号。

//中断向量独立在一个叫 .intvec 的段当中，这个段是 4字节对齐（2^2）
//所以用 DATA 来首先处理向量的入口地址为 4的倍数，然后放向量表。
//需要注意的是 NOROOT(2) 和 ROOT(2) 的区别，
//如果NOROOT表示如果符号没有被关联的时候是被优化掉的，如果想不被优化则使用ROOT
//向量表的第一个数据为堆栈的顶部，可以为一个确定的数值放在SRAM的尾部，
//然后通过 SFE 运算得到该段的结束地址，注意这个运算是在link的时候完成。
//所以IAR 的处理办法就是先定义一个block作为堆栈区，然后将堆栈指针放到这个block的末端，因为STM32的堆栈是向下生长的。
        DATA //定义数据段
__vector_table//标号 异常向量表入口地址 标号主要用于表示一片内存空间的某个位置，等价于C 语言中的“地址”概念。
//作用是开辟一段空间，其意义等价于C语言中的地址符“&”。因此从这
//行开始建立的中断向量表则类似于使用C 语言定义了一个指针数组，其每一个成员都是个函数指针，分别指向各个中断服务函数。

        DCD     sfe(CSTACK) //给标号sfe(CSTAKE)分配4字节32位的地址0x00 ARM Cortex-M3权威指南P43
        DCD     Reset_Handler             ; Reset Handler //给标号Reset_Handler分配4字节32位的地址0x04
        DCD     NMI_Handler               ; NMI Handler
        DCD     HardFault_Handler         ; Hard Fault Handler
        DCD     MemManage_Handler         ; MPU Fault Handler
        DCD     BusFault_Handler          ; Bus Fault Handler
        DCD     UsageFault_Handler        ; Usage Fault Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SVC_Handler               ; SVCall Handler
        DCD     DebugMon_Handler          ; Debug Monitor Handler
        DCD     0                         ; Reserved
        DCD     PendSV_Handler            ; PendSV Handler
        DCD     SysTick_Handler           ; SysTick Handler

         ; External Interrupts
        DCD     WWDG_IRQHandler            ; Window Watchdog
        DCD     PVD_IRQHandler             ; PVD through EXTI Line detect
        DCD     TAMPER_IRQHandler          ; Tamper
        DCD     RTC_IRQHandler             ; RTC
        DCD     FLASH_IRQHandler           ; Flash
        DCD     RCC_IRQHandler             ; RCC
        DCD     EXTI0_IRQHandler           ; EXTI Line 0
        DCD     EXTI1_IRQHandler           ; EXTI Line 1
        DCD     EXTI2_IRQHandler           ; EXTI Line 2
        DCD     EXTI3_IRQHandler           ; EXTI Line 3
        DCD     EXTI4_IRQHandler           ; EXTI Line 4
        DCD     DMA1_Channel1_IRQHandler   ; DMA1 Channel 1
        DCD     DMA1_Channel2_IRQHandler   ; DMA1 Channel 2
        DCD     DMA1_Channel3_IRQHandler   ; DMA1 Channel 3
        DCD     DMA1_Channel4_IRQHandler   ; DMA1 Channel 4
        DCD     DMA1_Channel5_IRQHandler   ; DMA1 Channel 5
        DCD     DMA1_Channel6_IRQHandler   ; DMA1 Channel 6
        DCD     DMA1_Channel7_IRQHandler   ; DMA1 Channel 7
        DCD     ADC1_2_IRQHandler          ; ADC1 and ADC2
        DCD     CAN1_TX_IRQHandler         ; CAN1 TX
        DCD     CAN1_RX0_IRQHandler        ; CAN1 RX0
        DCD     CAN1_RX1_IRQHandler        ; CAN1 RX1
        DCD     CAN1_SCE_IRQHandler        ; CAN1 SCE
        DCD     EXTI9_5_IRQHandler         ; EXTI Line 9..5
        DCD     TIM1_BRK_IRQHandler        ; TIM1 Break
        DCD     TIM1_UP_IRQHandler         ; TIM1 Update
        DCD     TIM1_TRG_COM_IRQHandler    ; TIM1 Trigger and Commutation
        DCD     TIM1_CC_IRQHandler         ; TIM1 Capture Compare
        DCD     TIM2_IRQHandler            ; TIM2
        DCD     TIM3_IRQHandler            ; TIM3
        DCD     TIM4_IRQHandler            ; TIM4
        DCD     I2C1_EV_IRQHandler         ; I2C1 Event
        DCD     I2C1_ER_IRQHandler         ; I2C1 Error
        DCD     I2C2_EV_IRQHandler         ; I2C2 Event
        DCD     I2C2_ER_IRQHandler         ; I2C1 Error
        DCD     SPI1_IRQHandler            ; SPI1
        DCD     SPI2_IRQHandler            ; SPI2
        DCD     USART1_IRQHandler          ; USART1
        DCD     USART2_IRQHandler          ; USART2
        DCD     USART3_IRQHandler          ; USART3
        DCD     EXTI15_10_IRQHandler       ; EXTI Line 15..10
        DCD     RTCAlarm_IRQHandler        ; RTC alarm through EXTI line
        DCD     OTG_FS_WKUP_IRQHandler     ; USB OTG FS Wakeup through EXTI line
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     TIM5_IRQHandler            ; TIM5
        DCD     SPI3_IRQHandler            ; SPI3
        DCD     UART4_IRQHandler           ; UART4
        DCD     UART5_IRQHandler           ; UART5
        DCD     TIM6_IRQHandler            ; TIM6
        DCD     TIM7_IRQHandler            ; TIM7
        DCD     DMA2_Channel1_IRQHandler   ; DMA2 Channel1
        DCD     DMA2_Channel2_IRQHandler   ; DMA2 Channel2
        DCD     DMA2_Channel3_IRQHandler   ; DMA2 Channel3
        DCD     DMA2_Channel4_IRQHandler   ; DMA2 Channel4
        DCD     DMA2_Channel5_IRQHandler   ; DMA2 Channel5
        DCD     ETH_IRQHandler             ; Ethernet
        DCD     ETH_WKUP_IRQHandler        ; Ethernet Wakeup through EXTI line
        DCD     CAN2_TX_IRQHandler         ; CAN2 TX
        DCD     CAN2_RX0_IRQHandler        ; CAN2 RX0
        DCD     CAN2_RX1_IRQHandler        ; CAN2 RX1
        DCD     CAN2_SCE_IRQHandler        ; CAN2 SCE
        DCD     OTG_FS_IRQHandler          ; USB OTG FS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
//切换回THUMB模式
//PUBWEAK 是由2个单词组成，PUBLIC 和 WEAK ，PUBLIC 表示导出标号，WEAK表示弱属性，
//这个属性有一个好处就是，如果用户程序定义了这个标号，那么系统自带的这个中断处理程序则丢弃，
//使用用户的处理程序，如果用户没有定义则使用系统默认的中断处理程序，这样处理给用户有足够的自由。
        THUMB//16位THUMB指令

        PUBWEAK Reset_Handler  //声明Reset_Handler为全局变量
        SECTION .text:CODE:REORDER(2)//见《IAR手册》P63 定义代码段 .text为段名 CODE为代码 REORDER为属性 
        //:REORDER 允许连接器重新为所有的段排序（2）地址对齐值的指数，范围0-30。例如，如果Align = 1，对齐值就是字对齐(2)。
        
//LDR指令：将指定地址（低12位）上数据加载到寄存器
//LDR指令中如果有“=”号，就是不是标准指令，可能是1条或2条指令的组合（由编译器自动翻译）
//BLX：将当前指令的下一条指令保存到LR中，PC = *R0 + 1 （+1是使地址符合thumb2的规范）
//BLX相当于调用子程序指令；BX相当于直接跳转指令

Reset_Handler  //标号 
        LDR     R0, =SystemInit//装载 SystemInit 函数的地址，跳转去执行该函数，这个函数主要是设置系统的时钟等初始化。
        BLX     R0   //转移 见《权威指南》P47
        LDR     R0, =__iar_program_start//__iar_program_start IAR提供的"内部函数" 转到复位向量处
        BX      R0
//__iar_program_start内容，实际上就是系统复位后首先运行的代码
//这个函数是在启动代码中 \arm\src\lib\thumb\下面的代码中找到，
//是系统自带的启动模块。在项目设置属性的 linker，library中勾选 Automatic runtime library 就可以自动装载了，
//然后在汇编代码中声明一下
        
        
//PUBWEAK 是由2个单词组成，PUBLIC 和 WEAK ，PUBLIC 表示导出标号，WEAK表示弱属性，
//这个属性有一个好处就是，如果用户程序定义了这个标号，那么系统自带的这个中断处理程序则丢弃，
//使用用户的处理程序，如果用户没有定义则使用系统默认的中断处理程序，这样处理给用户有足够的自由。        
        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER(1)
NMI_Handler
        B NMI_Handler  //B 为条件转移 见 《权威指南》P47

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER(1)
MemManage_Handler
        B MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER(1)
UsageFault_Handler
        B UsageFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER(1)
DebugMon_Handler
        B DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK WWDG_IRQHandler
        SECTION .text:CODE:REORDER(1)
WWDG_IRQHandler
        B WWDG_IRQHandler

        PUBWEAK PVD_IRQHandler
        SECTION .text:CODE:REORDER(1)
PVD_IRQHandler
        B PVD_IRQHandler

        PUBWEAK TAMPER_IRQHandler
        SECTION .text:CODE:REORDER(1)
TAMPER_IRQHandler
        B TAMPER_IRQHandler

        PUBWEAK RTC_IRQHandler
        SECTION .text:CODE:REORDER(1)
RTC_IRQHandler
        B RTC_IRQHandler

        PUBWEAK FLASH_IRQHandler
        SECTION .text:CODE:REORDER(1)
FLASH_IRQHandler
        B FLASH_IRQHandler

        PUBWEAK RCC_IRQHandler
        SECTION .text:CODE:REORDER(1)
RCC_IRQHandler
        B RCC_IRQHandler

        PUBWEAK EXTI0_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI0_IRQHandler
        B EXTI0_IRQHandler

        PUBWEAK EXTI1_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI1_IRQHandler
        B EXTI1_IRQHandler

        PUBWEAK EXTI2_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI2_IRQHandler
        B EXTI2_IRQHandler

        PUBWEAK EXTI3_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI3_IRQHandler
        B EXTI3_IRQHandler


        PUBWEAK EXTI4_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI4_IRQHandler
        B EXTI4_IRQHandler

        PUBWEAK DMA1_Channel1_IRQHandler
        SECTION .text:CODE:REORDER(1)
DMA1_Channel1_IRQHandler
        B DMA1_Channel1_IRQHandler

        PUBWEAK DMA1_Channel2_IRQHandler
        SECTION .text:CODE:REORDER(1)
DMA1_Channel2_IRQHandler
        B DMA1_Channel2_IRQHandler

        PUBWEAK DMA1_Channel3_IRQHandler
        SECTION .text:CODE:REORDER(1)
DMA1_Channel3_IRQHandler
        B DMA1_Channel3_IRQHandler

        PUBWEAK DMA1_Channel4_IRQHandler
        SECTION .text:CODE:REORDER(1)
DMA1_Channel4_IRQHandler
        B DMA1_Channel4_IRQHandler

        PUBWEAK DMA1_Channel5_IRQHandler
        SECTION .text:CODE:REORDER(1)
DMA1_Channel5_IRQHandler
        B DMA1_Channel5_IRQHandler

        PUBWEAK DMA1_Channel6_IRQHandler
        SECTION .text:CODE:REORDER(1)
DMA1_Channel6_IRQHandler
        B DMA1_Channel6_IRQHandler

        PUBWEAK DMA1_Channel7_IRQHandler
        SECTION .text:CODE:REORDER(1)
DMA1_Channel7_IRQHandler
        B DMA1_Channel7_IRQHandler

        PUBWEAK ADC1_2_IRQHandler
        SECTION .text:CODE:REORDER(1)
ADC1_2_IRQHandler
        B ADC1_2_IRQHandler

        PUBWEAK CAN1_TX_IRQHandler
        SECTION .text:CODE:REORDER(1)
CAN1_TX_IRQHandler
        B CAN1_TX_IRQHandler

        PUBWEAK CAN1_RX0_IRQHandler
        SECTION .text:CODE:REORDER(1)
CAN1_RX0_IRQHandler
        B CAN1_RX0_IRQHandler

        PUBWEAK CAN1_RX1_IRQHandler
        SECTION .text:CODE:REORDER(1)
CAN1_RX1_IRQHandler
        B CAN1_RX1_IRQHandler

        PUBWEAK CAN1_SCE_IRQHandler
        SECTION .text:CODE:REORDER(1)
CAN1_SCE_IRQHandler
        B CAN1_SCE_IRQHandler

        PUBWEAK EXTI9_5_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI9_5_IRQHandler
        B EXTI9_5_IRQHandler

        PUBWEAK TIM1_BRK_IRQHandler
        SECTION .text:CODE:REORDER(1)
TIM1_BRK_IRQHandler
        B TIM1_BRK_IRQHandler

        PUBWEAK TIM1_UP_IRQHandler
        SECTION .text:CODE:REORDER(1)
TIM1_UP_IRQHandler
        B TIM1_UP_IRQHandler

        PUBWEAK TIM1_TRG_COM_IRQHandler
        SECTION .text:CODE:REORDER(1)
TIM1_TRG_COM_IRQHandler
        B TIM1_TRG_COM_IRQHandler

        PUBWEAK TIM1_CC_IRQHandler
        SECTION .text:CODE:REORDER(1)
TIM1_CC_IRQHandler
        B TIM1_CC_IRQHandler

        PUBWEAK TIM2_IRQHandler
        SECTION .text:CODE:REORDER(1)
TIM2_IRQHandler
        B TIM2_IRQHandler

        PUBWEAK TIM3_IRQHandler
        SECTION .text:CODE:REORDER(1)
TIM3_IRQHandler
        B TIM3_IRQHandler

        PUBWEAK TIM4_IRQHandler
        SECTION .text:CODE:REORDER(1)
TIM4_IRQHandler
        B TIM4_IRQHandler

        PUBWEAK I2C1_EV_IRQHandler
        SECTION .text:CODE:REORDER(1)
I2C1_EV_IRQHandler
        B I2C1_EV_IRQHandler

        PUBWEAK I2C1_ER_IRQHandler
        SECTION .text:CODE:REORDER(1)
I2C1_ER_IRQHandler
        B I2C1_ER_IRQHandler

        PUBWEAK I2C2_EV_IRQHandler
        SECTION .text:CODE:REORDER(1)
I2C2_EV_IRQHandler
        B I2C2_EV_IRQHandler

        PUBWEAK I2C2_ER_IRQHandler
        SECTION .text:CODE:REORDER(1)
I2C2_ER_IRQHandler
        B I2C2_ER_IRQHandler

        PUBWEAK SPI1_IRQHandler
        SECTION .text:CODE:REORDER(1)
SPI1_IRQHandler
        B SPI1_IRQHandler    

        PUBWEAK SPI2_IRQHandler
        SECTION .text:CODE:REORDER(1)
SPI2_IRQHandler
        B SPI2_IRQHandler

        PUBWEAK USART1_IRQHandler
        SECTION .text:CODE:REORDER(1)
USART1_IRQHandler
        B USART1_IRQHandler

        PUBWEAK USART2_IRQHandler
        SECTION .text:CODE:REORDER(1)
USART2_IRQHandler
        B USART2_IRQHandler

        PUBWEAK USART3_IRQHandler
        SECTION .text:CODE:REORDER(1)
USART3_IRQHandler
        B USART3_IRQHandler

        PUBWEAK EXTI15_10_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI15_10_IRQHandler
        B EXTI15_10_IRQHandler

        PUBWEAK RTCAlarm_IRQHandler
        SECTION .text:CODE:REORDER(1)
RTCAlarm_IRQHandler
        B RTCAlarm_IRQHandler

        PUBWEAK OTG_FS_WKUP_IRQHandler
        SECTION .text:CODE:REORDER(1)
OTG_FS_WKUP_IRQHandler
        B OTG_FS_WKUP_IRQHandler

        PUBWEAK TIM5_IRQHandler
        SECTION .text:CODE:REORDER(1)
TIM5_IRQHandler
        B TIM5_IRQHandler

        PUBWEAK SPI3_IRQHandler
        SECTION .text:CODE:REORDER(1)
SPI3_IRQHandler
        B SPI3_IRQHandler

        PUBWEAK UART4_IRQHandler
        SECTION .text:CODE:REORDER(1)
UART4_IRQHandler
        B UART4_IRQHandler

        PUBWEAK UART5_IRQHandler
        SECTION .text:CODE:REORDER(1)
UART5_IRQHandler
        B UART5_IRQHandler

        PUBWEAK TIM6_IRQHandler
        SECTION .text:CODE:REORDER(1)
TIM6_IRQHandler
        B TIM6_IRQHandler

        PUBWEAK TIM7_IRQHandler
        SECTION .text:CODE:REORDER(1)
TIM7_IRQHandler
        B TIM7_IRQHandler

        PUBWEAK DMA2_Channel1_IRQHandler
        SECTION .text:CODE:REORDER(1)
DMA2_Channel1_IRQHandler
        B DMA2_Channel1_IRQHandler

        PUBWEAK DMA2_Channel2_IRQHandler
        SECTION .text:CODE:REORDER(1)
DMA2_Channel2_IRQHandler
        B DMA2_Channel2_IRQHandler

        PUBWEAK DMA2_Channel3_IRQHandler
        SECTION .text:CODE:REORDER(1)
DMA2_Channel3_IRQHandler
        B DMA2_Channel3_IRQHandler

        PUBWEAK DMA2_Channel4_IRQHandler
        SECTION .text:CODE:REORDER(1)
DMA2_Channel4_IRQHandler
        B DMA2_Channel4_IRQHandler

        PUBWEAK DMA2_Channel5_IRQHandler
        SECTION .text:CODE:REORDER(1)
DMA2_Channel5_IRQHandler
        B DMA2_Channel5_IRQHandler

        PUBWEAK ETH_IRQHandler
        SECTION .text:CODE:REORDER(1)
ETH_IRQHandler
        B ETH_IRQHandler

        PUBWEAK ETH_WKUP_IRQHandler
        SECTION .text:CODE:REORDER(1)
ETH_WKUP_IRQHandler
        B ETH_WKUP_IRQHandler

        PUBWEAK CAN2_TX_IRQHandler
        SECTION .text:CODE:REORDER(1)
CAN2_TX_IRQHandler
        B CAN2_TX_IRQHandler

        PUBWEAK CAN2_RX0_IRQHandler
        SECTION .text:CODE:REORDER(1)
CAN2_RX0_IRQHandler
        B CAN2_RX0_IRQHandler

        PUBWEAK CAN2_RX1_IRQHandler
        SECTION .text:CODE:REORDER(1)
CAN2_RX1_IRQHandler
        B CAN2_RX1_IRQHandler

        PUBWEAK CAN2_SCE_IRQHandler
        SECTION .text:CODE:REORDER(1)
CAN2_SCE_IRQHandler
        B CAN2_SCE_IRQHandler

        PUBWEAK OTG_FS_IRQHandler
        SECTION .text:CODE:REORDER(1)
OTG_FS_IRQHandler
        B OTG_FS_IRQHandler

        END//定义一个库模块的结束
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
