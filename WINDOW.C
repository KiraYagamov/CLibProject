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
    if (startx < 0 || startx > 24 || starty < 0 || starty > 79 ||
		endx < 0 || endx > 24 || endy < 0 || endy > 79){
		fprintf(stderr, "Error create_buffer 1");
		exit(1);
	}
    p = malloc(size);
    if( !p ) {
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
    if (startx < 0 || startx > 24 || starty < 0 || starty > 79 ||
		endx < 0 || endx > 24 || endy < 0 || endy > 79 || !w){
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
