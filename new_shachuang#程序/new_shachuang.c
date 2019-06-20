
//����ͷ����
#include <reg52.h>
//��ʾ����
#include <intrins.h>											   

//�궨��
#define uint unsigned int 
#define uchar unsigned char

//�ܽ�����
sbit LED_wendu = P2^3;		//�¶�����ֵ�����˿�,���������ϼ̵���;
sbit LED_wendu_L = P2^2;		//�¶����ޱ�����
sbit LED_humi = P2^1;		//ʪ�����ޱ�����
sbit LED_humi_L	= P2^0;		//ʪ�����ޱ�����
//sbit baojing			= P3^2;		//�������ӿ�
//����

sbit Key1=P1^0;				 //���ü�
sbit Key2=P1^1;				 //�Ӱ���
sbit Key3=P1^2;				 //������

sbit DHT11_IN=P1^3;				 //DHT11_IN	 ɴ���ڲഫ����
sbit DHT11_OUT=P1^4;			 //DHT11_OUT ɴ����ഫ����

signed char w;     				//�¶�ֵȫ�ֱ���
uint c; 					//�¶�ֵȫ�ֱ���
uchar temp;					 //���ڶ�ȡADC����
uchar yushe_wendu=60;		 //�¶�Ԥ��ֵ
uchar yushe_humi=88;		 //ʪ��Ԥ��ֵ

uchar yushe_wendu_L=15;		 //�¶�Ԥ��ֵ
uchar yushe_humi_L=10;		 //ʪ��Ԥ��ֵ
//��ťģʽ|  
uchar Mode=0;				 
//��temp��ʾ��������
uchar  U8T_data_H,U8T_data_L,U8HU_data_H,U8HU_data_L,U8checkdata;
uchar  U8FLAG,U8comdata;
uchar  U8count,U8temp;
//temp��׺��ʾtemporary��Ϊ��ʱ������
uchar  U8T_data_H_temp,U8T_data_L_temp,U8HU_data_H_temp,U8HU_data_L_temp,U8checkdata_temp;
//comp��׺��ʾcompile����Ϊ��������ʱ����ʱ������
uchar  U8T_data_H_comp,U8T_data_L_comp,U8HU_data_H_comp,U8HU_data_L_comp,U8checkdata_comp;

//�����ʱ����
void delay(uint z)		  //��ʱ������Լ��ʱz ms
{
	uint i,j;
	for(i=0;i<z;i++)
	for(j=0;j<121;j++);
}

void Delay_Ms(uint ms)
{
	uint i,j;
	for(i=ms;i>0;i--)
		for(j=112;j>0;j--);
}

//LCD�ܽ����� ��RW����ʵ��ֱ�ӽӵأ���Ϊ�����ֻ�õ�Һ����д������RW����һֱ�ǵ͵�ƽ��
sbit LCDRS = P3^5;
sbit LCDEN = P3^4;
//��ʼ��ʱ��ʾ������
uchar code Init1[]="PM=    L    H   ";
uchar code Init2[]="HU=  % L  % H  %";
//LCD��ʱ
void LCDdelay(uint z)		  //����ʱ��Լ100us
{
  uint x,y;
  for(x=z;x>0;x--)
    for(y=10;y>0;y--);
}

//д����
void write_com(uchar com)
{
  LCDEN = 0;
  LCDRS=0;			//ѡ��ָ��Ĵ���	 
  P0=com;       //д��ָ��
  LCDdelay(5);
  LCDEN=1;			//�ߵ�ƽ���͵�ƽ����
  LCDdelay(5);
  LCDEN=0;
}
//д����
void write_data(uchar date)
{
  LCDRS=1;    //ѡ�����ݼĴ���
  P0=date;    //д�������
  LCDdelay(5);
  LCDEN=1;
  LCDdelay(5);
  LCDEN=0;
}
/*1602ָ����
  write_com(0x38);//��Ļ��ʼ����16*2��ʾ��5*7����8λ����
  write_com(0x0c);//����ʾ �޹�� �޹����˸
  write_com(0x0d);//����ʾ ��Ӱ��˸
  write_com(0x0e);//����ʾ ��ʾ�»���
  write_com(0x0f);//����ʾ ��Ӱ��˸,��ʾ�»���
*/
//1602��ʼ��

