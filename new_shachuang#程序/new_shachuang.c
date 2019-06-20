
//程序头函数
#include <reg52.h>
//显示函数
#include <intrins.h>											   

//宏定义
#define uint unsigned int 
#define uchar unsigned char

//管脚声明
sbit LED_wendu = P2^3;		//温度上限值操作端口,报警灯吸合继电器;
sbit LED_wendu_L = P2^2;		//温度下限报警灯
sbit LED_humi = P2^1;		//湿度上限报警灯
sbit LED_humi_L	= P2^0;		//湿度下限报警灯
//sbit baojing			= P3^2;		//蜂鸣器接口
//按键

sbit Key1=P1^0;				 //设置键
sbit Key2=P1^1;				 //加按键
sbit Key3=P1^2;				 //减按键

sbit DHT11_IN=P1^3;				 //DHT11_IN	 纱窗内侧传感器
sbit DHT11_OUT=P1^4;			 //DHT11_OUT 纱窗外侧传感器

signed char w;     				//温度值全局变量
uint c; 					//温度值全局变量
uchar temp;					 //用于读取ADC数据
uchar yushe_wendu=60;		 //温度预设值
uchar yushe_humi=88;		 //湿度预设值

uchar yushe_wendu_L=15;		 //温度预设值
uchar yushe_humi_L=10;		 //湿度预设值
//按钮模式|  
uchar Mode=0;				 
//无temp表示最终量；
uchar  U8T_data_H,U8T_data_L,U8HU_data_H,U8HU_data_L,U8checkdata;
uchar  U8FLAG,U8comdata;
uchar  U8count,U8temp;
//temp后缀表示temporary，为临时变量；
uchar  U8T_data_H_temp,U8T_data_L_temp,U8HU_data_H_temp,U8HU_data_L_temp,U8checkdata_temp;
//comp后缀表示compile，作为数据整理时的临时变量；
uchar  U8T_data_H_comp,U8T_data_L_comp,U8HU_data_H_comp,U8HU_data_L_comp,U8checkdata_comp;

//软件延时函数
void delay(uint z)		  //延时函数大约延时z ms
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

//LCD管脚声明 （RW引脚实物直接接地，因为本设计只用到液晶的写操作，RW引脚一直是低电平）
sbit LCDRS = P3^5;
sbit LCDEN = P3^4;
//初始化时显示的内容
uchar code Init1[]="PM=    L    H   ";
uchar code Init2[]="HU=  % L  % H  %";
//LCD延时
void LCDdelay(uint z)		  //该延时大约100us
{
  uint x,y;
  for(x=z;x>0;x--)
    for(y=10;y>0;y--);
}

//写命令
void write_com(uchar com)
{
  LCDEN = 0;
  LCDRS=0;			//选择指令寄存器	 
  P0=com;       //写的指令
  LCDdelay(5);
  LCDEN=1;			//高电平到低电平启动
  LCDdelay(5);
  LCDEN=0;
}
//写数据
void write_data(uchar date)
{
  LCDRS=1;    //选择数据寄存器
  P0=date;    //写入的数据
  LCDdelay(5);
  LCDEN=1;
  LCDdelay(5);
  LCDEN=0;
}
/*1602指令简介
  write_com(0x38);//屏幕初始化，16*2显示，5*7点阵，8位数据
  write_com(0x0c);//打开显示 无光标 无光标闪烁
  write_com(0x0d);//打开显示 阴影闪烁
  write_com(0x0e);//打开显示 显示下划线
  write_com(0x0f);//打开显示 阴影闪烁,显示下划线
*/
//1602初始化

