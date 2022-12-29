#include <stdint.h>
#include "main.h"

#define USART_REC_LEN 200 //定义最大接收字节数 200
#define RXBUFFERSIZE 1    //缓存大小

uint8_t USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
// bit15，	接收完成标志
// bit14，	接收到0x0d
// bit13~0，	接收到的有效字节数目
uint16_t USART_RX_STA = 0;       //接收状态标记
uint8_t aRxBuffer[RXBUFFERSIZE]; // HAL库使用的串口接收缓冲

//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout; 

//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0)
        ; //循环发送,直到发送完毕
    USART1->DR = (uint8_t)ch;
    return ch;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) //如果是串口1
    {
        if ((USART_RX_STA & 0x8000) == 0) //接收未完成
        {
            if (USART_RX_STA & 0x4000) //接收到了0x0d
            {
                if (aRxBuffer[0] != 0x0a)
                    USART_RX_STA = 0; //接收错误,重新开始
                else
                    USART_RX_STA |= 0x8000; //接收完成了
            }
            else //还没收到0X0D
            {
                if (aRxBuffer[0] == 0x0d)
                    USART_RX_STA |= 0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = aRxBuffer[0];
                    USART_RX_STA++;
                    if (USART_RX_STA > (USART_REC_LEN - 1))
                        USART_RX_STA = 0; //接收数据错误,重新开始接收
                }
            }
        }
    }
}

//串口1中断服务程序
void USART1_IRQHandler(void)
{
    uint32_t timeout = 0;

    HAL_UART_IRQHandler(&huart1); //调用HAL库中断处理公用函数

    timeout = 0;
    while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY) //等待就绪
    {
        timeout++; ////超时处理
        if (timeout > HAL_MAX_DELAY)
            break;
    }

    timeout = 0;
    while (HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK) //一次处理完成之后，重新开启中断并设置RxXferCount为1
    {
        timeout++; //超时处理
        if (timeout > HAL_MAX_DELAY)
            break;
    }
}
