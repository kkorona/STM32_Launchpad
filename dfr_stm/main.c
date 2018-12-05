/*
 * main.c
 *
 *  Created on: 2015. 12. 01.
 *      Author: Lee Sangjin
 */
//Includes
#include "main.h"

/*******************************************************************************
* Function Name  : PUTCHAR_PROTOTYPE
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

#ifdef __GNUC__
  // With GCC/RAISONANCE, small printf
  // (option LD Linker->Libraries->Small printf set to 'Yes')
  // calls __io_putchar()
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif // __GNUC__

PUTCHAR_PROTOTYPE
{
  // Write a character to the USART
  USART_SendData(USART1, (u8) ch);
  // Loop until the end of transmission
  while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
  
  return ch;
}

   
//Typedefs
//Defines

//Private Variables
uint8_t B_UID = 1;
bool Emergency = FALSE;
uint8_t EmergBuf[9] = "Emergency";

char ff = 0x0c; // clear screen

char chit = 0; //usart data
uint8_t buf[90] = {0x00, };
int buflen = 0;
uint8_t HQ[3] = {0xff, 0x00, 0x00};

char DFPlayer_Cmd[10] = {0x7E, 0xFF, 0x06, 0, 0, 0, 0, 0, 0, 0xEF};

//Main
int main() { 
  SystemInit();
  RccInit();
  GpioInit();
  UartInit();
  EXTIInit();
  LED_Init(GreenLED);
  LED_Init(BlueLED);
  ClearScreen();
  printf("\nSystem and RCC and GPIO and UART Initialize Complete\n");

  printf("\n================Buoy Firmware Start================\n");
  

  DFPlayer_Cmd[3] = (char)0x06;
  DFPlayer_Cmd[4] = (char)0x00;
  DFPlayer_Cmd[5] = (char)0x00;
  DFPlayer_Cmd[6] = (char)0x05;
  DFPlayer_Cmd[7] = (char)0xFE;
  DFPlayer_Cmd[8] = (char)0xF6;
  
  SendStr(USART3,DFPlayer_Cmd,10);
  
  Delay_ms(60);
  
  while (1) {
    if(!Emergency){
      LED_Off(GreenLED);
      /*
      DFPlayer_Cmd[3] = (char)0x03;
      DFPlayer_Cmd[4] = (char)0x00;
      DFPlayer_Cmd[5] = (char)0x00;
      DFPlayer_Cmd[6] = (char)0x00;
      DFPlayer_Cmd[7] = (char)0xFE;
      DFPlayer_Cmd[8] = (char)0xF8;
      
      SendStr(USART3,DFPlayer_Cmd,10);
      Delay_ms(60000);
      
      DFPlayer_Cmd[3] = (char)0x03;
      DFPlayer_Cmd[4] = (char)0x00;
      DFPlayer_Cmd[5] = (char)0x00;
      DFPlayer_Cmd[6] = (char)0x01;
      DFPlayer_Cmd[7] = (char)0xFE;
      DFPlayer_Cmd[8] = (char)0xF7;

      SendStr(USART3,DFPlayer_Cmd,10);
      Delay_ms(60000);
      
      DFPlayer_Cmd[3] = (char)0x03;
      DFPlayer_Cmd[4] = (char)0x00;
      DFPlayer_Cmd[5] = (char)0x00;
      DFPlayer_Cmd[6] = (char)0x02;
      DFPlayer_Cmd[7] = (char)0xFE;
      DFPlayer_Cmd[8] = (char)0xF6;

      SendStr(USART3,DFPlayer_Cmd,10);
      Delay_ms(60000);*/
    }
    else{
      LED_On(GreenLED);
      
      DFPlayer_Cmd[3] = (char)0x03;
      DFPlayer_Cmd[4] = (char)0x00;
      DFPlayer_Cmd[5] = (char)0x00;
      DFPlayer_Cmd[6] = (char)0x04;
      DFPlayer_Cmd[7] = (char)0xFE;
      DFPlayer_Cmd[8] = (char)0xF4;

      SendStr(USART3,DFPlayer_Cmd,10);
      HNSO02_SendAPP(HQ,EmergBuf,9);
      Delay_ms(10000);
    }
  }
}

