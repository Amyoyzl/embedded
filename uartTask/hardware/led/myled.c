/*
 myled.c  LED驱动
 LED0 - PF9
 LED1 - PF10
*/

#include "sys.h"
#include "stm32f4xx.h"

#define LED0 PFout(9)  // 定义LED0为 PF数据寄存器的第9位
#define LED1 PFout(10)

/*
 初始化LED
 通用输出工作模式
 输出类型设置为推挽输出 上拉电阻使能
*/
void initLedGpio(){
	
	// 打开GPIO的时钟信号
	// 使能端，第五位设为1
	RCC->AHB1ENR |= (0x1<<5);
	//配置GPIO口的工作方式
	//端口名称、引脚、工作模式、推挽输出、工作速度、上拉电阻
	GPIO_Set(GPIOF, PIN9|PIN10, GPIO_MODE_OUT, GPIO_OTYPE_PP, 
						GPIO_SPEED_100M, GPIO_PUPD_PU);
	
	LED0 = 1;  // 初始化LED灭
	LED1 = 1;
	
}

/*
 打开和关闭LED灯
 ctl = 0 打开
*/
void ledControl(int ctl){
	 LED0 = ctl;
	 LED1 = ctl;
}




