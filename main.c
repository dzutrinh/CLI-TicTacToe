/* TIC-TAC-TOE GAME */
#include <windows.h>

/* -- MY CONSOLE I/O ROUTINES -- */
void cursor(int visible) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor = {1, visible};
    SetConsoleCursorInfo(handle, &cursor);
}

void textcolor(int x) {
    HANDLE color;
    color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(color, x);
}

void gotoxy(int x, int y) {
    HANDLE hConsoleOutput;
    COORD Cursor_an_Pos = {x, y};
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

void clrscr() {
	system("cls");	/* simplest way to clear the screen */
}

void writexy(int x, int y, int c, char * s) {
	gotoxy(x, y);
	textcolor(c);
	puts(s);
}

/* -- GAME LOGIC -- */

#define	S_WIN_X		"X WINS!"
#define	S_WIN_Y		"Y WINS!"
#define	S_DRAW		" DRAW! "
#define	S_ABORTED	"ABORTED"

typedef enum _state {WIN, PLAY, DRAW} STATE;

char board[9] = {'1','2','3','4','5','6','7','8','9'};
int	cell = 0;

void drawcell(int idx, int select) {
	int i, j, c;
	int row = 7, col = 10;
	i = idx % 3;
	j = idx / 3;
	if (select) c = 15; else c = 8;
    writexy(col+i*6, row+j*4+0, c, "+-----+");
    writexy(col+i*6, row+j*4+1, c, "|     |");
    writexy(col+i*6, row+j*4+2, c, "|     |");
    writexy(col+i*6, row+j*4+3, c, "|     |");
    writexy(col+i*6, row+j*4+4, c, "+-----+");
    if (board[idx]=='X') c = 3; else
    if (board[idx]=='O') c = 11; else c = 1;
    gotoxy(col+3+i*6, row+2+j*4);
	textcolor(c);
	putchar(board[idx]);
}

void drawboard(void) {
	int	i;
	writexy(0, 0, 15, " _______    ______        ______        ");
	writexy(0, 1, 15, "/_  __(_)__/_  __/__ ____/_  __/__  ___ ");
	writexy(0, 2,  7, " / / / / __// / / _ `/ __// / / _ \\/ -_)");
	writexy(0, 3,  8, "/_/ /_/\\__//_/  \\_,_/\\__//_/  \\___/\\__/ ");
                                       
	writexy(0, 5,  2, "            A: LEFT  D: RIGHT             ");
	writexy(0, 6,  6, "        ENTER: SELECT  ESC: QUIT          ");
	for (i=0; i < 9; i++)
		drawcell(i, 0);
}

int move(void) {
	int done = 0;
	char ch;
	do {
		drawcell(cell, 1);
		ch = getch();
		drawcell(cell, 0);
		switch(ch) {
		case 'A': 
		case 'a': if (cell > 0) cell--; else cell = 8; break;
		case 'D': 
		case 'd': if (cell < 8) cell++; else cell = 0; break;
		case 32 :
		case 13 : if (!(board[cell] == 'X' || board[cell] == 'O')) done = 1; break;
		case 27 : cell = -1; done = 1; break;
		}
	} while(!done);
	return cell;
}

STATE gamestate() {
    if ((board[0] == board[1] && board[1] == board[2]) || /* row */
        (board[3] == board[4] && board[4] == board[5]) ||
        (board[6] == board[7] && board[7] == board[8]) ||
        (board[0] == board[3] && board[3] == board[6]) || /* column */
        (board[1] == board[4] && board[4] == board[7]) ||
        (board[2] == board[5] && board[5] == board[8]) || /* diagonals */
        (board[0] == board[4] && board[4] == board[8]) ||
        (board[2] == board[4] && board[4] == board[6])) 
		return WIN;
    if (board[0] != '1' && board[1] != '2' && board[2] != '3' &&
    	board[3] != '4' && board[4] != '5' && board[5] != '6' &&
        board[6] != '7' && board[7] != '8' && board[8] != '9') 
		return DRAW;
    return PLAY;
}

int main()
{
	int	c, p1 = 1;
    char mark;
	STATE s;

	clrscr();
	cursor(0);
    do {
    	if (p1) mark = 'X'; else mark = 'O';
    	writexy(16, 20, 5, "TURN:");
    	if (p1) writexy(22, 20, 13, "X"); else writexy(22, 20, 13, "O");
		drawboard();
		c = move();
		if (c != -1) {
			board[c] = mark;
			s = gamestate();
			p1 = !p1;
		}
	}  while (s == PLAY && c != -1);
	if (c != -1) {
		p1 = !p1;
		drawboard();
		if (s == WIN)
			if (p1) writexy(16, 21, 15, S_WIN_X); else writexy(16, 21, 15, S_WIN_Y);
		else writexy(16, 21, 12, S_DRAW);
	}
	else writexy(16, 21, 7, S_ABORTED);
	cursor(1);
	getch();
    return 0;
}
