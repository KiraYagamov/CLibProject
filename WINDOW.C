#include <PROJECT/WINDOW.H>
#include <PROJECT/BASE.C>


/*
startx --- 0 - 24 (начальный номер строки)
starty --- 0 - 79 (начальный номер столбца)
endx --- 0 - 24 (endx > startx) (конечный номер строки)
endy --- 0 - 79 (endy > starty) (конечный номер столбца)
При несоблюдении возможен выход за границы видеопамяти.
Есть вероятность не просто не сохранить нужный буффер, так сохранится совершенно другая часть памяти
*/
char* create_buffer(int startx, int starty, int endx, int endy){
	char *p;
    int size = (endx-startx+1)*(endy-starty+1)*2*sizeof(char);
	CheckCoords(startx, starty);
	CheckCoords(endx, endy);
    p = malloc(size);
    if(!p) {
        fprintf(stderr, "Error create_buffer 2");
        exit(1);
    }
	return p;
}


/*
startx --- 0 - 24 (начальный номер строки)
starty --- 0 - 79 (начальный номер столбца)
endx --- 0 - 24 (endx > startx) (конечный номер строки)
endy --- 0 - 79 (endy > starty) (конечный номер столбца)
При несоблюдении возможен выход за границы видеопамяти.
Есть вероятность не просто не создать окно в нужном месте, так можно испортить другие участки памяти и сломать систему в целом
*/
window_t * create_window(char* name, int startx, int endx, int starty, int endy, int attrib){
	window_t * w = malloc(sizeof(window_t));
	CheckCoords(startx, starty);
	CheckCoords(endx, endy);
    if (!w){
		fprintf(stderr, "Error create_window");
		exit(1);
	}
	w->startx = startx;
	w->starty = starty;
	w->endx = endx;
	w->endy = endy;
	w->isActive = 0;
	w->buffer_size = (endx - startx - 1) * (endy - starty - 1) - 1;
	w->buffer = (char*) malloc(sizeof(char) * w->buffer_size);

	w->attrib = attrib;
    strcpy(w->name, name);
    w->p = create_buffer(startx, starty, endx, endy);

	return w;
}


/*
w - указатель на окно в памяти
*/
void open_window(window_t *w){
    get_vmem(w->startx, w->starty, w->endx, w->endy, w->p);
    clear(w->startx, w->starty, w->endx, w->endy, ' ', w->attrib);
    border(w->startx, w->starty, w->endx, w->endy, w->attrib, 1);
	name(w->startx, w->starty, w->endy, w->name, w->attrib);
	w->isActive = 1;
}

/*
w - указатель на окно в памяти
*/
void close_window(window_t *w){
    put_vmem(w->startx, w->starty, w->endx, w->endy, w->p);
	w->isActive = 0;
}

/*
w - указатель на окно в памяти
*/
void delete_window(window_t *w){
    free(w);
}

/*
w1 - указатель на нижнее окно (оно будет верхним) в памяти
w2 - указатель на верхнее окно (оно будет нижним) в памяти
*/
void switch_windows(window_t *w1, window_t *w2){
    close_window(w2);
	close_window(w1);
	open_window(w2);
	open_window(w1);
}

void window_xy(window_t* w, int x, int y){
	CheckCoordsWithBorders(w->startx + x + 1, w->starty + y + 1, w->startx-1, w->endx-1, w->starty-1, w->endy-1);
	if (w->isActive == 0) {
		return;
	}
	goto_xy(w->startx + x + 1, w->starty + y + 1);
}

void move_cursor_next(window_t* w){
	cursor_y += 1;
	if (cursor_y > w->endy - 1){
		cursor_x += 1;
		cursor_y = w->starty + 1;
		if (cursor_x > w->endx - 1){
			cursor_x = w->startx + 1;
			cursor_y = w->starty + 1;
		}
	}
	goto_xy(cursor_x, cursor_y);
}

void move_cursor_before(window_t* w){
	cursor_y -= 1;
	if (cursor_y <= w->starty){
		cursor_x -= 1;
		if (cursor_x <= w->startx){
			cursor_y += 1;
			cursor_x += 1;
			goto_xy(cursor_x, cursor_y);
			return;
		}
		cursor_y = w->endy - 1;
	}
	goto_xy(cursor_x, cursor_y);
}

void window_putchar(window_t* w, char ch) {
	CheckCoordsWithBorders(cursor_x, cursor_y, w->startx-1, w->endx-1, w->starty-1, w->endy-1);
	write_char(cursor_x, cursor_y, ch, w->attrib);
	move_cursor_next(w);
}

void window_putstr(window_t* w, char* s) {
	int len = get_length(s);
	int i = 0;
	for (i = 0; i < len; i++){
		window_putchar(w, s[i]);
	}
}

char* window_gets(window_t* w, char* s, int len){
	int key = window_getkey();
	int i = get_length(s);
	while (key != CR && key != F10 && i < len){
		if (key == BKSP){
			move_cursor_before(w);
			window_putchar(w, ' ');
			move_cursor_before(w);
			i--;
			s[i] = ' ';
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
	s[i] = 0;
	close_window(w);
	return s;
}

void write_data(window_t* w, char* s) {
	int length = get_length(s);
	int i = 0;
	window_xy(w, 0, 0);
	for (i = 0; i < length; i++) {
		window_putchar(w, s[i]);
	}
}