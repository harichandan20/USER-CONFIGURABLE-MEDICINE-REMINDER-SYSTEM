#define _8BIT_LINE1 0x30
#define _8BIT_LINE2 0X38
#define DISP_ON 0X0C
#define DISP_CUR_ON 0X0E
#define DISP_CUR_BLINK_ON 0X0F
#define CLEAR_LCD 0X01
#define CUR_SHIFT_RIGHT 0X06

#define LCD 8
#define RS 16
#define RW 17
#define EN 18

void WriteLCD(unsigned char);
void CmdLCD(unsigned char);
void CharLCD(char);
void InitLCD(void);
void StrLCD(char*);