void Init1602()
{
  uchar i=0;
  write_com(0x38);//��Ļ��ʼ��
  write_com(0x0c);//����ʾ �޹�� �޹����˸
  write_com(0x06);//������дһ���ַ���ָ�����һλ
  write_com(0x01);//����
  write_com(0x80);//����λ�ã�Ϊ00h
  //===============================================================
  for(i=0;i<16;i++)
  {
		write_data(Init1[i]);	  //��ʾ����1������
  }
	write_com(0x80+5);//�����ݴ洢����ַ
	write_data(0x25);//��ʾ����%
	write_com(0x80+10);
	write_data(0x25);//%%��ʾ����
	write_com(0x80+15);
	write_data(0x25);//%��ʾ����
  write_com(0x80+0x40);//����λ��
  for(i=0;i<16;i++)
  {
		write_data(Init2[i]);	  //��ʾ����2������
  }	
}

void Display_1602(yushe_wendu,yushe_yanwu,yushe_wendu_L,yushe_yanwu_L,c,temp)
{
	//��������¶�ֵ
	write_com(0x80+13);
	write_data(0x30+yushe_wendu/10);  //��0��+yushe_wendu/10��ʮλ
	write_data(0x30+yushe_wendu%10);  //��0��+yushe_wendu%10����λ
	//��������¶�ֵ
	write_com(0x80+8);
	write_data(0x30+yushe_wendu_L/10);
	write_data(0x30+yushe_wendu_L%10);

	//�������ʪ��ֵ
	write_com(0x80+0x40+13);
	write_data(0x30+yushe_yanwu%100/10);
	write_data(0x30+yushe_yanwu%10);
	//�������ʪ��ֵ
	write_com(0x80+0x40+8);
	write_data(0x30+yushe_yanwu_L%100/10);
	write_data(0x30+yushe_yanwu_L%10);
	//ʱʱ�¶�
	write_com(0x80+3);
	write_data(0x30+c%100/10);
	write_data(0x30+c%10);
	//ʱʱʪ��
	write_com(0x80+0x40+3);
	write_data(0x30+temp%100/10);
	write_data(0x30+temp%10);
}



