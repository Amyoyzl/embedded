/*
	main.c
*/
#include "sys.h"
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "string.h"
#include "lcd.h"

#define LED_ON  0
#define LED_OFF 1

#define ON  "on"
#define OFF "off"

void initLedGpio(void);
void ledControl(int ctl);
int cmp(u8* str1, const char* str2);
char testbuf[] = "please input your control instruction(on/off):\r\n";

int main(void){
	// 配置系统的工作时钟
	/*
	plln = 336
	pllm = 8
	pllp = 2
	*/
	Stm32_Clock_Init(336,8,2,7);
	delay_init(168); // 用延时初始化函数
	uart_init(84,115200);  //初始化串口1
	
	uart2_init(42,115200);//初始化串口2
	
	// 初始化GPIO
	initLedGpio();
	
	// 初始化LCD
	LCD_Init();
	
	// 清屏
	LCD_Clear(WHITE);
	// 调用LCD屏驱动API函数来显示字符
	POINT_COLOR = BLUE;
	// 显示一行字符串
	LCD_ShowString(40,50,400,24,24,"your instruction:");
	LCD_DrawRectangle(30,100,450,750);
	
	memset(uart2RecvBuf,0,UART_BUF_SIZE);
	uart2RecvLen = 0;
	
	uart2SendBuf((u8 *)testbuf,strlen(testbuf));
	
//	printf("Hello my first stmf4 program\r\n\n");
	
	while(1){
		
		// uart	
		if(uart2RecvLen>0){
			delay_ms(200);
			printf("uart2 recv : %s\r\n\r\n",uart2RecvBuf);
			
			LCD_ShowString(45,130,400,24,24,"                    ");
			LCD_ShowString(45,130,400,24,24,uart2RecvBuf);
			
			// 控制LED
			if(cmp(uart2RecvBuf, ON)==0)
					ledControl(LED_ON);
			else if ((cmp(uart2RecvBuf, OFF))==0)
					ledControl(LED_OFF);
			else
					printf("input error! (on/off)\r\n\r\n");
			
			memset(uart2RecvBuf,0,UART_BUF_SIZE);
			uart2RecvLen = 0;
		}
		delay_ms(500);
	}
	
}


int cmp(u8* str1, const char* str2){
		int len = strlen(str2);
		int i = 0;
		for(i=0; i < len; i++){
				if(str1[i]!=str2[i])
						return -1;
		}
		return 0;
}
