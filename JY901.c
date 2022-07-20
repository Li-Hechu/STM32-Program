/*
 *代码用途：接收JY901产生的数据并且解算打印在串口
 *使用的串口为USART1和USART2
*/

# include "stm32f10x.h"
# include "core_cm3.h"
# include "stdio.h"

//------结构体定义，存放JY901产生的数据--------
struct STime
{
	uint8_t H;
	uint8_t M;
	uint8_t S;
	uint8_t msL;
	uint8_t msH;
}Time;

struct SAcc
{
	uint8_t AxL;
	uint8_t AxH;
	uint8_t AyL;
	uint8_t AyH;
	uint8_t AzL;
	uint8_t AzH;
}Acc;

struct SGyro
{
	uint8_t WxL;
	uint8_t WxH;
	uint8_t WyL;
	uint8_t WyH;
	uint8_t WzL;
	uint8_t WzH;
}Gyro;

struct SAngle
{
	uint8_t RollL;
	uint8_t RollH;
	uint8_t PitchL;
	uint8_t PitchH;
	uint8_t YawL;
	uint8_t YawH;
}Angle;

struct SMag
{
	uint8_t MxL;
	uint8_t MxH;
	uint8_t MyL;
	uint8_t MyH;
	uint8_t MzL;
	uint8_t MzH;
}Mag;

struct SPress
{
	uint8_t P0;
	uint8_t P1;
	uint8_t P2;
	uint8_t P3;
	uint8_t H0;
	uint8_t H1;
	uint8_t H2;
	uint8_t H3;
}Press;

//-------说明：以下函数设置后可以使用printf和scanf函数进行串口打印---------
/*
 *配置单片机和电脑通信
 *使用查询法
 *使用USART1,引脚为A9和A10
*/
void USART1_Config(void) {
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//配置USART Tx为推挽复用
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//配置USART Rx为浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//配置串口工作参数
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	//使能串口
	USART_Cmd(USART1,ENABLE);
}

/*
 *重定向printf函数
*/
int fputc(int ch, FILE *f){
	
	//发送一个字节
	USART_SendData(USART1,(uint8_t) ch);
	
	//等待发送完成
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	
	return ch;
}

/*
 *重定向scanf函数
*/
int fgetc(FILE *f){
	
	//等待串口发送数据
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	
	return (int)USART_ReceiveData(USART1);
}

//--------说明：配置JY901和单片机之间的通信，采用中断法。-----------
/*
 *USART2配置中断
*/
void USART2_NVIC_Config(void) {
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

/*
 *配置JY901和STM32的串口通信
 *使用USART2,引脚为A2,A3
*/
void USART2_Config(void) {
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//配置USART Tx为推挽复用输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//配置USART Rx为浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//配置串口工作参数
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2,&USART_InitStructure);
	
	USART2_NVIC_Config();
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	//使能串口
	USART_Cmd(USART2, ENABLE);
}

/*
 *配置中断服务函数
*/
int count = 0;
uint8_t data[12];
void USART2_IRQHandler(void) {
	
	uint8_t bit;
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) {
		
		bit = USART_ReceiveData(USART2);
		
		if (bit == 0x55){
			data[0] = bit;
			count = 0;
			count++;
		}
		else{
			data[count++] = bit;
		}
		
		if (count == 11){
			switch (data[1]) {
				case 0x50: 
					Time.H = data[5];
					Time.M = data[6];
				  Time.S = data[7];
					Time.msL = data[8];
					Time.msH = data[9];
					break;
				case 0x51:
					Acc.AxL = data[2];
					Acc.AxH = data[3];
					Acc.AyL = data[4];
					Acc.AyH = data[5];
					Acc.AzL = data[6];
					Acc.AzH = data[7];
					break;
				case 0x52: 
					Gyro.WxL = data[2];
					Gyro.WxH = data[3];
					Gyro.WyL = data[4];
					Gyro.WyH = data[5];
					Gyro.WzL = data[6];
					Gyro.WzH = data[7];
					break;
				case 0x53: 
					Angle.RollL = data[2];
					Angle.RollH = data[3];
					Angle.PitchL = data[4];
					Angle.PitchH = data[5];
					Angle.YawL = data[6];
					Angle.YawH = data[7];
					break;
				case 0x54: 
					Mag.MxL = data[2];
					Mag.MxH = data[3];
					Mag.MyL = data[4];
					Mag.MyH = data[5];
					Mag.MzL = data[6];
					Mag.MzH = data[7];
					break;
				case 0x56:
					Press.P0 = data[2];
					Press.P1 = data[3];
					Press.P2 = data[4];
					Press.P3 = data[5];
					Press.H0 = data[6];
					Press.H1 = data[7];
					Press.H2 = data[8];
					Press.H3 = data[9];
					break;
			}
			count = 0;
		}
	}
}


//--------主函数--------
int main(void) {
	
	//串口初始化
	USART1_Config();
	USART2_Config();
	
	//循环打印
	while(1){
		printf("Time:%dH:%dM:%dS:%dms\r\n",Time.H,Time.M,Time.S,(Time.msH<<8)|Time.msL);
		printf("Acc:%.3f %.3f %.3f\r\n",(float)((short)(Acc.AxH<<8)|Acc.AxL)/32768*16,(float)((short)(Acc.AyH<<8)|Acc.AyL)/32768*16,
			(float)((short)(Acc.AzH<<8)|Acc.AzL)/32768*16);
		printf("Gyro:%.3f %.3f %.3f\r\n",(float)((short)(Gyro.WxH<<8)|Gyro.WxL)/32768*2000,(float)((short)(Gyro.WyH<<8)|Gyro.WyL)/32768*2000,
			(float)((short)(Gyro.WzH<<8)|Gyro.WzL)/32768*2000);
		printf("Angle:%.3f %.3f %.3f\r\n",(float)((short)(Angle.RollH<<8)|Angle.RollL)/32768*180,(float)((short)(Angle.PitchH<<8)|Angle.PitchL)/32768*180,
			(float)((short)(Angle.YawH<<8)|Angle.YawL)/32768*180);
		printf("Mag:%d %d %d\r\n",((Mag.MxH<<8)|Mag.MxL),((Mag.MyH<<8)|Mag.MyL),((Mag.MzH<<8)|Mag.MzL));
		printf("Pressure:%ld Height:%ld\r\n",(long int)((Press.P3<<24)|(Press.P2<<16)|(Press.P1<<8)|Press.P0),
		  (long int)((Press.H3<<24)|(Press.H2<<16)|(Press.H1<<8)|Press.H0));
	}
	/*
	 *    这里有必要解释一下为什么不在中断服务函数里面使用printf输出，而是数据存在结构体里面，在主函数里面循环输出。
	 *    我试过在中断服务里面直接输出，这样子导致的结果就是显示不完全，只能输出一部分数据，无法把全部信息输出（即使
	 *你代码是正确的）。造成这个现象的原因是JY901的读取速度太快，而串口的通信速率稍慢，导致数据还没有完全发送出
	 *去就被下一次的数据覆盖。
	 *    而在中断服务函数里面将数据存储起来，再在主函数中循环输出，可以在接收其他数据的时候，有充足的时间将当前数据
	 *传输出去。可以理解为双线程，一边将数据写入结构体，一边读取结构体的数据并且输出，结构体相当于一个缓冲区。
	*/
}






