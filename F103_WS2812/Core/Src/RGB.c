#include "RGB.h"
#include "tim.h"
#include "stdlib.h"

/*Some Static Colors------------------------------*/
const RGB_Color_TypeDef RED      = {255,0,0};   //??
const RGB_Color_TypeDef GREEN    = {0,255,0};   //??
const RGB_Color_TypeDef BLUE     = {0,0,255};   //???
const RGB_Color_TypeDef SKY      = {0,255,255};  //???
const RGB_Color_TypeDef MAGENTA  = {255,0,220};  //??
const RGB_Color_TypeDef YELLOW   = {128,216,0};  //??
const RGB_Color_TypeDef OEANGE   = {127,106,0};  //??
const RGB_Color_TypeDef BLACK    = {0,0,0};    //???
const RGB_Color_TypeDef WHITE    = {255,255,255}; //??

//???????????,?????????
RGB_Color_TypeDef table[16] = 
{
   {254,67,101}, 
   {76,0,10},
   {249,15,173},
   {128,0,32},
   {158,46,36},
   {184,206,142},
   {227,23,13},
   {178,34,34},
   {255,99,71},
   {99,38,18},
   {255,97,0},
   {21,161,201},
   {56,94,15},
   {50,205,50},
   {160,32,240},
    {218,60,90}
};
 //????????
const RGB_Color_TypeDef color1 = {254,67,101};
//const RGB_Color_TypeDef color2 = {76,0,10};
//const RGB_Color_TypeDef color3 = {249,15,173};
//const RGB_Color_TypeDef color4 = {128,0,32};
//const RGB_Color_TypeDef color5 = {158,46,36};
//const RGB_Color_TypeDef color6 = {184,206,142};
//const RGB_Color_TypeDef color7 = {227,23,13};
//const RGB_Color_TypeDef color8 = {178,34,34};
//const RGB_Color_TypeDef color9 = {255,99,71};
//const RGB_Color_TypeDef color10 ={99,38,18};
//const RGB_Color_TypeDef color11= {255,97,0};
//const RGB_Color_TypeDef color12= {21,161,201};
//const RGB_Color_TypeDef color13= {56,94,15};
//const RGB_Color_TypeDef color14= {50,205,50};
//const RGB_Color_TypeDef color15= {160,32,240};
//const RGB_Color_TypeDef color16= {218,60,90};

 
/*????????PWM????,???24???????LED,????24?0????*/
uint32_t Pixel_Buf[Pixel_NUM+1][24];       

/*
??:??????24?0,??24???????0?PWM?,????reset??,??????24*1.25=37.5us > 24us(????24us)
//???????????,????????Pixel_Buf[Pixel_NUM+1][24]???,???Reset_Load??,???????,
*/
void Reset_Load(void)
{
	uint8_t i;
	for(i=0;i<24;i++)
	{
		Pixel_Buf[Pixel_NUM][i] = 0;
	}
}
 
/*
??:????Pixel_Buf[Pixel_NUM+1][24]????,????????????1??1?CCR???,????PWM???
??:(&htim1)???1,(TIM_CHANNEL_1)??1,((uint32_t *)Pixel_Buf)?????,
			(Pixel_NUM+1)*24)????,??????
*/
void RGB_SendArray(void)
{
	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)Pixel_Buf,(Pixel_NUM+1)*24);
}

/*
??:????RGB LED???,?????RGB?24BIT???0??1?
??:LedId?LED??,Color:????????
*/
 //??WS2812B??????
void RGB_Flush(void)
{
    Reset_Load();     //??
    RGB_SendArray();  //????
    
	  
}

void RGB_SetOne_Color(uint8_t LedId,RGB_Color_TypeDef Color)
{
	uint8_t i; 
	if(LedId > Pixel_NUM)return; //avoid overflow ????ID??LED??
	//???? Pixel_Buf[LedId][i]???????????????(????PWM???????CCR1),
	for(i=0;i<8;i++) Pixel_Buf[LedId][i]   = ( ((Color.G/5) & (1 << (7 -i)))? (CODE_1):CODE_0 );//?????0~7????G
	for(i=8;i<16;i++) Pixel_Buf[LedId][i]  = ( ((Color.R/5) & (1 << (15-i)))? (CODE_1):CODE_0 );//?????8~15????R
	for(i=16;i<24;i++) Pixel_Buf[LedId][i] = ( ((Color.B/5) & (1 << (23-i)))? (CODE_1):CODE_0 );//?????16~23????B
}

//??RGB_SetOne_Color??,?????LED??????
void RGB_SetMore_Color(uint8_t head, uint8_t heal,RGB_Color_TypeDef color)
{
    uint8_t i=0;
    for(i=head;i<=heal;i++)
    {
        RGB_SetOne_Color(i,color) ;
    }
}
 

 
//???????????,??????????,???
//void RGB_RED(uint16_t Pixel_Len)
//{
//	uint16_t i;
//	for(i=0;i<Pixel_Len;i++)//?????LED????
//	{
//		RGB_SetOne_Color(i,RED);
//	}
//}
// 


//??????(?????????)
void RGB_Show_64(void)   
{
    RGB_SetMore_Color(0,63,BLACK);  //?????LED??
    RGB_SetMore_Color(0,rand()%8,table[rand()%16]);  //????????????
    RGB_SetMore_Color(8,rand()%8+8,table[rand()%16]);  //???????????
    RGB_SetMore_Color(16,rand()%8+16,table[rand()%16]);
    RGB_SetMore_Color(24,rand()%8+24,table[rand()%16]);
    RGB_SetMore_Color(32,rand()%8+32,table[rand()%16]);
    RGB_SetMore_Color(40,rand()%8+40,table[rand()%16]);
    RGB_SetMore_Color(48,rand()%8+48,table[rand()%16]);
    RGB_SetMore_Color(56,rand()%8+56,table[rand()%16]);
    RGB_Flush();      //??WS2812B???
}

