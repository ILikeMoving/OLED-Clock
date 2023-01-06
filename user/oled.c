/***********************************
	本驱动文件仅适配HAL库版本
***********************************/
#include "codetab.h"	//字库文件
#include "oled.h"		//声明


//oled显示尺寸
uint16_t const displayWidth                = 128;
uint16_t const displayHeight               = 64;

/*  OLED显存
[0]0 1 2 3 ... 127	
[1]0 1 2 3 ... 127	
[2]0 1 2 3 ... 127	
[3]0 1 2 3 ... 127	
[4]0 1 2 3 ... 127	
[5]0 1 2 3 ... 127	
[6]0 1 2 3 ... 127	
[7]0 1 2 3 ... 127 */
static uint8_t OLED_RAM[8][128];//定义GDDRAM缓存区

void Soft_I2C_WriteByte(uint8_t addr,uint8_t data)
{
	Soft_IIC_Start();
	Soft_IIC_SendByte(0x78);         //从机地址与读写位
	Soft_IIC_ReceiveACK();
	Soft_IIC_SendByte(addr);
	Soft_IIC_ReceiveACK();
	Soft_IIC_SendByte(data);
	Soft_IIC_ReceiveACK();
	Soft_IIC_Stop();
}

/**************************************************************
	 Prototype      : void WriteCmd(uint8_t IIC_Command)
	 Parameters     : IIC_Command
	 return					: none
	 Description    : 写命令
***************************************************************/
void WriteCmd(uint8_t IIC_Command)
{
	Soft_I2C_WriteByte(0x00, IIC_Command);
}

/**************************************************************
	 Prototype      : void WriteDat(uint8_t IIC_Data)
	 Parameters     : IIC_Data
	 return					: none
	 Description    : 写数据
***************************************************************/
void WriteDat(uint8_t IIC_Data)
{
	Soft_I2C_WriteByte(0x40, IIC_Data);
}

/**************************************************************
	 Prototype      : void OLED_Init(void)
	 Parameters     : none
	 return					: none
	 Description    : 初始化OLED模块
***************************************************************/
void OLED_Init(void) 
{
	HAL_Delay(500);
	
	WriteCmd(0xAE); //开显示
	WriteCmd(0x20);	//设置内存寻址模式	
	
	WriteCmd(0x00);	//00，水平寻址模式;01，垂直寻址模式;10，页面寻址模式(重置);11，无效
	WriteCmd(0x21); //设置列开始和结束地址
	WriteCmd(0x00); //列起始地址,范围:0 – 127d (默认值 = 0d)
	WriteCmd(0x7F); //列结束地址,范围:0 – 127d (默认值 = 127d)
	WriteCmd(0x22); //页面设置开始和结束地址
	WriteCmd(0x00); //页面起始地址,范围:0-7d (默认值= 0d)
	WriteCmd(0x07); //页面结束地址,范围:0-7d (默认值= 7d)

	WriteCmd(0xc8);	//设置COM输出扫描方向
	WriteCmd(0x40); //--设置起始行地址
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--设置段重新映射0到127
	WriteCmd(0xa6); //--设置正常显示
	WriteCmd(0xa8); //--设置复用比(1 ~ 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,输出遵循RAM内容;0xa5,Output忽略RAM内容
	WriteCmd(0xd3); //-设置显示抵消
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--设置显示时钟分频/振荡器频率
	WriteCmd(0xf0); //--设置分率
	WriteCmd(0xd9); //--设置pre-charge时期
	WriteCmd(0x22); //
	WriteCmd(0xda); //--设置com大头针硬件配置
	WriteCmd(0x12);
	WriteCmd(0xdb); //--设置vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--设置DC-DC
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--打开oled面板
	
	OLED_FullyClear();//清屏
}

/**************************************************************
	 Prototype      : void OLED_ON(void)
	 Parameters     : none
	 return					: none
	 Description    : 将OLED从休眠中唤醒
***************************************************************/
void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

/**************************************************************
	 Prototype      : void OLED_OFF(void)
	 Parameters     : none
	 return					: none
	 Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
***************************************************************/
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵
	WriteCmd(0XAE);  //OLED休眠
}

