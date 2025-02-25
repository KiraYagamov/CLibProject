#include <PROJECT/BASE.H>
#include <stdlib.h>
#include <stdio.h>
#include <alloc.h>
#include <DOS.H>


vector2_t* cursor;

/*
str - указатель на массив char, длину которого необходимо узнать
*/
int get_length(char* str){
	int i = 0;
	for (i = 0; str[i]; i++){}
	return i;
}

/*
x --- 0 - 24 (номер строки)
y --- 0 - 79 (номер столбца)
При выходе за границы значений можно попасть в совершенно неожиданную ячейку на экране или вовсе выйти за пределы видеопамяти
ch - любые 8 бит (символ для вывода)
attrib - любые 8 бит (аттрибут для вывода (цвета текста, фона, мигание))
v - указатель на начало видеопамяти, сдвигается на нужную ячейку по координатам x и y
*/
void write_char(int x, int y, char ch, char attrib) {
	char far *v = (char far*) BASEMEM;
	if (x < 0 || x > 24 || y < 0 || y > 79){
		fprintf(stderr, "Error write_char");
		exit(1);
	}
	v += x*160 + y*2;
	*(v++) = ch;
	*v = attrib;
}


/*
x --- 0 - 24 (номер строки)
y --- 0 - 79 (номер столбца)
При выходе за границы значений можно попасть в совершенно неожиданную ячейку на экране или вовсе выйти за пределы видеопамяти
string - указатель на массив char (строка для вывода)
attrib - любые 8 бит (аттрибут для вывода (цвета текста, фона, мигание))
lastY - координата y последнего символа выводимой строки
*/
void write_string(int x, int y, char *string, char attrib) {
	int i = 0;
	int lastY = y + get_length(string);
	if (x < 0 || x > 24 || y < 0 || lastY > 79){
		fprintf(stderr, "Error write_string");
		exit(1);
	}
	for (i = 0; string[i]; i++){
		write_char(x, y + i , string[i], attrib);
	}
}

/*

/*
x --- 0 - 24 (номер строки)
y --- 0 - 79 (номер столбца)
При выходе за границы значений можно попасть в совершенно неожиданную ячейку на экране
или вовсе выйти за пределы видеопамяти
*/
char get_char(int x, int y){
	char far *v = (char far*) BASEMEM;
	if (x < 0 || x > 24 || y < 0 || y > 79){
		fprintf(stderr, "Error get_char");
		exit(1);
	}
	v += x*160 + y*2;
	return *v;
}

/*
x --- 0 - 24 (номер строки)
y --- 0 - 79 (номер столбца)
При выходе за границы значений можно попасть в совершенно неожиданную ячейку на экране
или вовсе выйти за пределы видеопамяти
v - указатель на начало видеопамяти, сдвигается на нужную ячейку по координатам x и y
*/
char get_color(int x, int y){
	char far *v = (char far*) BASEMEM;
	if (x < 0 || x > 24 || y < 0 || y > 79){
		fprintf(stderr, "Error get_color");
		exit(1);
	}
	v += x*160 + y*2;
	v++;
	return *v;
}

/*
startx --- 0 - 24 (начальный номер строки)
starty --- 0 - 79 (начальный номер столбца)
endx --- 0 - 24 (endx > startx) (конечный номер строки)
endy --- 0 - 79 (endy > starty) (конечный номер столбца)
buffer - указатель на массив для заполнения его данными о виртуальной памяти.
Его размер должен быть быть равен (endx - startx) * (endy - starty) * 2, так как он записывает и аттрибут
При несоблюдении возможен выход за границы видеопамяти.
Есть вероятность не просто не сохранить нужный буффер, так сохранится совершенно другая часть памяти
*/
void get_vmem(int startx, int starty, int endx, int endy, char*buffer) {
	int x = startx, y = starty;
	if (startx < 0 || startx > 24 || starty < 0 || starty > 79 ||
		endx < 0 || endx > 24 || endy < 0 || endy > 79){
		fprintf(stderr, "Error get_vmem");
		exit(1);
	}
	for (x = startx; x <= endx; x++){
		for(y = starty; y <= endy; y++){
			*buffer++ = get_char(x, y);
			*buffer++ = get_color(x, y);
		}
	}
}


/*
startx --- 0 - 24 (начальный номер строки)
starty --- 0 - 79 (начальный номер столбца)
endx --- 0 - 24 (endx > startx) (конечный номер строки)
endy --- 0 - 79 (endy > starty) (конечный номер столбца)
buffer - указатель на массив для заполнения его данными о виртуальной памяти.
Его размер должен быть быть равен (endx - startx) * (endy - starty) * 2, так как он записывает и аттрибут
При несоблюдении возможен выход за границы видеопамяти.
Есть вероятность не просто не сохранить нужный буффер, так сохранится совершенно другая часть памяти
*/
void get_vmem2(int startx, int starty, int endx, int endy, char*buffer) {
	int x = startx, y = starty, k = 0;
	if (startx < 0 || startx > 24 || starty < 0 || starty > 79 ||
		endx < 0 || endx > 24 || endy < 0 || endy > 79){
		fprintf(stderr, "Error get_vmem2");
		exit(1);
	}
	for (x = startx; x <= endx; x++){
		for(y = starty; y <= endy; y++){
			buffer[k++] = get_char(x, y);
			buffer[k++] = get_color(x, y);
		}
	}
}


