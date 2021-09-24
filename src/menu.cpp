#include <Adafruit_GFX.h>

//摇杆相关变量
#define pinX  35
#define pinY  34

int valueX = 0;
int valueY = 0;
unsigned char keyValue = 0;

unsigned char menu = 0;


//多级菜单相关变量
//定义按键结构体
typedef struct
{
  unsigned char index;
  unsigned char up;
  unsigned char down;
  unsigned char left;
  unsigned char right;
  void (*operation)(void);
} KEY_TABLE;

unsigned char funIndex = 0;
void (*current)(void);
void menu00(void);
void menu11(void);
void menu12(void);
void menu21(void);
void menu22(void);
void menu23(void);
void menu24(void);
void menu25(void);
void menu26(void);


//定义按键操作数据
KEY_TABLE table[9] =
{
  {0, 0, 0, 0, 1, (*menu00)},
  {1, 1, 2, 0, 3, (*menu11)},
  {2, 1, 2, 0, 6, (*menu12)},
  {3, 3, 4, 1, 3, (*menu21)},
  {4, 3, 5, 1, 4, (*menu22)},
  {5, 4, 5, 1, 5, (*menu23)},
  {6, 6, 7, 2, 6, (*menu24)},
  {7, 6, 8, 2, 7, (*menu25)},
  {8, 7, 8, 2, 8, (*menu26)},
};

void menu00(void)
{

}

void menu11(void)
{

}

void menu12(void)
{

}

void menu21(void)
{

}

void menu22(void)
{

}

void menu23(void)
{

}

void menu24(void)
{

}

void menu25(void)
{

}

void menu26(void)
{

}

//按键扫描函数
unsigned char keyScan(void)
{
    return 1;
}


void initRTCTime()//初始化RTC时钟
{
}

void show_time()
{
}

void updatTime()//更新时间数据
{

}