/**************************************************************
	 Prototype      : void OLED_RefreshRAM(void)
	 Parameters     : none
	 return					: none
	 Description    : 全屏填充
***************************************************************/
void OLED_RefreshRAM(void)
{
	for(uint16_t m = 0; m < displayHeight/8; m++)
	{
		for(uint16_t n = 0; n < displayWidth; n++)
		{
				WriteDat(OLED_RAM[m][n]);
		}
	}
}

/**************************************************************
	 Prototype      : void OLED_RefreshRAM_Test(void)
	 Parameters     : Page_Start    页开始地址(最小为0)
     Parameters     : Page_Stop     页结束地址(最大为7)
     Parameters     : Column_Start  列开始地址(最小为0)
     Parameters     : Column_Stop   列结束地址(最大为127)
	 return			: none
	 Description    : 区域填充
***************************************************************/
void OLED_RefreshPartRAM(uint8_t Page_Start, uint8_t Page_Stop, uint8_t Column_Start, uint8_t Column_Stop)
{
	WriteCmd(0X21);  //设置列地址
	WriteCmd(Column_Start);  //列开始地址
	WriteCmd(Column_Stop);  //列结束地址
	
	WriteCmd(0X22);  //设置页地址
	WriteCmd(Page_Start);  //页开始地址
	WriteCmd(Page_Stop);  //页结束地址
	
    for(uint16_t m = Page_Start; m < (Page_Stop + 1); m++)
	{
		for(uint16_t n = Column_Start; n < (Column_Stop + 1); n++)
		{
				WriteDat(OLED_RAM[m][n]);
		}
	}
}

/**************************************************************
	 Prototype      : void OLED_ClearRAM(void)
	 Parameters     : none
	 return					: none
	 Description    : 清除数据缓冲区
***************************************************************/
void OLED_ClearRAM(void)
{
	for(uint16_t m = 0; m < displayHeight/8; m++)
	{
		for(uint16_t n = 0; n < displayWidth; n++)
		{
				OLED_RAM[m][n] = 0x00;
		}
	}
}


/**************************************************************
	 Prototype      : void OLED_Fill(uint8_t fill_Data)
	 Parameters     : fill_Data 填充的1字节数据
	 return					: none
	 Description    : 全屏填充 0x00~0xff
***************************************************************/
void OLED_FullyFill(uint8_t fill_Data)
{
	for(uint16_t m = 0; m < displayHeight/8; m++)
	{
		for(uint16_t n = 0; n < displayWidth; n++)
		{
				OLED_RAM[m][n] = fill_Data;
		}
	}
	
	OLED_RefreshRAM();
}

/**************************************************************
	 Prototype      : void OLED_FullyClear(void)
	 Parameters     : none
	 return					: none
	 Description    : 全屏清除
***************************************************************/
void OLED_FullyClear(void)
{
		OLED_FullyFill(RESET_PIXEL);
}

/**************************************************************
	Prototype      :  void OLED_SetPixel(int16_t x, int16_t y, uint8_t set_pixel)
	Parameters     : 	x,y -- 起始点坐标(x:0~127, y:0~63); 
										set_pixel	 该点的数据  SET_PIXEL = 1, RESET_PIXEL = 0
	return				 :  none
	Description    : 	设置坐标像素点数据
***************************************************************/
void OLED_SetPixel(int16_t x, int16_t y, uint8_t set_pixel)
{ 
	if (x >= 0 && x < displayWidth && y >= 0 && y < displayHeight) {
		if(set_pixel){
				OLED_RAM[y/8][x] |= (0x01 << (y%8));
		}  
		else{
				OLED_RAM[y/8][x] &= ~(0x01 << (y%8));
		}
	}
}

/**************************************************************
	Prototype      :  void OLED_GetPixel(int16_t x, int16_t y)
	Parameters     : 	x,y -- 起始点坐标(x:0~127, y:0~63); 
	return				 :  PixelStatus 像素点状态 	SET_PIXEL = 1, RESET_PIXEL = 0
	Description    : 	获得坐标像素点数据	
***************************************************************/
PixelStatus OLED_GetPixel(int16_t x, int16_t y)
{
	 if(OLED_RAM[y/8][x] >> (y%8) & 0x01)
		 return SET_PIXEL;
	 
	return	RESET_PIXEL;
}

