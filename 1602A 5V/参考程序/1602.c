/***************************************************************************
标题：	LCD1602
效果：	本程序在液晶LCD1602上显示两行字符
工作于：LY5A-L2A开发板
作者：	林洋电子-单片机
客服QQ：52927029
邮箱：	52927029@qq.com
******************************************************************************/
#include "reg51.h"

/********IO引脚定义***********************************************************/
sbit LCD_RS=P1^0;//定义引脚
sbit LCD_RW=P1^1;
sbit LCD_E=P1^2;

/********宏定义***********************************************************/
#define LCD_Data P0
#define Busy    0x80 //用于检测LCD状态字中的Busy标识

/********数据定义*************************************************************/
unsigned char code uctech[] = {"Happy every day"};
unsigned char code net[] = {"www.lydz8.com"};

/********函数声明*************************************************************/
void WriteDataLCD(unsigned char WDLCD);					//写数据
void WriteCommandLCD(unsigned char WCLCD,BuysC);		//写命令
unsigned char ReadDataLCD(void);						//读数据
unsigned char ReadStatusLCD(void);						//读状态
void LCDInit(void);										//初始化
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData);			//相应坐标显示字节内容
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData);	//相应坐标开始显示一串内容
void Delay5Ms(void);									//延时
void Delay400Ms(void);									//延时


/***********主函数开始********************************************************/	
void main(void)
{
	Delay400Ms(); 	//启动等待，等LCD讲入工作状态
	LCDInit(); 		//初始化
	Delay5Ms(); 	//延时片刻(可不要)

	DisplayListChar(0, 0, uctech);
	DisplayListChar(1, 5, net);
	ReadDataLCD();	//测试用句无意义
	while(1);
}

/***********写数据********************************************************/	
void WriteDataLCD(unsigned char WDLCD)
{
 	ReadStatusLCD(); //检测忙
 	LCD_Data = WDLCD;
 	LCD_RS = 1;
 	LCD_RW = 0;
 	LCD_E = 0; 		//若晶振速度太高可以在这后加小的延时
 	LCD_E = 0; 		//延时
 	LCD_E = 1;
}

/***********写指令********************************************************/	
void WriteCommandLCD(unsigned char WCLCD,BuysC) //BuysC为0时忽略忙检测
{
 	if (BuysC) ReadStatusLCD(); //根据需要检测忙
 	LCD_Data = WCLCD;
 	LCD_RS = 0;
 	LCD_RW = 0; 
 	LCD_E = 0;
 	LCD_E = 0;
	LCD_E = 1; 
}

/***********读数据********************************************************/	
unsigned char ReadDataLCD(void)
{
 	LCD_RS = 1; 
 	LCD_RW = 1;
 	LCD_E = 0;
 	LCD_E = 0;
 	LCD_E = 1;
 	return(LCD_Data);
}

/***********读状态*******************************************************/	
unsigned char ReadStatusLCD(void)
{
 	LCD_Data = 0xFF; 
 	LCD_RS = 0;
 	LCD_RW = 1;
 	LCD_E = 0;
 	LCD_E = 0;
 	LCD_E = 1;
 	while (LCD_Data & Busy); //检测忙信号
 	return(LCD_Data);
}

/***********初始化********************************************************/	
void LCDInit(void)
{
 	LCD_Data = 0;
 	WriteCommandLCD(0x38,0); 	//三次模式设置，不检测忙信号
 	Delay5Ms(); 
 	WriteCommandLCD(0x38,0);
 	Delay5Ms(); 
 	WriteCommandLCD(0x38,0);
 	Delay5Ms(); 

 	WriteCommandLCD(0x38,1); 	//显示模式设置,开始要求每次检测忙信号
 	WriteCommandLCD(0x08,1); 	//关闭显示
 	WriteCommandLCD(0x01,1); 	//显示清屏
 	WriteCommandLCD(0x06,1); 	//显示光标移动设置
 	WriteCommandLCD(0x0C,1); 	//显示开及光标设置
}

/***********按指定位置显示一个字符*******************************************/	
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
 	Y &= 0x1;
 	X &= 0xF; 				//限制X不能大于15，Y不能大于1
 	if (Y) X |= 0x40; 		//当要显示第二行时地址码+0x40;
 	X |= 0x80; 			//算出指令码
 	WriteCommandLCD(X, 0); //这里不检测忙信号，发送地址码
 	WriteDataLCD(DData);
}

/***********按指定位置显示一串字符*****************************************/	
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
 	unsigned char ListLength;

 	ListLength = 0;
 	Y &= 0x1;
 	X &= 0xF; 				//限制X不能大于15，Y不能大于1
 	while (DData[ListLength]>=0x20){ //若到达字串尾则退出
   		if (X <= 0xF){ 		//X坐标应小于0xF
     		DisplayOneChar(X, Y, DData[ListLength]); //显示单个字符
     		ListLength++;
     		X++;
    	}
  	}
}

/***********短延时********************************************************/	
void Delay5Ms(void)
{
 	unsigned int TempCyc = 5552;
 	while(TempCyc--);
}

/***********长延时********************************************************/	
void Delay400Ms(void)
{
 	unsigned char TempCycA = 5;
 	unsigned int TempCycB;
 	while(TempCycA--){
  		TempCycB=7269;
  		while(TempCycB--);
 	}
}