void Init1602()
{
  uchar i=0;
  write_com(0x38);//屏幕初始化
  write_com(0x0c);//打开显示 无光标 无光标闪烁
  write_com(0x06);//当读或写一个字符是指针后移一位
  write_com(0x01);//清屏
  write_com(0x80);//设置位置，为00h
  //===============================================================
  for(i=0;i<16;i++)
  {
		write_data(Init1[i]);	  //显示数组1的数据
  }
	write_com(0x80+5);//置数据存储器地址
	write_data(0x25);//显示符号%
	write_com(0x80+10);
	write_data(0x25);//%%显示符号
	write_com(0x80+15);
	write_data(0x25);//%显示符号
  write_com(0x80+0x40);//设置位置
  for(i=0;i<16;i++)
  {
		write_data(Init2[i]);	  //显示数组2的数据
  }	
}

void Display_1602(yushe_wendu,yushe_yanwu,yushe_wendu_L,yushe_yanwu_L,c,temp)
{
	//设置最高温度值
	write_com(0x80+13);
	write_data(0x30+yushe_wendu/10);  //‘0’+yushe_wendu/10：十位
	write_data(0x30+yushe_wendu%10);  //‘0’+yushe_wendu%10：个位
	//设置最低温度值
	write_com(0x80+8);
	write_data(0x30+yushe_wendu_L/10);
	write_data(0x30+yushe_wendu_L%10);

	//设置最高湿度值
	write_com(0x80+0x40+13);
	write_data(0x30+yushe_yanwu%100/10);
	write_data(0x30+yushe_yanwu%10);
	//设置最低湿度值
	write_com(0x80+0x40+8);
	write_data(0x30+yushe_yanwu_L%100/10);
	write_data(0x30+yushe_yanwu_L%10);
	//时时温度
	write_com(0x80+3);
	write_data(0x30+c%100/10);
	write_data(0x30+c%10);
	//时时湿度
	write_com(0x80+0x40+3);
	write_data(0x30+temp%100/10);
	write_data(0x30+temp%10);
}



