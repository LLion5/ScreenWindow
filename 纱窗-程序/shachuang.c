/*
�����շ����򣬰ѷ��͵����ݽ��պ���P1�ڵ�LED���Զ����Ƶ���ʽ��ʾ
���ҷ�����������ʾ�����ٰѽ��յ����ݼ�1��������
ʹ��STC ISP��������ϵĴ������֣�ѡ���Ӧ�Ĵ��ںţ�
���ò�����Ϊ9600��ѡ��HEXģʽ���ͺͽ���
*/
#include <reg52.h>

#define uchar unsigned char
#define uint  unsigned int

sbit beep = P2^3;//�������ӿ�

sbit s1=P3^5;
sbit s2=P3^4;
uchar c1[]="A110V";//�����ݴ����
uchar c2[]="A55V";//�����ݴ����
uchar  sendFlag = 0;     //δ��������ʱ  
uchar  receFlag =0;     //δ���ܵ�����ʱ  

void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}	
void UART_init()
{
//	TMOD = 0x20;  	//T1����ģʽ2  8λ�Զ���װ
//	TH1 = 0xfd;
//	TL1 = 0xfd; 	//������9600
//	TR1 = 1;		//����T1��ʱ��
	SM0 = 0;
	SM1 = 1; 		//���ڹ�����ʽ1 10λ�첽
	REN = 1;		//�����������
	EA  = 1;		//�����ж�
	ES  = 1;		//�����жϴ�
}

void sendChar(uchar Value)  //����һ���ֽ�����  
{  
     SBUF = Value;       
     sendFlag = 1;       //���÷��ͱ�־λ,��һ�ֽھ���λ  
     while(sendFlag);    //ֱ����������,��sendFlag�����,���˳�sendChar����  
}  
  
void sendAll(uchar *pValue) //����һ������  
{  
    while((*pValue) != '\0')   //���û�з�����Ͼͼ�����  
    {  
        sendChar(*pValue);      //����1�ֽ�����  
        pValue++;                 //ָ����1���ֽ�  
    }  
}  

void main()
{
	UART_init(); //���ڳ�ʼ��
	while(1)
	{
		if(s1 == 0)//�ж�S2�Ƿ񰴼�
		{
			delay(5);//�����ʱ
			if(s1 == 0)
			{
				sendAll(c1); 
				receFlag=0;             //���������־  
			}
			while(!s1);	//���ּ��
		}
		
		if(s2 == 0)//�ж�S2�Ƿ񰴼�
		{
			delay(5);//�����ʱ
			if(s2 == 0)
			{
				sendAll(c2); 
				receFlag=0;             //���������־  
			}
			while(!s2);	//���ּ��
		}
	}
}


//void UART() interrupt 4
//{
//	if(RI)	//����Ƿ�������
//	{
//		num = SBUF;	  //num ȡ�����ջ�������ֵ
//		P1 = SBUF;
//		beep = 0;
//		delay(100);
//		beep = 1;//��������������ʾ��
//		num++;//�ѽ��յ����ݼ�1������
//		RI = 0;
//		SBUF = num;	
//		while(!TI);
//		TI = 0;
//	}
//}