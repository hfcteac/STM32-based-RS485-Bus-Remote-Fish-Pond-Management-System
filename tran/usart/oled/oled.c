#include "oled.h"
#include "i2c.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "oledfont.h"
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
/**********************************************
**********************************************/
//初始化命令
uint8_t CMD_Data[]={
0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F,
					
0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12,
					
0xD8, 0x30, 0x8D, 0x14, 0xAF};

void OLED_Init(void)
{
 uint8_t i=0;
 for(i=0; i<27; i++)
	{
 HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,CMD_Data+i,1,0x100);
  }
}

void OLED_WR_Cmd(uint8_t Cmd)
{
  HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,&Cmd,1,0x100);	
}

void OLED_WR_Data(uint8_t Data)
{
  HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&Data,1,0x100);	
}

void OLED_Clear(void)
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Cmd(0xb0+i);
		OLED_WR_Cmd (0x00); 
		OLED_WR_Cmd (0x10); 
		for(n=0;n<128;n++)
		OLED_WR_Data(0);
	} 
}

//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Cmd(0X8D);  //SET DCDC命令
	OLED_WR_Cmd(0X14);  //DCDC ON
	OLED_WR_Cmd(0XAF);  //DISPLAY ON
}

//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Cmd(0X8D);  //SET DCDC命令
	OLED_WR_Cmd(0X10);  //DCDC OFF
	OLED_WR_Cmd(0XAE);  //DISPLAY OFF
}
//坐标设置
void OLED_Set_Pos(uint8_t x, uint8_t y) 
{ 	
	OLED_WR_Cmd(0xb0+y);
	OLED_WR_Cmd(((x&0xf0)>>4)|0x10);
	OLED_WR_Cmd(x&0x0f);
} 

void OLED_Refresh(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Cmd (0xb0+i);    //设置页地址（0~7）
		OLED_WR_Cmd (0x00);      //设置显示位置—列低地址
		OLED_WR_Cmd (0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
		OLED_WR_Data(1); 
	} //更新显示
}
unsigned int oled_pow(uint8_t m,uint8_t n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>128-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Data(F8X16[c*16+i]);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Data(F8X16[c*16+i+8]);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Data(F6x8[c][i]);
				
			}
}

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
}
void OLED_Showfloat(uint8_t x,uint8_t y,float num,uint8_t size2)
{
	unsigned char temp[10];
	sprintf((char*)temp,"%.2f",num);
	if(num>0)
	{
		OLED_ShowString(x,y,(uint8_t*)" ",size2);
		OLED_ShowString(x + 8,y,temp,size2);
	}
	else
		OLED_ShowString(x,y,temp,size2);
}
//显示一个字符号串
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

//显示汉字
void OLED_ShowCHinese(uint8_t y,uint8_t x,uint8_t no)
{      			    
	uint8_t t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Data(Hzk[2*no][t]);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Data(Hzk[2*no+1][t]);
				adder+=1;
      }					
}

void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Data(0xb0+m);		//page0-page1
		OLED_WR_Data(0x00);		//low column start address
		OLED_WR_Data(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_WR_Data(fill_Data);
			}
	}
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Data(BMP[j++]);	    	
	    }
	}
}