/**************************************************************
	Prototype      : void OLED_ShowStr(int16_t x, int16_t y, uint8_t ch[], uint8_t TextSize)
	Parameters     : 	x,y -- 起始点坐标(x:0~127, y:0~63); 
										ch[] -- 要显示的字符串; 
										TextSize -- 字符大小(1:6*8 ; 2:8*16)
	return				 :  none
	Description    : 	显示codetab.h中的ASCII字符,有6*8和8*16可选择
***************************************************************/
void OLED_ShowStr(int16_t x, int16_t y, uint8_t ch[], uint8_t TextSize)
{ 
	if (x >= 0 && x < displayWidth && y >= 0 && y < displayHeight) {
		int32_t c = 0;
		uint8_t j = 0;
	
		switch(TextSize)
		{
			case 1:
			{
				while(ch[j] != '\0')
				{
					c = ch[j] - 32;
					if(c < 0)	//无效字符
						break;
					
					if(x >= 125 || (127-x < 6))//一行最大显示字符数：21字节显示,多出两列，不显示 || 剩余列小于6不能显示完整字符，换行显示
					{
						x = 0;
						y += 8;//换行显示
						if(63 - y < 8)	// 不足以显示一行时不显示
							break;
					}
					for(uint8_t m = 0; m < 6; m++)
					{
						for(uint8_t n = 0; n < 8; n++)
						{
							OLED_SetPixel(x+m, y+n, (F6x8[c][m] >> n) & 0x01);
						}
					}
					x += 6;
					j++;
				}
			}break;
			case 2:
			{
				while(ch[j] != '\0')
				{
					c = ch[j] - 32;
					if(c < 0)	//无效字符
						break;
					
					if(x >= 127 || (127-x < 8))//16字节显示 || 剩余列小于8不能显示完整字符，换行显示
					{
						x = 0;
						y += 16;//换行显示
						if(63 - y < 16)	// 不足以显示一行时不显示
							break;
					}
					for(uint8_t m = 0; m < 2; m++)
					{
						for(uint8_t n = 0; n < 8; n++)
						{
							for(uint8_t i = 0; i < 8; i++)
							{
									OLED_SetPixel(x+n, y+i+m*8, (F8X16[c][n+m*8] >> i) & 0x01);
							}
						}	
					}
					x += 8;
					j++;
				}
			}break;
		}
	}
}

/**************************************************************
	 Prototype      : void OLED_ShowCN(int16_t x, int16_t y, uint8_t* ch)
	 Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); 
										CN[]:汉字在codetab.h中的索引
	 return				  : none
	 Description    : 显示codetab.h中的汉字,16*16点阵
***************************************************************/
void OLED_ShowCN(int16_t x, int16_t y, uint8_t* ch)
{
	if (x >= 0 && x < displayWidth && y >= 0 && y < displayHeight) {
		int32_t  len = 0;
		
		while(ch[len] != '\0')
		{
			if(x >= 127 || (127-x < 16))//8个汉字显示||剩余列小于16不能显示完整字符，换行显示
			{
				x = 0;
				y += 16;
				if(63 - y < 16)	// 不足以显示一行时不显示
					break;
			}
					
			//需要处理输入数据大于显示数据的问题
			for(uint8_t i = 0; i < sizeof(F16x16_CN)/sizeof(GB2312_CN); i++)
			{
				if(((F16x16_CN[i].index[0] == ch[len]) && (F16x16_CN[i].index[1] == ch[len+1]))){
						for(uint8_t m = 0; m < 2; m++)	//页
						{
								for(uint8_t n = 0; n < 16; n++) // 列
								{
										for(uint8_t j = 0; j < 8; j++)	// 行
										{
											OLED_SetPixel(x+n, y+j+m*8, (F16x16_CN[i].encoder[n+m*16] >> j) & 0x01);
										}
								}
						}			
						x += 16;
						len += 2;
						break;
				}
				else if(F16x16_CN[i].index[0] == ch[len] && ch[len] == 0x20){
					for(uint8_t m = 0; m < 2; m++)
					{
							for(uint8_t n = 0; n < 16; n++)
							{
								for(uint8_t j = 0; j < 8; j++)
								{
									OLED_SetPixel(x+n, y+j+m*8, (F16x16_CN[i].encoder[n+m*16] >> j) & 0x01);
								}								
							}	
					}			
					x += 16;
					len++;
					break;
				}
			}
		}
	}
}

