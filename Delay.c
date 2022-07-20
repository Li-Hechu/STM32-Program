# include "stm32f10x.h"
# include "core_cm3.h"

void delay_us (__IO uint32_t us) {
	
	uint32_t temp;
	SysTick->LOAD = 9*us;
	SysTick->VAL=0X00;
	SysTick->CTRL=0X01;
	
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&(!(temp&(1<<16))));
	
	SysTick->CTRL=0x00;
  SysTick->VAL =0X00;
}

void delay_ms (__IO uint32_t ms) {
	
	uint32_t i;
	
	for(i=0;i<ms;i++){
		delay_us(1000);
	}
	
}

void VCC_Config(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

int main(void){
	
	VCC_Config();
	
	while(1){
		GPIO_SetBits(GPIOA,GPIO_Pin_3);
		
		delay_ms(3000);
		
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);
		
		delay_ms(3000);
	}
}

