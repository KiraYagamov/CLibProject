#include <PROJECT/WINDOW.C>

/*main function*/
int main() {
	/*
	window_t *w1;
	window_t *w2;
	desktop();
	getch();

	w1 = create_window("Bomes", 5, 15, 5, 35, BLACK | BGLIGHTGRAY);
	open_window(w1);
	getch();

	w2 = create_window("Telegram", 7, 20, 25, 55, BLACK | BGLIGHTGRAY);
	open_window(w2);
	getch();

	switch_windows(w1, w2);
	getch();

	switch_windows(w2, w1);
	getch();

	close_window(w2);
	getch();
	close_window(w1);
	getch();

	delete_window(w1);
	delete_window(w2);
	*/

	window_t *w1;
	desktop();
	getch();

	w1 = create_window("Bomes", 5, 15, 5, 35, BLACK | BGLIGHTGRAY);
	open_window(w1);
	getch();
	window_xy(w1, 0, 0);
	getch();
	window_gets(w1, w1->buffer, w1->buffer_size);
	return 0;
}