/**************************************************************
	 Prototype      : void OLED_Show_MixedCH(int16_t x, int16_t y, uint8_t* ch)
	 Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); CN[]:汉字在codetab.h中的索引
	 return				  : none
	 Description    : 显示codetab.h中的汉字,16*16点阵,英文,8*16点阵
***************************************************************/
void OLED_ShowMixedCH(int16_t x, int16_t y, uint8_t* ch)
{
	if (x >= 0 && x < displayWidth && y >= 0 && y < displayHeight) {
		int32_t len = 0, c;

		while(ch[len] != '\0')
		{
			if(ch[len] >= 0xa1)//GB2312从0xA1A0开始
			{
				for(uint8_t i = 0; i < sizeof(F16x16_CN)/sizeof(GB2312_CN); i++)
				{
					if(((F16x16_CN[i].index[0] == ch[len]) && (F16x16_CN[i].index[1] == ch[len+1])))
					{
						if(x >= 127|| (127-x < 16))//8个汉字显示||剩余列小于16不能显示完整字符，换行显示
						{
							x = 0;
							y += 16;
							if(63 - y < 16)	// 不足以显示一行时不显示
								break;
						}
						for(uint8_t m = 0; m < 2; m++)	//页
						{
								for(uint8_t n = 0; n < 16; n++)	//列
								{
									for(uint8_t j = 0; j < 8; j++)	//行
									{
	
										OLED_SetPixel(x+n, y+j+m*8, (F16x16_CN[i].encoder[n+m*16] >> j) & 0x01);
									}		
								}								
						}			
						x += 16;
						len += 2;
						break;
					}
				}
			}
			else if(ch[len] <= 127)//ASCII编码范围0-127
			{
				c = ch[len] - 32;
				if(c < 0)	// 无效字符
					break;
				if(x >= 127 || (127-x < 8))//16字节显示 || 剩余列小于8不能显示完整字符，换行显示
				{
					x = 0;
					y += 16;
					if(63 - y < 16)	// 不足以显示一行时不显示
								break;
				}
				for(uint8_t m = 0; m < 2; m++)
				{
						for(uint8_t n = 0; n < 8; n++)
						{
							for(uint8_t i = 0; i < 8; i++)
							{
									OLED_SetPixel(x+n, y+i+m*8, (F8X16[c][n+m*8] >> i) & 0x01);
							}
						}
				}
				x += 8;
				len++;
			}
		}
	}
}

/***************************************************************
	 Prototype      :	void OLED_DrawBMP(int16_t x0,int16_t y0,int16_t L,int16_t H,const uint8_t BMP[])
	 Parameters     : (x0,y0)坐标长L宽H区域绘制图像BMP
										0<=x0<=127 0<=y0<=63 0<=L+x0<=127 0<=H+y0<= 63 图像取模 纵向取模，字节倒序
	 return				  : none
	 Description    : 区域图像绘制，显示BMP位图,格式使用二维数组存储
***************************************************************/
void OLED_DrawBMP(int16_t x0,int16_t y0,int16_t L,int16_t H,const uint8_t BMP[])
{
	if (x0 >= 0 && x0 < displayWidth && x0+L <= displayWidth &&\
		y0 >= 0 && y0 < displayHeight && y0+H <= displayHeight) {
		
		uint8_t *p = (uint8_t *)BMP;
		for(int16_t y = y0; y < y0+H; y+=8)
		{
			for(int16_t x = x0; x < x0+L; x++)
			{
				for(int16_t i = 0; i < 8; i++)
				{
//					OLED_SetPixel(x, y+i, ((*((uint8_t *)BMP+(x-x0)+L*((y-y0)/8))) >> i) & 0x01);
						OLED_SetPixel(x, y+i, ((*p) >> i) & 0x01);
				}
				p++;
			}
		}
	}
}

/***************************************************************
	 Prototype      :	void OLED_AreaFill(int16_t x0,int16_t y0,int16_t L,int16_t H)
	 Parameters     : 区域内容清除，(x0,y0)坐标长L宽H区域
										0<=x0<=127 0<=y0<=63 0<=L+x0<=127 0<=H+y0<= 63 图像取模 纵向取模，字节倒序
	 return				  : none
	 Description    : 规定区域内容填充
***************************************************************/
void OLED_AreaFill(int16_t x0,int16_t y0,int16_t L,int16_t H, uint8_t fill_data)
{
	if (x0 >= 0 && x0 < displayWidth && x0+L <= displayWidth &&\
		y0 >= 0 && y0 < displayHeight && y0+H <= displayHeight) {
		
		for(int16_t y = y0; y < y0+H; y++)
		{
			for(int16_t x = x0; x < x0+L; x++)
			{
				for(int16_t i = 0; i < 8; i++)
					{
							OLED_SetPixel(x, y+i, (fill_data >> i) & SET_PIXEL);
					}
			}
		}
		OLED_RefreshRAM();
	}
}