//����������
void Key()
{
	//ģʽѡ��
	if(Key1==0)	  //���ð���
	{
		while(Key1==0); //��һ��key1��ʼ����
		if(Mode>=5) Mode=0;
		else
		{
			Mode++;			//ģʽ��һ
			switch(Mode)	//�ж�ģʽ��ֵ
			{
				case 1://���¶�����ֵģʽ
				{
					write_com(0x80+12);//Ϊ1ʱ �¶ȷ�ֵ��λ����˸
					write_data('-');
					write_com(0x80+0x40+12);//Ϊ2ʱ ʪ�ȷ�ֵ��λ����˸
					write_data('H');
					write_com(0x80+7);//Ϊ1ʱ �¶ȷ�ֵ��λ����˸
					write_data('L');
					write_com(0x80+0x40+7);//Ϊ2ʱ ʪ�ȷ�ֵ��λ����˸
					write_data('L');
					break;			  //ִ�к�����switch
				}
				case 2://��ʪ������ֵģʽ
				{
					write_com(0x80+0x40+12);//Ϊ2ʱ ʪ�ȷ�ֵ��λ����˸
					write_data('-');
					write_com(0x80+12);//Ϊ1ʱ �¶ȷ�ֵ��λ����˸
					write_data('H');
					write_com(0x80+7);//Ϊ1ʱ �¶ȷ�ֵ��λ����˸
					write_data('L');
					write_com(0x80+0x40+7);//Ϊ2ʱ ʪ�ȷ�ֵ��λ����˸
					write_data('L');
					break;
				}
				case 3://���¶�����ֵģʽ
				{
					write_com(0x80+7);//Ϊ1ʱ �¶ȷ�ֵ��λ����˸
					write_data('-');
					write_com(0x80+12);//Ϊ1ʱ �¶ȷ�ֵ��λ����˸
					write_data('H');
					write_com(0x80+0x40+12);//Ϊ2ʱ ʪ�ȷ�ֵ��λ����˸
					write_data('H');
					write_com(0x80+0x40+7);//Ϊ2ʱ ʪ�ȷ�ֵ��λ����˸
					write_data('L');
					break;			  //ִ�к�����switch
				}
				case 4://��ʪ������ֵģʽ
				{
					write_com(0x80+0x40+7);//Ϊ2ʱ ʪ�ȷ�ֵ��λ����˸
					write_data('-');
					write_com(0x80+12);//Ϊ1ʱ �¶ȷ�ֵ��λ����˸
					write_data('H');
					write_com(0x80+0x40+12);//Ϊ2ʱ ʪ�ȷ�ֵ��λ����˸
					write_data('H');
					write_com(0x80+7);//Ϊ1ʱ �¶ȷ�ֵ��λ����˸
					write_data('L');
					break;
				}
				case 5:					//��ģʽ�ӵ�3ʱ
				{
					write_com(0x38);//��Ļ��ʼ��
					write_com(0x0c);//����ʾ �޹�� �޹����˸
					write_com(0x80+12);//Ϊ1ʱ �¶ȷ�ֵ��λ����˸
					write_data('H');
					write_com(0x80+0x40+12);//Ϊ2ʱ ʪ�ȷ�ֵ��λ����˸
					write_data('H');
					write_com(0x80+7);//Ϊ1ʱ �¶ȷ�ֵ��λ����˸
					write_data('L');
					write_com(0x80+0x40+7);//Ϊ2ʱ ʪ�ȷ�ֵ��λ����˸
					write_data('L');
					Mode=0;			//ģʽ����
					break;
				}
			}
		}
	}
	if(Key2==0&&Mode!=0)	 //�Ӱ���ֻ����ģʽ������0ʱ��Ч
	{
		while(Key2==0);
		switch(Mode)		 //�Ӱ�������ʱ �жϵ�ǰģʽ
		{
			case 1:			 //ģʽΪ1ʱ
			{
				yushe_wendu++;				//Ԥ���¶�����ֵ����ֵ����1
				if(yushe_wendu>=99)		    //�ӵ����ڵ���99ʱ
				yushe_wendu=99;	 					//�̶�Ϊ99
				write_com(0x80+13);			//ѡ1602����ʾ��λ��
				write_data(0x30+yushe_wendu/10); 	//���¶�����ֵ���ݷֽ⿪����Һ����ʾ
				write_data(0x30+yushe_wendu%10);
				break;
			}
			case 2:
			{
				yushe_humi++;				//ʪ������ֵ,ͬ�¶�����ֵ����
				if(yushe_humi>=100)
				yushe_humi=100;
				write_com(0x80+0x40+13);
				write_data(0x30+yushe_humi%100/10);
				write_data(0x30+yushe_humi%10);
				break;				
			}
			case 3:			 //ģʽΪ3ʱ
			{
				yushe_wendu_L++;		//�¶�����ֵ,ͬ�¶�����ֵ����
				if(yushe_wendu_L>=95)				 				
				yushe_wendu_L=95;					 					
				write_com(0x80+8);				 				
				write_data(0x30+yushe_wendu_L/10); 
				write_data(0x30+yushe_wendu_L%10);
				break;
			}
			case 4:
			{
				yushe_humi_L++;		//ʪ������ֵ,ͬ�¶�����ֵ����
				if(yushe_humi_L>=90)				 				
				yushe_humi_L=90;					 					
				write_com(0x80+0x40+8);
				write_data(0x30+yushe_humi_L%100/10);
				write_data(0x30+yushe_humi_L%10);
				break;				
			}
		}
	}
	if(Key3==0&&Mode!=0)
	{
		while(Key3==0);
		switch(Mode)
		{
			case 1:
			{
				if(yushe_wendu<=1)			//���¶�����ֵ��С��1ʱ
				yushe_wendu=1;                        //�̶�Ϊ1
				yushe_wendu--;				            //Ԥ���¶�ֵ��һ
				write_com(0x80+13);						//ѡ1602����ʾ��λ��
				write_data(0x30+yushe_wendu/10);     	//���¶�����ֵ���ݷֽ⿪����Һ����ʾ
				write_data(0x30+yushe_wendu%10);
				break;
			}
			case 2:
			{
				if(yushe_humi<=1)			//ʪ������ֵ,ͬ�¶�����ֵ����
				yushe_humi=1;
				yushe_humi--;							  
				write_com(0x80+0x40+13);
				write_data(0x30+yushe_humi%100/10);
				write_data(0x30+yushe_humi%10);
				break;				
			}
			case 3:			 //ģʽΪ3ʱ
			{
				if(yushe_wendu_L<=1)		//�¶�����ֵ,ͬ�¶�����ֵ����
				yushe_wendu_L=1;					 				
				yushe_wendu_L--;					 				
				write_com(0x80+8);				 				
				write_data(0x30+yushe_wendu_L/10);
				write_data(0x30+yushe_wendu_L%10);
				break;
			}
			case 4:
			{
				if(yushe_humi_L<=1)			//ʪ������ֵ,ͬ�¶�����ֵ����
				yushe_humi_L=1;					 					
				yushe_humi_L--;					 					
				write_com(0x80+0x40+8);
				write_data(0x30+yushe_humi_L%100/10);
				write_data(0x30+yushe_humi_L%10);
				break;				
			}		}
	}
	delay(10);					 //��ʱȥ����
	write_com(0x38);//��Ļ��ʼ��
	write_com(0x0c);//����ʾ �޹�� �޹����˸
}

