typedef struct window
{
    int startx, starty, endx, endy;
    int attrib;
    char name[80];
    char *p;
    int isActive;
    char* buffer;
    int buffer_size;
} window_t;