//Function
void RccInit(void) {
  //USART2 enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  //USART3 enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  //AFIO enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  //GPIOA enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  //GPIOB enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  //USART1 enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}

void GpioInit(void) {
  GPIO_InitTypeDef GPIOA_Init;
  GPIO_InitTypeDef GPIOB_Init;
  GPIOA_Init.GPIO_Speed = GPIO_Speed_50MHz;
  GPIOB_Init.GPIO_Speed = GPIO_Speed_50MHz;
  
  // Initialize GPIOA Pin0
  GPIOA_Init.GPIO_Pin = GPIO_Pin_0; // BUTTON
  GPIOA_Init.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOA, &GPIOA_Init);

  // Initialize GPIOA Pin7
  GPIOA_Init.GPIO_Pin = GPIO_Pin_7; // BUTTON
  GPIOA_Init.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOA, &GPIOA_Init);
  
  // GPIO of UART2_Init
  /*
  // Initialize GPIOA Pin0
  GPIOA_Init.GPIO_Pin = GPIO_Pin_0; // UART2_CTS
  GPIOA_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIOA_Init);

  // Initialize GPIOA Pin1
  GPIOA_Init.GPIO_Pin = GPIO_Pin_1; // UART2_RTS
  GPIOA_Init.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIOA_Init);
  */
  
  // Initialize GPIOA Pin2
  GPIOA_Init.GPIO_Pin = GPIO_Pin_2; // UART2_TX
  GPIOA_Init.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIOA_Init);

  // Initialize GPIOA Pin3
  GPIOA_Init.GPIO_Pin = GPIO_Pin_3; // UART2_RX
  GPIOA_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIOA_Init);

  // GPIO of UART1_Init
  // Initialize GPIOA Pin9
  GPIOA_Init.GPIO_Pin = GPIO_Pin_9; // UART1_TX
  GPIOA_Init.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIOA_Init);

  // Initialize GPIOA Pin10
  GPIOA_Init.GPIO_Pin = GPIO_Pin_10; // UART1_RX
  GPIOA_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIOA_Init);

  // GPIO of UART3_Init
  // Initialize GPIOB Pin10
  GPIOB_Init.GPIO_Pin = GPIO_Pin_10; // UART3_TX
  GPIOB_Init.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIOB_Init);

  // Initialize GPIOB Pin11
  GPIOB_Init.GPIO_Pin = GPIO_Pin_11; // UART3_RX
  GPIOB_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIOB_Init);
}

void UartInit(void) {
  USART_InitTypeDef USART1_Init;
  USART_InitTypeDef USART2_Init;
  USART_InitTypeDef USART3_Init;

  NVIC_InitTypeDef NVIC1_InitStruct;
  NVIC_InitTypeDef NVIC2_InitStruct;
  NVIC_InitTypeDef NVIC3_InitStruct;

  // Initialize USART1
  USART1_Init.USART_BaudRate = 115200;
  USART1_Init.USART_WordLength = USART_WordLength_8b;
  USART1_Init.USART_StopBits = USART_StopBits_1;
  USART1_Init.USART_Parity = USART_Parity_No;
  USART1_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART1_Init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART1_Init);

  // Initialize USART2
  USART2_Init.USART_BaudRate = 115200;
  USART2_Init.USART_WordLength = USART_WordLength_8b;
  USART2_Init.USART_StopBits = USART_StopBits_1;
  USART2_Init.USART_Parity = USART_Parity_No;
  USART2_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART2_Init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART2_Init);

  // Initialize USART3
  USART3_Init.USART_BaudRate = 9600;
  USART3_Init.USART_WordLength = USART_WordLength_8b;
  USART3_Init.USART_StopBits = USART_StopBits_1;
  USART3_Init.USART_Parity = USART_Parity_No;
  USART3_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART3_Init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART3_Init);

  // Initialize USART1 IRQ
  NVIC1_InitStruct.NVIC_IRQChannel = USART1_IRQn;
  NVIC1_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC1_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC1_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC1_InitStruct);

  // Initialize USART2 IRQ
  NVIC2_InitStruct.NVIC_IRQChannel = USART2_IRQn;
  NVIC2_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC2_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC2_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC2_InitStruct);
  
  // Initialize USART3 IRQ
  NVIC3_InitStruct.NVIC_IRQChannel = USART3_IRQn;
  NVIC3_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC3_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC3_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC3_InitStruct);

  // Enable USARTx Interrupt
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART1, ENABLE);
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART2, ENABLE);
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART3, ENABLE);
  
  ClearScreen();
  
  printf("5");
  Delay_ms(1000);
  printf("\b4");
  Delay_ms(1000);
  printf("\b3");
  Delay_ms(1000);
  printf("\b2");
  Delay_ms(1000);
  printf("\b1");
  Delay_ms(1000);
  printf("\b0");
  
  HNSO02_Init(USART2, &USART2_Init);
}