void Delay1(uint j)
{  
	uchar i;
	for(;j>0;j--)
	{ 	
		for(i=0;i<27;i++);
	}
}

void  Delay_10us(void)
{
	uchar i;
	i--;
	i--;
	i--;
	i--;
	i--;
	i--;
}

void  COM(void)
{
	uchar i;
	for(i=0;i<8;i++)	   
	{
		U8FLAG=2;	
		while((!DHT11_IN)&&U8FLAG++);
		Delay_10us();
		Delay_10us();
		Delay_10us();
		U8temp=0;
		if(DHT11_IN)U8temp=1;
		U8FLAG=2;
		while((DHT11_IN)&&U8FLAG++);
		//��ʱ������forѭ��		  
		if(U8FLAG==1)break;
		//�ж�����λ��0����1	 
		// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 
		U8comdata<<=1;
		U8comdata|=U8temp;        //
	}
}
//--------------------------------
//-----ʪ�ȶ�ȡ�ӳ��� ------------
//--------------------------------
//----���±�����Ϊȫ�ֱ���--------
//----�¶ȸ�8λ== U8T_data_H------
//----�¶ȵ�8λ== U8T_data_L------
//----ʪ�ȸ�8λ== U8HU_data_H-----
//----ʪ�ȵ�8λ== U8HU_data_L-----
//----У�� 8λ == U8checkdata-----
//----��������ӳ�������----------
//---- Delay();, Delay_10us();,COM(); 
//--------------------------------

//uchar choice = 1;
//sbit DHT11 = P1^4;
void ReadHu(/*uchar Cho*/)	  
{
/*	if(!Cho)
		DHT11 = DHT11_IN;
	else DHT11 = DHT11_OUT;	 */
	//��������18ms 
	DHT11_IN=0;
	Delay1(180);
	ET0 = 0;//�رն�ʱ��0�ж�
	DHT11_IN=1;
	//������������������ ������ʱ20us
	Delay_10us();
	Delay_10us();
	Delay_10us();
	Delay_10us();
	//������Ϊ���� �жϴӻ���Ӧ�ź� 
	DHT11_IN=1;
	//�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
	if(!DHT11_IN)		 //T !	  
	{
		U8FLAG=2;
		//�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
		while((!DHT11_IN)&&U8FLAG++);
		U8FLAG=2;
		//�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
		while((DHT11_IN)&&U8FLAG++);
		//���ݽ���״̬		 
		COM();
		U8HU_data_H_temp=U8comdata;
		COM();
		U8HU_data_L_temp=U8comdata;
		COM();
		U8T_data_H_temp=U8comdata;
		COM();
		U8T_data_L_temp=U8comdata;
		COM();
		U8checkdata_temp=U8comdata;
		DHT11_IN=1;
		ET0 = 1;//������ʱ��0�ж�
		//����У�� 

		U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8HU_data_H_temp+U8HU_data_L_temp);
		if(U8temp==U8checkdata_temp)
		{
			U8HU_data_H=U8HU_data_H_temp;
			U8HU_data_L=U8HU_data_L_temp;
			U8T_data_H=U8T_data_H_temp;
			U8T_data_L=U8T_data_L_temp;
			U8checkdata=U8checkdata_temp;
		}//fi
	}//fi

}

