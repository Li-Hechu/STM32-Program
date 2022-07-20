/*
 *������;���жϷ��������ݲ��һش�
*/

# include "stm32f10x.h"
# include "core_cm3.h"

/*
 *�����ж�
*/
void NVIC_Config(void) {
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

/*
 *���ڳ�ʼ��
*/
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
	
	//�����ж�
	NVIC_Config();
	
	//ʹ�ܽ����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//ʹ�ܴ���
	USART_Cmd(USART1,ENABLE);
}

/*
 *����һ���ֽ�
*/
void USART_SendByte (USART_TypeDef * pUSARTx, uint8_t ch) {
	
	USART_SendData(pUSARTx,ch);
	
	//�ȴ��������
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
} 

/*
 *�����ַ���
*/
void USART_SendString (USART_TypeDef * pUSARTx, char *str) {
	
	unsigned int k = 0;
	
	do {
		USART_SendByte(pUSARTx, *(str + k));
		k++;
	}while(*(str + k) != '\0');
	
	//�ȴ��������
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

/*
 *�жϷ���������Ƭ�����յ����ڷ��͵����ݺ���������ͬ�����ݡ�
*/
void USART1_IRQHandler (void) {
	uint8_t ucTemp;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		ucTemp = USART_ReceiveData(USART1);
		USART_SendData(USART1,ucTemp);
	}
}

int main(void) {
	
	USART_Config();
	
	//���Ե�Ƭ���Ƿ���Խ��յ�����
	USART_SendString(USART1,"sancnsknc;\n");
	
	//���庯��������ѭ����ֻ��Ҫ�����жϼ���
	while(1);
}

