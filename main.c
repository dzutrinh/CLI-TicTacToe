/* TIC-TAC-TOE GAME */
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "screen.h"

/* -- GAME LOGIC -- */
#define	S_WIN_X		"X WINS!"
#define	S_WIN_Y		"Y WINS!"
#define	S_DRAW		" DRAW! "
#define	S_ABORTED	"ABORTED"
#define	BOARD_X		10
#define	BOARD_Y		7

typedef enum _state {WIN_ROW, WIN_COL, WIN_DIAG1, WIN_DIAG2, PLAY, DRAW} STATE;

char board[9] = {'1','2','3','4','5','6','7','8','9'};
int	cell = 0;
int win_cell = -1;

void drawcell(const int idx, const int select) {
	int i, j, c;
	if (idx < 0 || idx > 8) return;
	i = idx % 3;
	j = idx / 3;
	switch (select) {
	case 0: c = 8; break;
	case 1: c = 15; break;
	case 2: c = 14; break;
	}
	frame(BOARD_X+i*6, BOARD_Y+j*4, 7, 5, c);
    switch(board[idx]) {
    case 'X': c = 3; break;
    case 'O': c = 11; break;
    default : c = 0;
	}	
	writecat(BOARD_X+3+i*6, BOARD_Y+2+j*4, c, board[idx]);
}

void drawboard(void) {
	int	i;
	writesat(0, 0, 15, " _______    ______        ______        ");
	writesat(0, 1, 15, "/_  __(_)__/_  __/__ ____/_  __/__  ___ ");
	writesat(0, 2,  7, " / / / / __// / / _ `/ __// / / _ \\/ -_)");
	writesat(0, 3,  8, "/_/ /_/\\__//_/  \\_,_/\\__//_/  \\___/\\__/ ");
                                       
	writesat(0, 5,  2, "            WASD: SELECT CELL             ");
	writesat(0, 6,  6, "        ENTER: SELECT  ESC: QUIT          ");
	for (i=0; i < 9; i++)
		drawcell(i, 0);
}

int move(void) {
	int done = 0;
	int cc = cell % 3, cr = cell / 3;
	char ch;
	do {
		cell = cr*3+cc;
		drawcell(cell, 1);
		ch = toupper(getch());
		drawcell(cell, 0);
		switch(ch) {
		case 'A': if (cc > 0) cc--; else { cc = 2; if (cr > 0) cr--; else cr = 2; } break; 
		case 'D': if (cc < 2) cc++; else { cc = 0; if (cr < 2) cr++; else cr = 0; } break;
		case 'W': if (cr > 0) cr--; else { cr = 2; if (cc > 0) cc--; else cc = 2; } break; 
		case 'S': if (cr < 2) cr++; else { cr = 0; if (cc < 2) cc++; else cc = 0; } break;
		case 32 :
		case 13 : if (!(board[cell] == 'X' || board[cell] == 'O')) done = 1; break;
		case 27 : cell = -1; done = 1; break;
		}
	} while(!done);
	return cell;
}

STATE gamestate() {
    if (board[0] == board[1] && board[1] == board[2])	{ win_cell = 0; return WIN_ROW; }
    if (board[3] == board[4] && board[4] == board[5])	{ win_cell = 3; return WIN_ROW; }
    if (board[6] == board[7] && board[7] == board[8])	{ win_cell = 6; return WIN_ROW; }
	if (board[0] == board[3] && board[3] == board[6])	{ win_cell = 0; return WIN_COL; }
    if (board[1] == board[4] && board[4] == board[7])	{ win_cell = 1; return WIN_COL; }
    if (board[2] == board[5] && board[5] == board[8])	{ win_cell = 2; return WIN_COL; }    
	if (board[0] == board[4] && board[4] == board[8])	{ return WIN_DIAG1; }
    if (board[2] == board[4] && board[4] == board[6])	{ return WIN_DIAG2; }		
	if (board[0] != '1' && board[1] != '2' && board[2] != '3' &&
    	board[3] != '4' && board[4] != '5' && board[5] != '6' &&
        board[6] != '7' && board[7] != '8' && board[8] != '9') return DRAW;
    return PLAY;
}

void drawresult(const STATE s) {
	int	i;
	switch (s) {
	case WIN_ROW: for (i=0; i < 3; i++) drawcell(i+win_cell  , 2); break;
	case WIN_COL: for (i=0; i < 3; i++) drawcell(i*3+win_cell, 2); break;
	case WIN_DIAG1: drawcell(0, 2); drawcell(4, 2); drawcell(8, 2); break;
	case WIN_DIAG2: drawcell(2, 2); drawcell(4, 2); drawcell(6, 2); break;
	}
}

int main() {
	int	c, p1 = 1;
    char mark;
	STATE s;

	clrscr();
	cursor(0);
    do {
    	if (p1) mark = 'X'; else mark = 'O';
    	writesat(16, 20, 4, "TURN:");
    	writecat(22, 20, 12, mark);
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
		if (s != DRAW) {
			drawresult(s);
			if (p1)	writesat(16, 21, 15, S_WIN_X);  
			else 	writesat(16, 21, 15, S_WIN_Y);
		}
		else writesat(16, 21, 12, S_DRAW);
	}
	else writesat(16, 21, 7, S_ABORTED);
	
	cursor(1);
	getch();
    return 0;
}