//���ڴ���������յ���ʪ��ֵ��	
/*void CompileHu(){
	ReadHu(DHT11_IN);
	U8HU_data_H_comp=U8HU_data_H;
	U8HU_data_L_comp=U8HU_data_L;
	U8T_data_H_comp=U8T_data_H;
	U8T_data_L_comp=U8T_data_L;
	U8checkdata_comp=U8checkdata;
	ReadHu(DHT11_OUT);
	U8HU_data_H_comp=U8HU_data_H*0.6+U8HU_data_H_comp*0.4;
	U8HU_data_L_comp=U8HU_data_L*0.6+U8HU_data_L_comp*0.4;
	U8T_data_H=U8T_data_H*0.6+U8T_data_H_comp*0.4;
	U8T_data_L=U8T_data_L*0.6+U8T_data_L_comp*0.4;
	U8checkdata_comp=U8checkdata;

}		*/

/*****������*****/
void main()
{
	uint i=0;
	Delay_Ms(100);//�ϵ���ʱ100ms,�ȴ�DHT11����
	Init1602();			  //���ó�ʼ����ʾ����
	U8T_data_H=30;
	U8HU_data_H=60;
	while(1)			  //����ѭ��
	{

//		Key();			  		//���ð�������  ɨ�谴��
//		if(Mode==0)		  //ֻ�е�ģʽΪ0ʱ�Ż�ִ�����µķ�ֵ�жϲ���
//		{
//			if(i<1)
//			{
//				i=10;
//				ReadHu();//��ʪ�ȼ���ܶ�ʱ��Ӱ��,�������������йرն�ʱ���ж�,��Сɨ��Ƶ�ʲ��ܸ��õ�ʵ�ֺ������
//			}
//			i--;
//			//��ʾԤ���¶�����ֵ��Ԥ��ʪ������ֵ��Ԥ���¶�����ֵ��Ԥ��ʪ������ֵ����ǰ�¶�ֵ����ǰʪ��ֵ
			Display_1602(1,2,3,4,5,6);
			
//			if(U8HU_data_H>=yushe_humi)	 //ʪ��ֵ���ڵ���Ԥ������ֵʱ
//			{
//				LED_humi=0;		  	//ʪ������ָʾ����
//				//baojing=0;			  //����������
//			}
//			else if(U8HU_data_H<=yushe_humi_L)	  //ʪ��ֵС�ڵ���Ԥ��ֵ����ֵʱ
//			{
//				LED_humi_L=0;		  //ʪ������ָʾ����
//				//baojing=0;			  //����������
//			}
//			else					  //ʪ��ֵ�������޽���֮��ʱ
//			{
//				LED_humi=1;		  	//�ص�ʪ�����ޱ�����
//				LED_humi_L=1;		  //�ص�ʪ�����ޱ�����
//			}
//			if(U8T_data_H>=(yushe_wendu))	  //�¶�ֵ���ڵ���Ԥ������ֵʱ
//			{
//				//baojing=0;			  //�򿪷���������
//				LED_wendu=0;		  //���¶����ޱ�����
//			}
//			else if(U8T_data_H<=yushe_wendu_L)	  //�¶�ֵС�ڵ���Ԥ��ֵ����ֵʱ
//			{
//				LED_wendu_L=0;		  //�¶�����ָʾ����
//				//baojing=0;			  //����������
//			}
//			else					  //�¶�ֵ�������޽���֮��ʱ
//			{
//				LED_wendu=1;		  	//�ر��¶����ޱ�����
//				LED_wendu_L=1;		  //�ر��¶����ޱ�����
//			}
//			if(yushe_humi_L<U8HU_data_H&&U8HU_data_H<yushe_humi&&yushe_wendu_L<U8T_data_H&&U8T_data_H<yushe_wendu);	  //��ʪ�Ⱥ��¶ȶ���������ֵʱ 			
//		else
//		{
//			ET0 = 1;//������ʱ��0�ж�
//		}
//	}
}
}	