/*
startx --- 0 - 24 (начальный номер строки)
starty --- 0 - 79 (начальный номер столбца)
endx --- 0 - 24 (endx > startx) (конечный номер строки)
endy --- 0 - 79 (endy > starty) (конечный номер столбца)
buffer - указатель на массив для заполнения его данными о виртуальной памяти.
Его размер должен быть быть равен (endx - startx) * (endy - starty) * 2, так как он воспроизводит и аттрибут
При несоблюдении возможен выход за границы видеопамяти.
Есть вероятность не просто не сохранить нужный буффер, так сохранится совершенно другая часть памяти
*/
void put_vmem(int startx, int starty, int endx, int endy, char*buffer) {
	int i = startx, j = starty, k = 0;
	if (startx < 0 || startx > 24 || starty < 0 || starty > 79 ||
		endx < 0 || endx > 24 || endy < 0 || endy > 79){
		fprintf(stderr, "Error put_vmem");
		exit(1);
	}
	for (i = startx; i <= endx; i++){
		for(j = starty; j <= endy; j++){
			write_char(i, j, buffer[k++], buffer[k+1]);
			k++;
		}
	}
}

/*
startx --- 0 - 24 (начальный номер строки)
starty --- 0 - 79 (начальный номер столбца)
endx --- 0 - 24 (endx > startx) (конечный номер строки)
endy --- 0 - 79 (endy > starty) (конечный номер столбца)
ch - любые 8 бит (символ для вывода)
attrib - любые 8 бит (аттрибут для вывода (цвета текста, фона, мигание))
При несоблюдении возможен выход за границы видеопамяти.
Есть вероятность не просто не сохранить нужный буффер, так сохранится совершенно другая часть памяти
*/
void clear(int startx, int starty, int endx, int endy, char ch, char attrib) {
	int i = 0, j = 0;
	if (startx < 0 || startx > 24 || starty < 0 || starty > 79 ||
		endx < 0 || endx > 24 || endy < 0 || endy > 79){
		fprintf(stderr, "Error Clear");
		exit(1);
	}
	for (i = startx; i <= endx; i++){
		for (j = starty; j <= endy; j++) {
			write_char(i, j, ch, attrib);
		}
	}
}

void desktop(){
	clear(0, 0, 24, 79, 0xB0, WHITE | BGBLUE);
}

/*
startx --- 0 - 24 (начальный номер строки)
starty --- 0 - 79 (начальный номер столбца)
endx --- 0 - 24 (endx > startx) (конечный номер строки)
endy --- 0 - 79 (endy > starty) (конечный номер столбца)
attrib - любые 8 бит (аттрибут для вывода (цвета текста, фона, мигание))
typ - порядковый номер вида границ. Варианты: 0, 1. Если выйти из диапазона, то примется значение по умолчанию (0)
При несоблюдении возможен выход за границы видеопамяти.
Есть вероятность не просто не сохранить нужный буффер, так сохранится совершенно другая часть памяти

horizontal_border, vertical_border
up_left_angle, up_right_angle           ------  символы границ
down_left_angle, down_right_angle
*/
void border(int startx, int starty, int endx, int endy, char attrib, int typ){
	int i = startx;
	char horizontal_border = 0xC4, vertical_border = 0xB3;
	char up_left_angle = 0xDA, up_right_angle = 0xBF;
	char down_left_angle = 0xC0, down_right_angle = 0xD9;
	if (startx < 0 || startx > 24 || starty < 0 || starty > 79 ||
		endx < 0 || endx > 24 || endy < 0 || endy > 79){
		fprintf(stderr, "Error Border");
		exit(1);
	}
	if (typ == 0){
		horizontal_border = 0xC4;
		vertical_border = 0xB3;
		up_left_angle = 0xDA;
		up_right_angle = 0xBF;
		down_left_angle = 0xC0;
		down_right_angle = 0xD9;
	}
	else if (typ == 1){
		horizontal_border = 0xCD;
		vertical_border = 0xBA;
		up_left_angle = 0xC9;
        up_right_angle = 0xBB;
		down_left_angle = 0xC8;
		down_right_angle = 0xBC;
	}
	for (i = startx; i <= endx; i++){
		write_char(i, starty, vertical_border, attrib);
		write_char(i, endy, vertical_border, attrib);
	}
	for (i = starty; i <= endy; i++){
		if (i == starty){
			write_char(startx, i, up_left_angle, attrib);
			write_char(endx, i, down_left_angle, attrib);
		}
		else if (i == endy) {
			write_char(startx, i, up_right_angle, attrib);
			write_char(endx, i, down_right_angle, attrib);
		}
		else{
			write_char(startx, i, horizontal_border, attrib);
			write_char(endx, i, horizontal_border, attrib);
		}
	}
}


