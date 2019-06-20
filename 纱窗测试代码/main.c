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
 * 延时
*/
void delay(uint16 n)
{
        while (n--);
}

/*
 * UART初始化
 * 波特率：9600
*/
void UART_init(void)
{
    SCON = 0x50;        // 10位uart，允许串行接受

    TMOD = 0x20;        // 定时器1工作在方式2（自动重装）
    TH1 = 0xFD;
    TL1 = 0xFD;

    TR1 = 1;
}

/*
 * UART 发送一字节
*/
void UART_send_byte(uint8 dat)
{
        SBUF = dat;
        while (TI == 0);
        TI = 0;
}

/*
 * UART 发送字符串
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
          if(s1 == 0)//判断S2是否按键
		      {
						delay(50);//软件延时
						if(s1 == 0)
						{
							UART_send_string(Buf1);
						}
						while(!s1);	//松手检测
					}      
					
					if(s2 == 0)//判断S2是否按键
		      {
						delay(50);//软件延时
						if(s2 == 0)
						{
							UART_send_string(Buf2);
						}
						while(!s2);	//松手检测
					}      
					
					if(s3 == 0)//判断S2是否按键
		      {
						delay(50);//软件延时
						if(s3 == 0)
						{
							UART_send_string(Buf3);
						}
						while(!s3);	//松手检测
					}      
           delay(20000);
        }

}