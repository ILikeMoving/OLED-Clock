#include "draw_rolling_clock.h"


uint8_t Hour=0,Minute=59,Second=55;//时间参数
uint8_t H1,H2,M1,M2,S1,S2;//时钟时分秒六位数字

static uint16_t Y1=0,Y2=0,Y3=0,Y4=0,Y5=0,Y6=0;
static uint8_t TEMP;

void Draw_Rolling_Clock()
{
	switch(H1)
	{
		case 0:if(Y1 < 24*2+1) Y1 = 24*2;if(Y1 < 24*2+24) Y1++;break;
		case 1:
		case 2:if(Y1 < H1*24-23 || Y1 > H1*24) Y1 = H1*24-24;if(Y1 < H1*24) Y1++;
	}
	Draw_BMP_For_Scroll(0, 16, Scroll_Digit_BMP[0], Y1, 20, 24, 2*24+23);//end_line=2*24+23,Scroll_Digit_Small_BMP划到2*24+23行，即0~2
	
	switch(H2)
	{
		case 0:
		{
			if(Hour == 0) {TEMP = 3;if(Y2 < 24*3+1 || Y2 > 4*24) Y2 = 24*3;if(Y2 < 24*3+24) Y2++;break;}
			if(Hour == 10 || Hour == 20){TEMP = 9;if(Y2 < 24*9+1) Y2 = 24*9;if(Y2 < 24*9+24) Y2++;break;} 
		}
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:if(Y2 < H2*24-23 || Y2 > H2*24) Y2 = H2*24-24;if(Y2 < H2*24) Y2++;TEMP = 9;//if(Hour == 23) TEMP = 3;else TEMP = 9;
	}
	Draw_BMP_For_Scroll(22, 16, Scroll_Digit_BMP[0], Y2, 20, 24, TEMP*24+23);//end_line=Temp*24+23,Scroll_Digit_Small_BMP划到Temp*24+23行，即0~Temp
	
	switch(M1)
	{
		case 0:if(Y3 < 24*5+1) Y3 = 24*5;if(Y3 < 24*5+24) Y3++;break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:if(Y3 < M1*24-23 || Y3 > M1*24) Y3 = M1*24-24;if(Y3 < M1*24) Y3++;
	}
	Draw_BMP_For_Scroll(50, 16, Scroll_Digit_BMP[0], Y3, 20, 24, 5*24+23);//end_line=5*24+23,Scroll_Digit_Small_BMP划到5*24+23行，即0~5
	
	switch(M2)
	{
		case 0:if(Y4 < 24*9+1) Y4 = 24*9;if(Y4 < 24*9+24) Y4++;break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:if(Y4 < M2*24-23 || Y4 > M2*24) Y4 = M2*24-24;if(Y4 < M2*24) Y4++;
	}
	Draw_BMP_For_Scroll(72, 16, Scroll_Digit_BMP[0], Y4, 20, 24, 9*24+23);//end_line=9*24+23,Scroll_Digit_Small_BMP划到9*24+23行，即0~9
	
	switch(S1)
	{
		case 0:if(Y5 < 16*5+1) Y5 = 16*5;if(Y5 < 16*5+16) Y5++;break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:if(Y5 < S1*16-15 || Y5 > S1*16) Y5 = S1*16-16;if(Y5 < S1*16) Y5++;
	}
	Draw_BMP_For_Scroll(94, 24, Scroll_Digit_Small_BMP[0], Y5, 14, 16, 5*16+15);//end_line=6*16+15,Scroll_Digit_Small_BMP划到6*16+15行，即0~6
	
	switch(S2)
	{
		case 0:if(Y6 < 16*9+1) Y6 = 16*9;if(Y6 < 16*9+16) Y6++;break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:if(Y6 < S2*16-15 || Y6 > S2*16) Y6 = S2*16-16;if(Y6 < S2*16) Y6++;
	}
	Draw_BMP_For_Scroll(111, 24, Scroll_Digit_Small_BMP[0], Y6, 14, 16, 9*16+15);//end_line=9*16+15,Scroll_Digit_Small_BMP划到9*16+15行，即0~9
	
	if(Second % 2 == 1) OLED_DrawBMP(44,16,4,24,Colon_BMP[0]);      //绘制冒号
	else OLED_AreaClear(44,16,4,24);                                //清除冒号
	
	OLED_RefreshPartRAM(2,4,0,127);
}
