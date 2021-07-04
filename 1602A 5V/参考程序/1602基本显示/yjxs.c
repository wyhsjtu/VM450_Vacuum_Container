#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
sbit wxLE=P2^6;
sbit dxLE=P2^7;
sbit RS=P1^3;
sbit RW=P1^4;
sbit EN=P1^5;
uint i;
uchar code table[]="I LOVE YOU !";
uchar code table1[]="1314520 ! ^_^";
void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=114;y>0;y--);
}
bit LCD_Check_Busy()
{
	P0=0xff;
	RS=0;
	RW=1;
	EN=0;
	_nop_();
	EN=1;
	return (bit)(P0&0x80);
}
void LCD_Write_Com(uchar com)
{
	while(LCD_Check_Busy());
	RS=0;
	RW=0;
	EN=1;
	P0=com;
	_nop_();
	EN=0;
}
void LCD_Write_Data(uchar Data)
{
	while(LCD_Check_Busy());
	RS=1;
	RW=0;
	EN=1;
	P0=Data;
	_nop_();
	EN=0;
}
void main()
{
	wxLE=1;
	P0=0;
	wxLE=0;
	dxLE=1;
	P0=0;
	dxLE=0;
	LCD_Write_Com(0x38);//显示模式设置
	LCD_Write_Com(0x01);//清屏，地址计数器清零，光标复位，显示缓冲区DDRAM的内容全部写入空格
	LCD_Write_Com(0x0c);//显示开启，不显示光标，光标不闪烁
	LCD_Write_Com(0x06);//读写一个字符过后，地址指针加1，光标加1


	while(1)
	{
		LCD_Write_Com(0x80);
		delay(10);
		for(i=0;i<12;i++)
		{
			LCD_Write_Data(table[i]);
			delay(10);
		}
		LCD_Write_Com(0xc0);
		delay(10);
		for(i=0;i<13;i++)
		{
			LCD_Write_Data(table1[i]);
			delay(10);
		}
		/*for(i=0;i<16;i++) //循环左移显示，此时不需要加下面的while(1)语句
		{
			LCD_Write_Com(0x18);
			delay(300);
		}*/
		while(1);//保持显示稳定，不要改语句后，程序从头开始执行会间断的显示
	}
}
