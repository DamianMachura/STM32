


#include "stm32f4xx_hal.h"

/********** Definicions of Comands on LCD *********/

#define     LCD_CLEAR	    0x01
#define     LCD_HOME	    0x02
#define     LCD_CURSOR_ON	0x0F
#define     LCD_CURSOR_OFF	0x0C
#define     LCD_LINEA1	    0x00
#define     LCD_LINEA2		0XC0
#define     LCD_LINEA3		0x94
#define     LCD_LINEA4		0xD4
#define     LCD_LEFT		0x10
#define     LCD_RIGHT		0x14
#define     LCD_ROT_LEFT	0x18
#define     LCD_ROT_RIGHT	0x1C
#define     LCD_OFF 		0x08

/************** func declarations  *****************/

void LCD_init(void);
void LCD_write_nible(unsigned char);
void LCD_cmd(unsigned char comando);
void LCD_char(char caracter);           //character
void LCD_write_text(char *dato);
void LCD_gotoxy(char x, char y);        //cursor position
void LCD_custom_char(unsigned char loc, unsigned char *msg);
void LCD_write_number(float number);

void delay(void);

void delay(void)
{
	uint16_t i;
	for(i=0;i<2000;i++);
}

/*********************************************************
---------------- Initialize LCD -------------------
**********************************************************/

void LCD_init(void)
{
    HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_RESET);


    HAL_Delay(20);

    LCD_write_nible(0x03);
    HAL_Delay(5);
    LCD_write_nible(0x03);
    HAL_Delay(1);//_delay_us(100);
    LCD_write_nible(0x03);
    HAL_Delay(1);//_delay_us(100);
    LCD_write_nible(0x02);
    HAL_Delay(1); //_delay_us(100);

    LCD_cmd(0x28);
    LCD_cmd(LCD_CURSOR_OFF);
    LCD_cmd(LCD_CLEAR);
    LCD_cmd(0x06);
    LCD_cmd(0x80);
}



void LCD_write_nible(unsigned char nible)
{
    if (nible & (1<<0))
        HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_RESET);

    if (nible & (1<<1))
        HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_RESET);

    if (nible & (1<<2))
        HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
    else
         HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_RESET);

    if (nible & (1<<3))
        HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_SET);
    else
         HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET);
    delay();//HAL_Delay(1);//_delay_us(1);	                            //Enable pulse width PWeh min 230ns
    HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);
}

/****************************************************
 ---------- Envia comando al LCD ----------
 ****************************************************/

void LCD_cmd(unsigned char comando)
{
    HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET);
    LCD_write_nible(comando >> 4);
    LCD_write_nible(comando & 0x0F);
    if(comando == LCD_CLEAR || comando == LCD_HOME)
    	HAL_Delay(1);//HAL_Delay(2);              //Tiempo m�nimo 1.6ms
    else
    	 delay();//HAL_Delay(1);//_delay_us(55);
}



void LCD_char(char caracter)
{
    HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_SET);
    LCD_write_nible(caracter >> 4);
    LCD_write_nible(caracter & 0x0F);
    delay();//HAL_Delay(1);//_delay_us(55);
}

/*************************************************
 ------ Display on lcd--------
 ************************************************/

void LCD_write_text(char *dato)
{
    while (*dato){
        LCD_char(*dato);    // Envoy dato to char
        dato++;             // Iincrement buffer of dato
    }
}



void LCD_gotoxy(char x, char y)
{
    char posicion;
    switch (y)
    {
        case 1: posicion = 0x00 + x - 1; break;
        case 2: posicion = 0x40 + x - 1; break;
        case 3: posicion = 0x14 + x - 1; break;
        case 4: posicion = 0x54 + x - 1; break;
        default: posicion = 0x00 + x - 1; break;
    }
    LCD_cmd(0x80 | posicion);
}



void LCD_custom_char(unsigned char loc,unsigned char *msg)
{
    unsigned char i;
    if(loc<8){
        LCD_cmd(0x40+(loc*8));  /* Command 0x40 and onwards forces the device to point CGRAM address */
        for(i=0;i<8;i++)        /* Write 8 byte for generation of 1 character */
            LCD_char(msg[i]);
    }
}
void LCD_write_number(float number)
{
	char numStr[16];
	sprintf(numStr,"%.1f", number);
	LCD_write_text(numStr);
}