/*
startx --- 0 - 24 (начальный номер строки)
starty --- 0 - 79 (начальный номер столбца)
endy --- 0 - 79 (endy > starty) (конечный номер столбца)
При выходе за границы значений можно попасть в совершенно неожиданную ячейку на экране или вовсе выйти за пределы видеопамяти
str - указатель на массив char (строка для вывода)
attrib - любые 8 бит (аттрибут для вывода (цвета текста, фона, мигание))
*/
void name(int startx, int starty, int endy, char*str, char attrib) {
	char *resultStr;
	int strLength = get_length(str);
	int y, i, isShort = 0;

	if (endy - starty-3 < strLength){
		strLength = endy - starty - 3;
		isShort = 1;
	}
	resultStr = (char*) malloc((strLength+1) * sizeof(char));
	if (!resultStr){
		fprintf(stderr, "Error name 1");
		exit(1);
	}
	for(i = 0; i < strLength; i++){
		if (isShort && strLength - i <= 3){
			resultStr[i] = '.';
		}
		else{
			resultStr[i] = str[i];
		}
	}
	resultStr[i] = '\0';
	
	y = starty + (endy-starty-strLength)/2 + 1;
	if (startx < 0 || startx > 24 || starty < 0 || starty > 79 || endy < 0 || endy > 79){
		fprintf(stderr, "Error name 2");
		exit(1);
	}
	write_string(startx, y, resultStr, attrib);
}

int get_key() {
	/* Коды не совпадают с маковскими!
	int code;
	_AH = 0x00;
	geninterrupt(0x16);
	code = _AX;
	return code; */

	/* Коды не совпадают с маковскими! */
	union REGS in,out;
	int code;
	in.h.ah = 0x00;
	int86(0x16,&in,&out);
	code = out.x.ax;
	return code;


	/* Коды совпадают
	return getch();
	*/
}

int window_getkey() {
	union KEY {
		char ascii, scancode;
		int code;
	};
	union KEY k;
	k.code = get_key();
	if(k.ascii == 0) {
		return k.scancode + 0x100;
	}
	else {
		return k.ascii;
	}
}
void goto_xy(int x, int y) {
	_AH = 0x02;
	_DH = x;
	_DL = y;
	_BH = 0x00;
	geninterrupt(0x10);
	if (!cursor) cursor = (vector2_t*) malloc(sizeof(vector2_t));
	cursor->x = x;
	cursor->y = y;
}

void window_xy(window_t* w, int x, int y){
	if (w->startx + x + 1 <= w->startx || w->startx + x + 1 > w->endx - 1 ||
		w->starty + y + 1 <= w->starty || w->starty + y + 1 > w->endy - 1 ||
		w->isActive == 0) {
			return;
	}
	goto_xy(w->startx + x + 1, w->starty + y + 1);
}

vector2_t* get_cursor() {
	vector2_t* vector = (vector2_t*) malloc(sizeof(vector2_t));
	vector->x = _DL;
	vector->y = _DH;
	return vector;
}

void move_cursor_next(window_t* w){
	cursor->y += 1;
	if (cursor->y > w->endy - 1){
		cursor->x += 1;
		cursor->y = w->starty + 1;
		if (cursor->x > w->endx - 1){
			cursor->x = w->startx + 1;
			cursor->y = w->starty + 1;
		}
	}
	goto_xy(cursor->x, cursor->y);
}

void move_cursor_before(window_t* w){
	cursor->y -= 1;
	if (cursor->y <= w->starty){
		cursor->x -= 1;
		if (cursor->x <= w->startx){
			cursor->y += 1;
			cursor->x += 1;
			goto_xy(cursor->x, cursor->y);
			return;
		}
		cursor->y = w->endy - 1;
	}
	goto_xy(cursor->x, cursor->y);
}

void window_putchar(window_t* w, char ch) {
	if (cursor->x <= w->startx || cursor->x > w->endx - 1 ||
		cursor->y <= w->starty || cursor->y > w->endy - 1 || 
		w->isActive == 0){
			printf("Error putchar\nData: x: %d, y: %d", cursor->x, cursor->y);
			return;
	}
	write_char(cursor->x, cursor->y, ch, w->attrib);
	move_cursor_next(w);
}

void window_putstr(window_t* w, char* s) {
	int len = get_length(s);
	int i = 0;
	for (i = 0; i < len; i++){
		window_putchar(w, s[i]);
	}
}

void window_gets(window_t* w, char* s, int len){
	int key = window_getkey();
	int i = 0;
	while (key != CR && key != F10 && i <= len){
		if (key == BKSP){
			move_cursor_before(w);
			window_putchar(w, ' ');
			move_cursor_before(w);
			i--;
			s[i] = key;
			key = window_getkey();
			continue;
		}
		else{
			window_putchar(w, key);
		}
		s[i] = key;
		key = window_getkey();
		i++;
	}
}