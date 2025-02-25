#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

#define BGBLACK 0
#define BGBLUE 16
#define BGGREEN 32
#define BGCYAN 48
#define BGRED 64
#define BGMAGENTA 80
#define BGBROWN 96
#define BGLIGHTGRAY 112

#define BLINK 128

#define BASEMEM 0xb8000000

/*
#define RIGHT 0x4D
#define LEFT 0x4B
#define UP 0x48
#define DOWN 0x50
#define PGUP 0x49
#define PGDN 0x51
#define HOME 0x47
#define END 0x4F
#define INSERT 0x52
#define DEL 0x53
#define F1 0x3B
#define F2 0x3C
#define F3 0x3D
#define F4 0x3E
#define F5 0x3F
#define F6 0x40
#define F7 0x41
#define F8 0x42
#define F9 0x43

#define F11 0x45
#define F12 0x46
*/

#define F10 256
#define LEFT 0x014B
#define BKSP 0x08
#define CR 0x0D
#define ESC 0x1B

typedef struct vector2
{
    int x, y;
} vector2_t;