//按键处理函数
void Key()
{
	//模式选择
	if(Key1==0)	  //设置按键
	{
		while(Key1==0); //按一下key1开始设置
		if(Mode>=5) Mode=0;
		else
		{
			Mode++;			//模式加一
			switch(Mode)	//判断模式的值
			{
				case 1://调温度上限值模式
				{
					write_com(0x80+12);//为1时 温度阀值的位置闪烁
					write_data('-');
					write_com(0x80+0x40+12);//为2时 湿度阀值的位置闪烁
					write_data('H');
					write_com(0x80+7);//为1时 温度阀值的位置闪烁
					write_data('L');
					write_com(0x80+0x40+7);//为2时 湿度阀值的位置闪烁
					write_data('L');
					break;			  //执行后跳出switch
				}
				case 2://调湿度上限值模式
				{
					write_com(0x80+0x40+12);//为2时 湿度阀值的位置闪烁
					write_data('-');
					write_com(0x80+12);//为1时 温度阀值的位置闪烁
					write_data('H');
					write_com(0x80+7);//为1时 温度阀值的位置闪烁
					write_data('L');
					write_com(0x80+0x40+7);//为2时 湿度阀值的位置闪烁
					write_data('L');
					break;
				}
				case 3://调温度下限值模式
				{
					write_com(0x80+7);//为1时 温度阀值的位置闪烁
					write_data('-');
					write_com(0x80+12);//为1时 温度阀值的位置闪烁
					write_data('H');
					write_com(0x80+0x40+12);//为2时 湿度阀值的位置闪烁
					write_data('H');
					write_com(0x80+0x40+7);//为2时 湿度阀值的位置闪烁
					write_data('L');
					break;			  //执行后跳出switch
				}
				case 4://调湿度下限值模式
				{
					write_com(0x80+0x40+7);//为2时 湿度阀值的位置闪烁
					write_data('-');
					write_com(0x80+12);//为1时 温度阀值的位置闪烁
					write_data('H');
					write_com(0x80+0x40+12);//为2时 湿度阀值的位置闪烁
					write_data('H');
					write_com(0x80+7);//为1时 温度阀值的位置闪烁
					write_data('L');
					break;
				}
				case 5:					//当模式加到3时
				{
					write_com(0x38);//屏幕初始化
					write_com(0x0c);//打开显示 无光标 无光标闪烁
					write_com(0x80+12);//为1时 温度阀值的位置闪烁
					write_data('H');
					write_com(0x80+0x40+12);//为2时 湿度阀值的位置闪烁
					write_data('H');
					write_com(0x80+7);//为1时 温度阀值的位置闪烁
					write_data('L');
					write_com(0x80+0x40+7);//为2时 湿度阀值的位置闪烁
					write_data('L');
					Mode=0;			//模式清零
					break;
				}
			}
		}
	}
	if(Key2==0&&Mode!=0)	 //加按键只有在模式不等于0时有效
	{
		while(Key2==0);
		switch(Mode)		 //加按键按下时 判断当前模式
		{
			case 1:			 //模式为1时
			{
				yushe_wendu++;				//预设温度上限值（阀值）加1
				if(yushe_wendu>=99)		    //加到大于等于99时
				yushe_wendu=99;	 					//固定为99
				write_com(0x80+13);			//选1602上显示的位置
				write_data(0x30+yushe_wendu/10); 	//将温度上限值数据分解开送入液晶显示
				write_data(0x30+yushe_wendu%10);
				break;
			}
			case 2:
			{
				yushe_humi++;				//湿度上限值,同温度上限值设置
				if(yushe_humi>=100)
				yushe_humi=100;
				write_com(0x80+0x40+13);
				write_data(0x30+yushe_humi%100/10);
				write_data(0x30+yushe_humi%10);
				break;				
			}
			case 3:			 //模式为3时
			{
				yushe_wendu_L++;		//温度下限值,同温度上限值设置
				if(yushe_wendu_L>=95)				 				
				yushe_wendu_L=95;					 					
				write_com(0x80+8);				 				
				write_data(0x30+yushe_wendu_L/10); 
				write_data(0x30+yushe_wendu_L%10);
				break;
			}
			case 4:
			{
				yushe_humi_L++;		//湿度下限值,同温度上限值设置
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
				if(yushe_wendu<=1)			//当温度上限值减小到1时
				yushe_wendu=1;                        //固定为1
				yushe_wendu--;				            //预设温度值减一
				write_com(0x80+13);						//选1602上显示的位置
				write_data(0x30+yushe_wendu/10);     	//将温度上限值数据分解开送入液晶显示
				write_data(0x30+yushe_wendu%10);
				break;
			}
			case 2:
			{
				if(yushe_humi<=1)			//湿度上限值,同温度上限值设置
				yushe_humi=1;
				yushe_humi--;							  
				write_com(0x80+0x40+13);
				write_data(0x30+yushe_humi%100/10);
				write_data(0x30+yushe_humi%10);
				break;				
			}
			case 3:			 //模式为3时
			{
				if(yushe_wendu_L<=1)		//温度下限值,同温度上限值设置
				yushe_wendu_L=1;					 				
				yushe_wendu_L--;					 				
				write_com(0x80+8);				 				
				write_data(0x30+yushe_wendu_L/10);
				write_data(0x30+yushe_wendu_L%10);
				break;
			}
			case 4:
			{
				if(yushe_humi_L<=1)			//湿度下限值,同温度上限值设置
				yushe_humi_L=1;					 					
				yushe_humi_L--;					 					
				write_com(0x80+0x40+8);
				write_data(0x30+yushe_humi_L%100/10);
				write_data(0x30+yushe_humi_L%10);
				break;				
			}		}
	}
	delay(10);					 //延时去抖动
	write_com(0x38);//屏幕初始化
	write_com(0x0c);//打开显示 无光标 无光标闪烁
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
		//超时则跳出for循环		  
		if(U8FLAG==1)break;
		//判断数据位是0还是1	 
		// 如果高电平高过预定0高电平值则数据位为 1 
		U8comdata<<=1;
		U8comdata|=U8temp;        //
	}
}
//--------------------------------
//-----湿度读取子程序 ------------
//--------------------------------
//----以下变量均为全局变量--------
//----温度高8位== U8T_data_H------
//----温度低8位== U8T_data_L------
//----湿度高8位== U8HU_data_H-----
//----湿度低8位== U8HU_data_L-----
//----校验 8位 == U8checkdata-----
//----调用相关子程序如下----------
//---- Delay();, Delay_10us();,COM(); 
//--------------------------------

