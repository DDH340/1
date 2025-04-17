#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "LED.h"
#include "lcd.h"
#include "key.h"

// 颜色定义 (RGB565)
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_YELLOW  0xFFE0
#define COLOR_WHITE   0xFFFF
#define COLOR_BLACK   0x0000

// 图形类型枚举
typedef enum {
    SHAPE_TRIANGLE = 1,
    SHAPE_CIRCLE,
    SHAPE_STAR,
    SHAPE_SQUARE
} ShapeType;

// 图形结构体
typedef struct {
    ShapeType type;
    u16 x;
    u16 y;
    u16 color;
} Shape;

// 全局变量
Shape shapes[4];       // 存储图形数据
u8 shape_count = 0;    // 图形计数器
u16 scroll_y = 240;    // 学号垂直位置
u16 scroll_x = 0;      // 姓名水平位置
u8 lcd_id[12];         // LCD ID存储
// 绘制三角形
void DrawTriangle(u16 x, u16 y, u16 color) {
    POINT_COLOR = color;
    LCD_DrawLine(x+25, y, x, y+50);
    LCD_DrawLine(x+25, y, x+50, y+50);
    LCD_DrawLine(x, y+50, x+50, y+50);
    LCD_ShowString(x+5, y+20, 100, 16, 16, (u8*)"Triangle");
}

// 绘制圆形
void DrawCircle(u16 x, u16 y, u16 color) {
    POINT_COLOR = color;
    LCD_Draw_Circle(x+25, y+25, 25);
    LCD_ShowString(x+10, y+15, 80, 16, 16, (u8*)"Circle");
}

// 绘制五角星
void DrawStar(u16 x, u16 y, u16 color) {
    POINT_COLOR = color;
    LCD_DrawLine(x+25,y, x+35,y+40);
    LCD_DrawLine(x+35,y+40, x,y+15);
    LCD_DrawLine(x,y+15, x+50,y+15);
    LCD_DrawLine(x+50,y+15, x+15,y+40);
    LCD_DrawLine(x+15,y+40, x+25,y);
    LCD_ShowString(x+5, y+45, 80, 16, 16, (u8*)"Star");
}

// 绘制正方形
void DrawSquare(u16 x, u16 y, u16 color) {
    POINT_COLOR = color;
    LCD_DrawRectangle(x, y, x+50, y+50);
    LCD_ShowString(x+5, y+15, 100, 16, 16, (u8*)"Square");
}

// 滚动显示
void ScrollDisplay() {
    // 清除旧位置内容
    LCD_Fill(100, scroll_y, 220, scroll_y+16, COLOR_WHITE);
    LCD_Fill(scroll_x, 200, scroll_x+200, 216, COLOR_WHITE);
    
    // 显示新内容
    LCD_ShowString(100, scroll_y, 200, 16, 16, (u8*)"2022014340");
    LCD_ShowString(scroll_x, 200, 200, 16, 16, (u8*)"Du Dianhong");
    
    // 更新位置
    scroll_y = (scroll_y > 0) ? scroll_y - 1 : 240;
    scroll_x = (scroll_x < 320) ? scroll_x + 2 : 0;
}

int main(void)
{ 
    u8 key;
    
    // 系统初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);
    uart_init(115200);
    LED_Init();
    KEY_Init();
    LCD_Init();
    
    // 获取LCD ID
    sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);
    LCD_Clear(COLOR_WHITE);
    LCD_ShowString(0, 0, 210, 24, 24, (u8*)"Explorer STM32F4");

    while(1)
    {
        key = KEY_Scan(0);
        
        // 处理按键输入
        if(key && shape_count < 4) {
            switch(key) {
                case KEY0_PRES:  // 红色三角形
                    shapes[shape_count++] = (Shape){SHAPE_TRIANGLE, 20, 50, COLOR_RED};
                    break;
                case KEY1_PRES:  // 绿色圆形
                    shapes[shape_count++] = (Shape){SHAPE_CIRCLE, 100, 50, COLOR_GREEN};
                    break;
                case KEY2_PRES:  // 蓝色五角星
                    shapes[shape_count++] = (Shape){SHAPE_STAR, 20, 150, COLOR_BLUE};
                    break;
                case WKUP_PRES:  // 黄色正方形
                    shapes[shape_count++] = (Shape){SHAPE_SQUARE, 100, 150, COLOR_YELLOW};
                    break;
            }
        }

        // 清屏并重绘所有图形
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

        // 滚动显示
        ScrollDisplay();
        
        // 显示状态信息
        LCD_ShowString(30, 290, 200, 16, 16, lcd_id);
        delay_ms(20);
				LED0=!LED0;
    }
}

