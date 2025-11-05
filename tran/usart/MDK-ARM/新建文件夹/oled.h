#ifndef __oled_H
#define __oled_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */



/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void OLED_Init(void);
void OLED_WR_Cmd(uint8_t Cmd);
void OLED_WR_Data(uint8_t Data);
void OLED_Clear(void);   
void OLED_Display_On(void);   
void OLED_Display_Off(void);
void OLED_Set_Pos(uint8_t x, uint8_t y); 
void OLED_Refresh(void);  
void OLED_Showfloat(uint8_t x,uint8_t y,float num,uint8_t size2);
unsigned int oled_pow(uint8_t m,uint8_t n);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);		  
void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void fill_picture(unsigned char fill_Data);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif 

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
