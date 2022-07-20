# include "stm32f10x.h"
# include "core_cm3.h"
# include "stdio.h"

void delay_us(u32 nus){
	u32 temp;
  SysTick->LOAD = 9*nus;
  SysTick->VAL=0X00;
  SysTick->CTRL=0X01;
  do{
		
    temp=SysTick->CTRL;
		
  }while((temp&0x01)&&(!(temp&(1<<16))));
	
  SysTick->CTRL=0x00; 
  SysTick->VAL =0X00;
}

void GPIO_Input(void){
	
	GPIO_InitTypeDef GPIOx;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIOx.GPIO_Pin = GPIO_Pin_11;
	GPIOx.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOx.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(GPIOB,&GPIOx);
}

void GPIO_Output(void){
	GPIO_InitTypeDef GPIOx;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIOx.GPIO_Pin = GPIO_Pin_11;
	GPIOx.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOx.GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_Init(GPIOB,&GPIOx);
}

void DHT11_Reset(void){
	
	GPIO_Output();
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);
	delay_us(19000);
	GPIO_SetBits(GPIOB,GPIO_Pin_11);
	delay_us(30);
	GPIO_Input();
}

u16 BitStatus(void){
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
}

u16 ReadBit(void){
	
	while (BitStatus() == RESET);
	delay_us(40);
	if (BitStatus() == SET){
		while (BitStatus() == SET);
		return 1;
	}
	else return 0;
}

u16 ReadByte(void){
	u16 i;
  u16 data = 0;
  for (i = 0; i < 8; i++){
		data <<= 1;
    data |= ReadBit();
  }
  return data;
}

u16 ReadData(u8 buffer[5]){
	
	u16 i =0;
	
	DHT11_Reset();
	
	if(BitStatus()== RESET){
		
		while(BitStatus() == RESET);
		
		while(BitStatus() == SET);
		
		for(i=0;i<5;i++){
			buffer[i] = ReadByte();
		}
		
		while(BitStatus == RESET);
		
		GPIO_Output();
		GPIO_SetBits(GPIOB,GPIO_Pin_11);
		
		if((buffer[0] + buffer[1] + buffer[2] + buffer[3]) != buffer[4]){
			return 1;
		}
	}
	return 0;
}

//供电引脚初始化
void VCC_Config(){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

/*初始化串口打印的通信函数*/
void USART_Config(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}

int fputc(int ch, FILE *f){
	
	USART_SendData(USART1,(uint8_t) ch);
	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	
	return ch;
}

int fgetc(FILE *f){
	
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	
	return (int)USART_ReceiveData(USART1);
}


int main(){
	
	double hum,tep;
	u8 buffer[5];
	
	//初始化串口
	USART_Config();
	
	//开启供电
	VCC_Config();

	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	printf("供电成功\n");
	
	while (1){
		if (ReadData(buffer) == 0){
			hum = buffer[0] + buffer[1]/10;
			tep = buffer[2] + buffer[3]/10;
			
			printf("温度为：%lf\n", tep);
			printf("湿度为：%lf\n", hum);
			printf("\n");
		
			delay_us(1800000);
		}
		else {
			printf("error");
		}
	}
}


