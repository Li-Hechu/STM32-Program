# include "stm32f10x.h"
# include "core_cm3.h"
// stm32f10x.h 必须在 core_cm3.h 前面，否则出错
# include "stm32f10x_gpio.h"


int main(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;  //必须在第一个位置，否则出错
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
  GPIO_SetBits(GPIOB,GPIO_Pin_7);
}

