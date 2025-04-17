#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "LED.h"
#include "lcd.h"
#include "key.h"

// ��ɫ���� (RGB565)
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_YELLOW  0xFFE0
#define COLOR_WHITE   0xFFFF
#define COLOR_BLACK   0x0000

// ͼ������ö��
typedef enum {
    SHAPE_TRIANGLE = 1,
    SHAPE_CIRCLE,
    SHAPE_STAR,
    SHAPE_SQUARE
} ShapeType;

// ͼ�νṹ��
typedef struct {
    ShapeType type;
    u16 x;
    u16 y;
    u16 color;
} Shape;

// ȫ�ֱ���
Shape shapes[4];       // �洢ͼ������
u8 shape_count = 0;    // ͼ�μ�����
u16 scroll_y = 240;    // ѧ�Ŵ�ֱλ��
u16 scroll_x = 0;      // ����ˮƽλ��
u8 lcd_id[12];         // LCD ID�洢
// ����������
void DrawTriangle(u16 x, u16 y, u16 color) {
    POINT_COLOR = color;
    LCD_DrawLine(x+25, y, x, y+50);
    LCD_DrawLine(x+25, y, x+50, y+50);
    LCD_DrawLine(x, y+50, x+50, y+50);
    LCD_ShowString(x+5, y+20, 100, 16, 16, (u8*)"Triangle");
}

// ����Բ��
void DrawCircle(u16 x, u16 y, u16 color) {
    POINT_COLOR = color;
    LCD_Draw_Circle(x+25, y+25, 25);
    LCD_ShowString(x+10, y+15, 80, 16, 16, (u8*)"Circle");
}

// ���������
void DrawStar(u16 x, u16 y, u16 color) {
    POINT_COLOR = color;
    LCD_DrawLine(x+25,y, x+35,y+40);
    LCD_DrawLine(x+35,y+40, x,y+15);
    LCD_DrawLine(x,y+15, x+50,y+15);
    LCD_DrawLine(x+50,y+15, x+15,y+40);
    LCD_DrawLine(x+15,y+40, x+25,y);
    LCD_ShowString(x+5, y+45, 80, 16, 16, (u8*)"Star");
}

// ����������
void DrawSquare(u16 x, u16 y, u16 color) {
    POINT_COLOR = color;
    LCD_DrawRectangle(x, y, x+50, y+50);
    LCD_ShowString(x+5, y+15, 100, 16, 16, (u8*)"Square");
}

// ������ʾ
void ScrollDisplay() {
    // �����λ������
    LCD_Fill(100, scroll_y, 220, scroll_y+16, COLOR_WHITE);
    LCD_Fill(scroll_x, 200, scroll_x+200, 216, COLOR_WHITE);
    
    // ��ʾ������
    LCD_ShowString(100, scroll_y, 200, 16, 16, (u8*)"2022014340");
    LCD_ShowString(scroll_x, 200, 200, 16, 16, (u8*)"Du Dianhong");
    
    // ����λ��
    scroll_y = (scroll_y > 0) ? scroll_y - 1 : 240;
    scroll_x = (scroll_x < 320) ? scroll_x + 2 : 0;
}

int main(void)
{ 
    u8 key;
    
    // ϵͳ��ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);
    uart_init(115200);
    LED_Init();
    KEY_Init();
    LCD_Init();
    
    // ��ȡLCD ID
    sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);
    LCD_Clear(COLOR_WHITE);
    LCD_ShowString(0, 0, 210, 24, 24, (u8*)"Explorer STM32F4");

    while(1)
    {
        key = KEY_Scan(0);
        
        // ����������
        if(key && shape_count < 4) {
            switch(key) {
                case KEY0_PRES:  // ��ɫ������
                    shapes[shape_count++] = (Shape){SHAPE_TRIANGLE, 20, 50, COLOR_RED};
                    break;
                case KEY1_PRES:  // ��ɫԲ��
                    shapes[shape_count++] = (Shape){SHAPE_CIRCLE, 100, 50, COLOR_GREEN};
                    break;
                case KEY2_PRES:  // ��ɫ�����
                    shapes[shape_count++] = (Shape){SHAPE_STAR, 20, 150, COLOR_BLUE};
                    break;
                case WKUP_PRES:  // ��ɫ������
                    shapes[shape_count++] = (Shape){SHAPE_SQUARE, 100, 150, COLOR_YELLOW};
                    break;
            }
        }

        // �������ػ�����ͼ��
        LCD_Clear(COLOR_WHITE);
        for(u8 i=0; i<shape_count; i++) {
            switch(shapes[i].type) {
                case SHAPE_TRIANGLE:
                    DrawTriangle(shapes[i].x, shapes[i].y, shapes[i].color);
                    break;
                case SHAPE_CIRCLE:
                    DrawCircle(shapes[i].x, shapes[i].y, shapes[i].color);
                    break;
                case SHAPE_STAR:
                    DrawStar(shapes[i].x, shapes[i].y, shapes[i].color);
                    break;
                case SHAPE_SQUARE:
                    DrawSquare(shapes[i].x, shapes[i].y, shapes[i].color);
                    break;
            }
        }

        // ������ʾ
        ScrollDisplay();
        
        // ��ʾ״̬��Ϣ
        LCD_ShowString(30, 290, 200, 16, 16, lcd_id);
        delay_ms(20);
				LED0=!LED0;
    }
}

