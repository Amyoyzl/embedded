/*
 串口驱动
*/

#include "sys.h"
#include "usart.h"
/*
 串口初始化函数，配置串口开始位、数据位、波特率
 串口2所用的引脚：
		USART2_TX   PA2
		USART2_RX   PA3
*/
void uart2_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV@OVER8=0
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分@OVER8=0 
    mantissa<<=4;
	mantissa+=fraction; 
	
	RCC->AHB1ENR|=1<<0;   	//使能PORTA口时钟  
	RCC->APB1ENR|=1<<17;		//使能串口2时钟 
	
	GPIO_Set(GPIOA,PIN2|PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);//PA2,PA3,复用功能,上拉输出 
 	GPIO_AF_Set(GPIOA,2,7);	
	GPIO_AF_Set(GPIOA,3,7); 	//PA3,AF7 
	
	//波特率设置
	 	USART2->BRR=mantissa; 	//波特率设置	 
		USART2->CR1&=~(1<<15); 	//设置OVER8=0 
		USART2->CR1|=1<<3;  	//串口发送使能 
	
	
	//使能接收中断 
		USART2->CR1|=1<<2;  	//串口接收使能
		USART2->CR1|=1<<5;    	//接收缓冲区非空中断使能	    	
		MY_NVIC_Init(3,3,USART2_IRQn,2);//组2，最低优先级	
	
		USART2->CR1|=1<<13;  	//串口使能
}

/*
	使用串口2发送函数
*/
void uart2Send(u8 data){
	
	while((USART2->SR & (1<<6))==0){
		; // 等待上次数据发送完成
	}
	
	USART2->DR = data;
}

void uart2SendBuf(u8 *buf, int len){
	int i = 0;
	for(i=0;i<len;i++){
		uart2Send(buf[i]);
	}
}

/*
	串口2接收 中断实现
	串口2中断服务处理程序
*/
u8 uart2RecvBuf[UART_BUF_SIZE] = {0};
u8 uart2RecvLen = 0;

void USART2_IRQHandler(void){
	u8 recv_data = 0;
	if(USART2->SR & (1<<5)){
		// 串口接收数据，存放在接收数据寄存器里
		recv_data = USART2->DR;
		uart2RecvBuf[uart2RecvLen] = recv_data;
		uart2RecvLen++;
	}
}

