# include "stm32f10x.h"
# include "core_cm3.h"
// stm32f10x.h ������ core_cm3.h ǰ�棬�������
# include "stm32f10x_gpio.h"


int main(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;  //�����ڵ�һ��λ�ã��������
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
  GPIO_SetBits(GPIOB,GPIO_Pin_7);
}