/***************************************************************
	 Prototype      :	void OLED_AreaCLR(int16_t x0,int16_t y0,int16_t L,int16_t H)
	 Parameters     : (x0,y0)坐标长L宽H区域
										0<=x0<=127 0<=y0<=63 0<=L+x0<=127 0<=H+y0<= 63 图像取模 纵向取模，字节倒序
	 return				  : none
	 Description    : 规定区域内容清除
***************************************************************/
void OLED_AreaClear(int16_t x0,int16_t y0,int16_t L,int16_t H)
{
	if (x0 >= 0 && x0 < displayWidth && x0+L <= displayWidth &&\
		y0 >= 0 && y0 < displayHeight && y0+H <= displayHeight) {
		
		for(int16_t y = y0; y < y0+H; y+=8)
		{
			for(int16_t x = x0; x < x0+L; x++)
			{
				for(int16_t i = 0; i < 8; i++)
					{
							OLED_SetPixel(x, y+i, RESET_PIXEL);
					}
			}
		}		
		//OLED_RefreshRAM();
	}
}

/***************************************************************
	 Prototype      :	void OLED_FullyToggle(void)
	 Parameters     : none
	 return				  : none
	 Description    : 缓冲区数据取反后刷新到GDDRAM
***************************************************************/
void OLED_FullyToggle(void)
{
	for(uint16_t m = 0; m < displayHeight/8; m++)
	{
		for(uint16_t n = 0; n < displayWidth; n++)
		{
				OLED_RAM[m][n] = ~OLED_RAM[m][n];
		}
	}
	OLED_RefreshRAM();
}
/***************************************************************
	 Prototype      :	void OLED_AreaToggle(int16_t x0,int16_t y0,int16_t L,int16_t H)
	 Parameters     : (x0,y0)坐标长L宽H区域
										0<=x0<=127 0<=y0<=63 0<=L+x0<=127 0<=H+y0<= 63 图像取模 纵向取模，字节倒序
	 return				  : none
	 Description    : 规定区域内容取反
***************************************************************/
void OLED_AreaToggle(int16_t x0,int16_t y0,int16_t L,int16_t H)
{
	if (x0 >= 0 && x0 < displayWidth && x0+L <= displayWidth &&\
		y0 >= 0 && y0 < displayHeight && y0+H <= displayHeight) {
		
		for(int16_t y = y0; y < y0+H; y+=8)
		{
			for(int16_t x = x0; x < x0+L; x++)
			{
				for(int16_t i = 0; i < 8; i++)
					{
							OLED_SetPixel(x, y+i, !OLED_GetPixel(x, y+i));
					}
			}
		}		
		OLED_RefreshRAM();
	}
}

/****************************************************************
	全屏垂直偏移,0->63方向
	方向垂直向上,范围0-63
	方向垂直向下,范围63-0
****************************************************************/
void OLED_VerticalShift(void)
{
	for(uint8_t i = 0; i < displayHeight; i++)
		{
			WriteCmd(0xd3);//设置显示偏移，0->63方向
			WriteCmd(i);//偏移量
			HAL_Delay(40);//延时时间
		}
}

/****************************************************************
	屏幕内容水平全屏滚动播放
	左		LEFT	0x27
	右		RIGHT	0x26
****************************************************************/
void OLED_HorizontalShift(uint8_t direction)

{
	WriteCmd(direction);//设置滚动方向
	WriteCmd(0x00);//虚拟字节设置，默认为0x00
	WriteCmd(0x00);//设置开始页地址
	WriteCmd(0x05);//设置每个滚动步骤之间的时间间隔的帧频
	WriteCmd(0x07);//设置结束页地址
	WriteCmd(0x00);//虚拟字节设置，默认为0x00
	WriteCmd(0xff);//虚拟字节设置，默认为0xff
	WriteCmd(0x2f);//开启滚动-0x2f，禁用滚动-0x2e，禁用需要重写数据
}

