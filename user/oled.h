#ifndef __OLED_H__
#define __OLED_H__

#include "stm32f1xx_hal.h"	//链接HAL库
#include "soft_iic.h"       //软件IIC


/* 控制宏 */
#define LEFT 			0x27
#define RIGHT 			0x26
#define UP 				0X29
#define DOWM 			0x2A
#define ON				0xA7
#define OFF				0xA6


/* BMP图片声明 
	图片格式为二位数组，下标分别对应图片的宽和高：
		BMP_xx[H/8][L];
*/
extern const uint8_t BMP_Picture[32/8][32];

/* 箭头滚动测试 */
extern const uint8_t Test_BMP[48/8][24];

/* 数字滚动时钟的BMP素材图片 */
extern const uint8_t Scroll_Digit_BMP[240/8][20];          //大数字
extern const uint8_t Scroll_Digit_Small_BMP[160/8][14];    //小数字
extern const uint8_t Colon_BMP[24/8][4];                   //冒号


/* 设置坐标点的状态 */
typedef enum 
{
	SET_PIXEL = 0x01,
  RESET_PIXEL = 0x00, 
} PixelStatus;


/* 功能函数声明 */
//写数据，软件IIC使用
void Soft_I2C_WriteByte(uint8_t addr,uint8_t data);
//写命令
void WriteCmd(uint8_t IIC_Command);
//写数据
void WriteDat(uint8_t IIC_Data);
//初始化OLED
void OLED_Init(void);
//开启电荷泵
void OLED_ON(void);
//关闭电荷泵
void OLED_OFF(void);
//刷新缓冲区数据到GDDRAM
void OLED_RefreshRAM(void);
//刷新部分缓冲区数据到部分GDDRAM
void OLED_RefreshPartRAM(uint8_t Page_Start, uint8_t Page_Stop, uint8_t Column_Start, uint8_t Column_Stop);
//清除数据缓冲区OLED_RAM buffer
void OLED_ClearRAM(void);
//全屏填充
void OLED_FullyFill(uint8_t fill_Data);
//清屏
void OLED_FullyClear(void);
//设置坐标像素点数据
void OLED_SetPixel(int16_t x, int16_t y, uint8_t set_pixel);
//获得坐标像素点数据
PixelStatus OLED_GetPixel(int16_t x, int16_t y);

/* 显示指定字符和图片时需要手动刷新缓冲区到GDDRAM 
* function list: OLED_ShowStr\OLED_ShowCN\OLED_Show_MixedCH\OLED_DrawBMP
*/
//显示英文字符串
void OLED_ShowStr(int16_t x, int16_t y, uint8_t ch[], uint8_t TextSize);
//显示中文字符串
void OLED_ShowCN(int16_t x, int16_t y, uint8_t* ch);
//显示中英文混合文字
void OLED_ShowMixedCH(int16_t x, int16_t y, uint8_t* ch);
//显示图片
void OLED_DrawBMP(int16_t x0,int16_t y0,int16_t L,int16_t H,const uint8_t BMP[]);

//区域填充
void OLED_AreaFill(int16_t x0,int16_t y0,int16_t L,int16_t H, uint8_t fill_data);
//区域清除
void OLED_AreaClear(int16_t x0,int16_t y0,int16_t L,int16_t H);
//全屏切换显示
void OLED_FullyToggle(void);
//区域切换显示
void OLED_AreaToggle(int16_t x0,int16_t y0,int16_t L,int16_t H);
//全屏垂直滚动播放
void OLED_VerticalShift(void);
//全屏水平滚动播放
void OLED_HorizontalShift(uint8_t direction);
//全屏同时垂直和水平滚动播放
void OLED_VerticalAndHorizontalShift(uint8_t direction);
//屏幕内容取反显示
void OLED_DisplayMode(uint8_t mode);
//屏幕亮度调节
void OLED_IntensityControl(uint8_t intensity);

//获得坐标像素点数据（可以为滚动动画服务）
uint8_t GetPixel_For_Scroll(int16_t x, int16_t y, const uint8_t BMP[], uint8_t W);
//设置坐标像素点数据（可以为滚动动画服务）
void SetPixel_For_Scroll(int16_t X, int16_t Y, int16_t x, int16_t y, uint8_t set_pixel);
//从bmp大图片中获取小图片作为滚动动画的一帧图片
void Draw_BMP_For_Scroll(uint16_t x1, uint16_t y1, const uint8_t BMP[], uint16_t Y, uint8_t W, uint8_t H, uint16_t end_line);


#endif /* __OLED_H__ */