void EXTIInit(void){
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
  
  EXTI_InitStruct.EXTI_Line = EXTI_Line0;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStruct);
  
  NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
}

void OhLoRa_Cmd(){
  switch (RecvFrame[Frame_IDX_APP]){
  case 's':
  case 'S':
    Emergency = FALSE;
    break;
    
  case '0':
    if(!Emergency){
      LED_On(BlueLED);
      DFPlayer_Cmd[3] = (char)0x03;
      DFPlayer_Cmd[4] = (char)0x00;
      DFPlayer_Cmd[5] = (char)0x00;
      DFPlayer_Cmd[6] = (char)0x01;
      DFPlayer_Cmd[7] = (char)0xFE;
      DFPlayer_Cmd[8] = (char)0xF7;

      SendStr(USART3,DFPlayer_Cmd,10);
      Delay_ms(5000);
      LED_Off(BlueLED);
    }
    break;
    
  case '1':
    if(!Emergency){
      LED_On(BlueLED);
      DFPlayer_Cmd[3] = (char)0x03;
      DFPlayer_Cmd[4] = (char)0x00;
      DFPlayer_Cmd[5] = (char)0x00;
      DFPlayer_Cmd[6] = (char)0x02;
      DFPlayer_Cmd[7] = (char)0xFE;
      DFPlayer_Cmd[8] = (char)0xF6;
      
      SendStr(USART3,DFPlayer_Cmd,10);
      Delay_ms(5000);
      LED_Off(BlueLED);
    }
    break;
  case '2':
    if(!Emergency){
      LED_On(BlueLED);
      DFPlayer_Cmd[3] = (char)0x03;
      DFPlayer_Cmd[4] = (char)0x00;
      DFPlayer_Cmd[5] = (char)0x00;
      DFPlayer_Cmd[6] = (char)0x03;
      DFPlayer_Cmd[7] = (char)0xFE;
      DFPlayer_Cmd[8] = (char)0xF5;
      
      SendStr(USART3,DFPlayer_Cmd,10);
      Delay_ms(5000);
      LED_Off(BlueLED);
    }
    break;
  }
}

void SendData(USART_TypeDef* USARTx, char dat){
  USART_SendData(USARTx, dat);
  while (!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));
}

void SendStr(USART_TypeDef* USARTx, char* str, int len){
  int i = 0;
  for (i = 0; i < len; i++) {
    SendData(USARTx, str[i]);
  }
}

void Delay_ms(volatile unsigned int nTime) {
  volatile unsigned int clk = 0;
  while(nTime > 0){
    clk = 2400;
    while(clk > 0)clk--;
    nTime--;
  }
}

void ClearScreen(void){
  printf("%c",ff);
}

void CHKCMD(uint8_t chkch){
  switch (chkch){
  case 0x7f:
    if(buflen > 0){
      buflen--;
    }
    break;
  case '\r':
  case '\n':
    HNSO02_SendAPP(HQ,buf,buflen);
    buflen = 0;
    printf("OK\n");
    break;
  default:
    buf[buflen] = chkch;
    buflen++;
    break;
  }
}

//defined by stm32f10x_it.h
/**
  * @brief  This function handles USART1 Interrupt Handler.
  * @param  None
  * @retval None
  */

void USART1_IRQHandler(void) {
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    chit = USART_ReceiveData(USART1);

    USART_SendData(USART1, chit);
    CHKCMD(chit);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  }
}

/**
  * @brief  This function handles USART2 Interrupt Handler.
  * @param  None
  * @retval None
  */

void USART2_IRQHandler(void) {
  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    chit = USART_ReceiveData(USART2);
    CHKFrame(chit);
    //printf("0x%X\n",chit);
    //while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    //USART_SendData(USART1, chit);
    //Recv_Buf_Len++;
    //Recv_Buffer[Recv_Buf_Len] = chit;
    //if(chit == '\n') OhLoRa_Cmd();
  }
}

/**
  * @brief  This function handles USART3 Interrupt Handler.
  * @param  None
  * @retval None
  */

void USART3_IRQHandler(void) {
  if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    chit = USART_ReceiveData(USART3);
    
    printf("0x%02X\n",chit);
    //while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
    //USART_SendData(USART1, chit);
  }
}

/**
  * @brief  This function handles EXTI0 Interrupt Handler.
  * @param  None
  * @retval None
  */

void EXTI0_IRQHandler(void) {
  if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
    EXTI_ClearITPendingBit(EXTI_Line0);
    Emergency = TRUE;
  }
}