/****************************************************************
	屏幕内容垂直水平全屏滚动播放
	上		UP		0x29
	下		DOWN	0x2A
****************************************************************/
void OLED_VerticalAndHorizontalShift(uint8_t direction)
{
	WriteCmd(direction);//设置滚动方向
	WriteCmd(0x00);//虚拟字节设置，默认为0x00
	WriteCmd(0x00);//设置开始页地址
	WriteCmd(0x05);//设置每个滚动步骤之间的时间间隔的帧频
	WriteCmd(0x07);//设置结束页地址
	WriteCmd(0x01);//垂直滚动偏移量
	
	WriteCmd(0x2f);//开启滚动-0x2f，禁用滚动-0x2e，禁用需要重写数据
}

/****************************************************************
	屏幕内容取反显示
	开	ON	0xA7
	关	OFF	0xA6	默认此模式，设置像素点亮
****************************************************************/
void OLED_DisplayMode(uint8_t mode)
{
	WriteCmd(mode);
}

/****************************************************************
	屏幕亮度调节
	intensity	0-255
	默认为0x7f
****************************************************************/
void OLED_IntensityControl(uint8_t intensity)
{
	WriteCmd(0x81);
	WriteCmd(intensity);
}

/***************************************************************
	 Prototype      : uint8_t GetPixel_For_Scroll(int16_t x, int16_t y, const uint8_t BMP[], uint8_t W)
	 Parameters     : x        横坐标
     Parameters     : y        纵坐标
     Parameters     : BMP[]    bmp图片（二维数组）
     Parameters     : W        bmp图片宽度（像素）
	 return			: 坐标点数据
	 Description    : 获得坐标像素点数据（可以为滚动动画服务）
***************************************************************/
uint8_t GetPixel_For_Scroll(int16_t x, int16_t y, const uint8_t BMP[], uint8_t W)
{
	uint8_t *p = (uint8_t *)BMP;
	p += x + (y/8)*W;
	if((*p) >> (y%8) & 0x01) return 1;
	return 0;
}

/***************************************************************
	 Prototype      : void SetPixel_for_ScrollDigit(int16_t X, int16_t Y, int16_t x, int16_t y, uint8_t set_pixel)
	 Parameters     : X                
     Parameters     : Y                
     Parameters     : x                
     Parameters     : y
     Parameters     : set_pixel        
	 return			: none
	 Description    : 设置坐标像素点数据（可以为滚动动画服务）
***************************************************************/
void SetPixel_For_Scroll(int16_t X, int16_t Y, int16_t x, int16_t y, uint8_t set_pixel)
{ 
		if(set_pixel)
		{
			OLED_RAM[(Y+y)/8][X+x] |= (0x01 << ((Y+y)%8));
		}  
		else
		{
			OLED_RAM[(Y+y)/8][X+x] &= ~(0x01 << ((Y+y)%8));
		}
}

/****************************************************************************************************************************************************
	 Prototype      : void Draw_Digit_BMP(uint16_t x1, uint16_t y1, const uint8_t BMP[], uint16_t Y,uint8_t W, uint8_t H,  uint16_t end_line)  
     Parameters     : x1                确定图片显示位置（左上角像素点横坐标）
     Parameters     : y1                确定图片显示位置（左上角像素点纵坐标）
     Parameters     : BMP[]             素材图片
     Parameters     : Y                 所选的一帧图片在素材图片中的纵坐标
     Parameters     : W                 素材图片宽度（也是一帧图片的宽度）
     Parameters     : H                 一帧图片的高度
     Parameters     : end_line          在素材图片中划出最后一行(用于滚动循环，首尾相接)
	 return			: none
	 Description    : 从bmp大图片中获取小图片作为滚动动画的一帧图片
*****************************************************************************************************************************************************/
void Draw_BMP_For_Scroll(uint16_t x1, uint16_t y1, const uint8_t BMP[], uint16_t Y, uint8_t W, uint8_t H, uint16_t end_line)
{
	uint16_t x0,y0,y,Temp;
	
	for(y = Y , y0 = 0 ; y0 < H ; y++ , y0++)
		{
			if(y > end_line) y -= (end_line+1);
			for(x0 = 0; x0 < W ; x0++)
			{
				Temp = GetPixel_For_Scroll(x0, y, BMP, W);
				SetPixel_For_Scroll(x1,y1,x0,y0,Temp);
			}
		}
	  
}