//uchar choice = 1;
//sbit DHT11 = P1^4;
void ReadHu(/*uchar Cho*/)	  
{
/*	if(!Cho)
		DHT11 = DHT11_IN;
	else DHT11 = DHT11_OUT;	 */
	//主机拉低18ms 
	DHT11_IN=0;
	Delay1(180);
	ET0 = 0;//关闭定时器0中断
	DHT11_IN=1;
	//总线由上拉电阻拉高 主机延时20us
	Delay_10us();
	Delay_10us();
	Delay_10us();
	Delay_10us();
	//主机设为输入 判断从机响应信号 
	DHT11_IN=1;
	//判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
	if(!DHT11_IN)		 //T !	  
	{
		U8FLAG=2;
		//判断从机是否发出 80us 的低电平响应信号是否结束	 
		while((!DHT11_IN)&&U8FLAG++);
		U8FLAG=2;
		//判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
		while((DHT11_IN)&&U8FLAG++);
		//数据接收状态		 
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
		ET0 = 1;//开启定时器0中断
		//数据校验 

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

//用于处理内外接收到的湿度值；	
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

/*****主函数*****/
void main()
{
	uint i=0;
	Delay_Ms(100);//上电延时100ms,等待DHT11启动
	Init1602();			  //调用初始化显示函数
	U8T_data_H=30;
	U8HU_data_H=60;
	while(1)			  //进入循环
	{

//		Key();			  		//调用按键函数  扫描按键
//		if(Mode==0)		  //只有当模式为0时才会执行以下的阀值判断部分
//		{
//			if(i<1)
//			{
//				i=10;
//				ReadHu();//温湿度检测受定时器影响,故在驱动程序中关闭定时器中断,减小扫描频率才能更好的实现红外接收
//			}
//			i--;
//			//显示预设温度上限值，预设湿度上限值，预设温度下限值，预设湿度下限值，当前温度值，当前湿度值
			Display_1602(1,2,3,4,5,6);
			
//			if(U8HU_data_H>=yushe_humi)	 //湿度值大于等于预设上限值时
//			{
//				LED_humi=0;		  	//湿度上限指示灯亮
//				//baojing=0;			  //蜂鸣器报警
//			}
//			else if(U8HU_data_H<=yushe_humi_L)	  //湿度值小于等于预设值下限值时
//			{
//				LED_humi_L=0;		  //湿度下限指示灯亮
//				//baojing=0;			  //蜂鸣器报警
//			}
//			else					  //湿度值处于上限界限之间时
//			{
//				LED_humi=1;		  	//关掉湿度上限报警灯
//				LED_humi_L=1;		  //关掉湿度下限报警灯
//			}
//			if(U8T_data_H>=(yushe_wendu))	  //温度值大于等于预设上限值时
//			{
//				//baojing=0;			  //打开蜂鸣器报警
//				LED_wendu=0;		  //打开温度上限报警灯
//			}
//			else if(U8T_data_H<=yushe_wendu_L)	  //温度值小于等于预设值下限值时
//			{
//				LED_wendu_L=0;		  //温度下限指示灯亮
//				//baojing=0;			  //蜂鸣器报警
//			}
//			else					  //温度值处于上限界限之间时
//			{
//				LED_wendu=1;		  	//关闭温度上限报警灯
//				LED_wendu_L=1;		  //关闭温度下限报警灯
//			}
//			if(yushe_humi_L<U8HU_data_H&&U8HU_data_H<yushe_humi&&yushe_wendu_L<U8T_data_H&&U8T_data_H<yushe_wendu);	  //当湿度和温度都处于正常值时 			
//		else
//		{
//			ET0 = 1;//开启定时器0中断
//		}
//	}
}
}	
