/*
串口收发程序，把发送的数据接收后用P1口的LED灯以二进制的形式显示
并且蜂鸣器发出提示音，再把接收的数据加1并发出。
使用STC ISP软件界面上的串口助手，选择对应的串口号，
设置波特率为9600，选择HEX模式发送和接收
*/
#include <reg52.h>

#define uchar unsigned char
#define uint  unsigned int

sbit beep = P2^3;//蜂鸣器接口

sbit s1=P3^5;
sbit s2=P3^4;
uchar c1[]="A110V";//发送暂存变量
uchar c2[]="A55V";//发送暂存变量
uchar  sendFlag = 0;     //未发送数据时  
uchar  receFlag =0;     //未接受到数据时  

void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}	
void UART_init()
{
//	TMOD = 0x20;  	//T1工作模式2  8位自动重装
//	TH1 = 0xfd;
//	TL1 = 0xfd; 	//比特率9600
//	TR1 = 1;		//启动T1定时器
	SM0 = 0;
	SM1 = 1; 		//串口工作方式1 10位异步
	REN = 1;		//串口允许接收
	EA  = 1;		//开总中断
	ES  = 1;		//串口中断打开
}

void sendChar(uchar Value)  //发送一个字节数据  
{  
     SBUF = Value;       
     sendFlag = 1;       //设置发送标志位,发一字节就置位  
     while(sendFlag);    //直到发完数据,将sendFlag清零后,才退出sendChar函数  
}  
  
void sendAll(uchar *pValue) //发送一组数据  
{  
    while((*pValue) != '\0')   //如果没有发送完毕就继续发  
    {  
        sendChar(*pValue);      //发送1字节数据  
        pValue++;                 //指向下1个字节  
    }  
}  

void main()
{
	UART_init(); //串口初始化
	while(1)
	{
		if(s1 == 0)//判断S2是否按键
		{
			delay(5);//软件延时
			if(s1 == 0)
			{
				sendAll(c1); 
				receFlag=0;             //发完了清标志  
			}
			while(!s1);	//松手检测
		}
		
		if(s2 == 0)//判断S2是否按键
		{
			delay(5);//软件延时
			if(s2 == 0)
			{
				sendAll(c2); 
				receFlag=0;             //发完了清标志  
			}
			while(!s2);	//松手检测
		}
	}
}


//void UART() interrupt 4
//{
//	if(RI)	//检测是否接收完成
//	{
//		num = SBUF;	  //num 取出接收缓存器的值
//		P1 = SBUF;
//		beep = 0;
//		delay(100);
//		beep = 1;//蜂鸣器发出滴提示音
//		num++;//把接收的数据加1并发送
//		RI = 0;
//		SBUF = num;	
//		while(!TI);
//		TI = 0;
//	}
//}