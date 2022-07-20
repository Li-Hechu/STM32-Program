# include "stm32f10x.h"
# include "core_cm3.h"
# include "stdio.h"

void USART_Config(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//����USART Tx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//����USART Rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//���ô��ڹ�������
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	//ʹ�ܴ���
	USART_Cmd(USART1,ENABLE);
}

//�ض���printf����
int fputc(int ch, FILE *f){
	
	//����һ���ֽ�
	USART_SendData(USART1,(uint8_t) ch);
	
	//�ȴ��������
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	
	return ch;
}

//�ض���scanf����
int fgetc(FILE *f){
	
	//�ȴ����ڷ�������
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	
	return (int)USART_ReceiveData(USART1);
}


//����һ���ַ�
void USART_SendByte (USART_TypeDef * pUSARTx, uint8_t ch) {
	
	USART_SendData(pUSARTx,ch);
	
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
} 

//�����ַ���
void USART_SendString (USART_TypeDef * pUSARTx, char *str) {
	
	unsigned int k = 0;
	do {
		USART_SendByte(pUSARTx, *(str + k));
		k++;
	}while(*(str + k) != '\0');
	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

int main(void){
	
	char str[20];
	
	USART_Config();
	
	USART_SendString(USART1,"sassaxsaxasxasxmjjk");
	
	printf("sxsaxsxss");
	while (1) {
		scanf("%s",str);
		printf("%s",str);
	}
}


