#include <reg51.h>

typedef   unsigned char  uint8;
typedef   unsigned int   uint16;

sbit s1 = P3^5;
sbit s2 = P3^4;
sbit s3 = P3^3;

uint8 Buf1[]="A220V";
uint8 Buf2[]="A110V";
uint8 Buf3[]="A55V";

/*
 * ��ʱ
*/
void delay(uint16 n)
{
        while (n--);
}

/*
 * UART��ʼ��
 * �����ʣ�9600
*/
void UART_init(void)
{
    SCON = 0x50;        // 10λuart�������н���

    TMOD = 0x20;        // ��ʱ��1�����ڷ�ʽ2���Զ���װ��
    TH1 = 0xFD;
    TL1 = 0xFD;

    TR1 = 1;
}

/*
 * UART ����һ�ֽ�
*/
void UART_send_byte(uint8 dat)
{
        SBUF = dat;
        while (TI == 0);
        TI = 0;
}

/*
 * UART �����ַ���
*/
void UART_send_string(uint8 *buf)
{
        while (*buf != '\0')
        {
                UART_send_byte(*buf++);
        }
}

main()
{
        UART_init();
        
        while (1)
        {
          if(s1 == 0)//�ж�S2�Ƿ񰴼�
		      {
						delay(50);//�����ʱ
						if(s1 == 0)
						{
							UART_send_string(Buf1);
						}
						while(!s1);	//���ּ��
					}      
					
					if(s2 == 0)//�ж�S2�Ƿ񰴼�
		      {
						delay(50);//�����ʱ
						if(s2 == 0)
						{
							UART_send_string(Buf2);
						}
						while(!s2);	//���ּ��
					}      
					
					if(s3 == 0)//�ж�S2�Ƿ񰴼�
		      {
						delay(50);//�����ʱ
						if(s3 == 0)
						{
							UART_send_string(Buf3);
						}
						while(!s3);	//���ּ��
					}      
           delay(20000);
        }

}