#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <fstream>
#pragma comment(lib,"Winmm.lib")
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 77
#define KEY_RIGHT 75

using namespace std;

ifstream ifs;
ofstream ofs;

const int MAX = 20;

struct user_info
{
	char username[MAX];
	char password[MAX];
	unsigned int win_of_E;
	unsigned int win_of_P;
	unsigned int win_of_P1;
	unsigned int win_of_P2;
};

struct check_square
{
	char icon;
	int horizontal;
	int vertical;
	int mainDiagonal;
	int oppositeDiagonal;
};

struct tictactoe_board
{
	check_square pos[MAX][MAX];
	int X_steps;
	int O_steps;
	int countX;
	int countY;
};

struct design_icon
{
	char icon;
	int color;
};

struct position
{
	int X;
	int Y;
	int X_on_screen;
	int Y_on_screen;
};

user_info user;
tictactoe_board game;
design_icon x, o;
position saved;
char rep2;
int n, m, condition_win, clueX, clueY, clue_i = 2, clue_j = 1, Count = 0, limit_step, count_X, count_O; // 17, 17
bool go_first = 1, mode, check_move = 0, have_account, signed_in = 0, turn_on_sound = 0;

// Interface support function

void welcome(); // Print the name of the game

void disable_selection(); // Disable mouse click and selection on the screen

void set_color(int background_color, int text_color); // set background color and text color

void color_icon(); // Set icon color

void print_board(); // Print the board on the screen

void design_cursor(int i, int j); // Design the cursor at the current position on the board

// Audio support function

void Sound(); // Setting sound

void Sound_Game_1(); // Sound of game

// Game support function

void continue_game(); // Ask to continue the unfinished game

void check_Y_N(char& re, bool& check); // Check Yes/No

void check_account(char& rep, bool& existence); // Sign up/in account

void show_list(); // Show the statistical outcomes of all accounts

void init(); // Setting all of the things needed before starting the game

void update_old_board(); // Setting again the unfinished game

void GoTo(SHORT, SHORT); // Set the location of the cursor on the screen

void vertical_to_up(int x, int y, int current_y); // Check the continuous icons at a position on the board to up

void vertical_to_down(int x, int y); // Check the continuous icons at a position on the board to down

void horizontal_to_left(int x, int y, int current_x); // Check the continuous icons at a position on the board to the left

void horizontal_to_right(int x, int y); // Check the continuous icons at a position on the board to the right

void mainDiagonal_above(int x, int y, int current_x, int current_y); // The main diagonal is divided into 2 parts from the point. The function is used to check continuous points in the above parts

void mainDiagonal_below(int x, int y); // The main diagonal is divided into 2 parts from the point. The function is used to check continuous points in the below parts

void oppositeDiagonal_above(int x, int y, int current_x, int current_y); // The opposite diagonal is divided into 2 parts from the point. The function is used to check continuous points in the above parts

void oppositeDiagonal_below(int x, int y); // The opposite diagonal is divided into 2 parts from the point. The function is used to check continuous points in the below parts

void check_consecutive(int x, int y); // Using to update the current position and around that position

void processing_game_PvP(); // Processing game in PvP mode

void processing_P_go_first(); // Processing game in PvE mode, P go first

void processing_E_go_first(); // Processing game in PvE mode, E go first

bool check_win(int x, int y); // Checking win

bool check_turn(int move); // Checking Player turns

void move_cursor_PvP(int&, int&, int&, int&, int&); // Move cursor in PvP mode

void move_cursor_PvE(int&, int&, int&, int&, int&); // Move cursor in PvE mode

void choose_move_processing_mainDiagonal(int adjacent, int i, int j, int i1, int j1,  // Choose the next move for E in the main diagonal
	int i2, int j2, int& posX, int& posY, bool& check_move, int check_step);

void choose_move_processing_oppositeDiagonal(int adjacent, int i, int j, int i1, int j1, // Choose the next move for E in the opposite diagonal
	int i2, int j2, int& posX, int& posY, bool& check_move, int check_step);

void choose_move_processing_horizontal(int adjacent, int i, int j, int i1, int j1, // Choose the next move for E in the horizontal
	int i2, int j2, int& posX, int& posY, bool& check_move, int check_step);

void choose_move_processing_vertical(int adjacent, int i, int j, int i1, int j1, // Choose the next move for E in the vertical
	int i2, int j2, int& posX, int& posY, bool& check_move, int check_step);

void choose_move(int adjacent, int& posX, int& posY, bool& check_move); // Processing of choosing the next move for E

bool replay(char rep); // Check if the player want to replay the game

int main()
{
	char rep;
	bool play = 1, existence = 0;

	Sound();

	system("cls");

	welcome();

	system("cls");

	continue_game();

	system("cls");

	if (rep2 == 'n' || rep2 == 'N') check_account(rep, existence);

	system("cls");

	while (play) {

		Count = 0;

		set_color(7, 0);

		system("cls");

		init();

		system("cls");

		GoTo(0, 0);

		set_color(7, 0);

		print_board();

		update_old_board();

		if (mode == 0) processing_game_PvP();
		else {
			if (go_first) processing_P_go_first();
			else processing_E_go_first();
		}

		cout << "Enter to continue...\n";

		_getch();

		system("cls");

		cout << "Do you want to play again? Y(yes) or N(no)\n";
		cout << "Type your choice: ";
		ofs.open("saved_position.txt", ofstream::out);
		ofs << "";
		ofs.close();
		cin >> rep;
		play = replay(rep);
	}

	cout << "\nThanks for playing my game!!!\n";
	Sound_Game_1();
	return 0;
}

void Sound()
{
	cout << "*SETTING SOUND*\n";
	cout << "[Y]: Turn on sound\n";
	cout << "[N]: Turn off sound\n";
	bool tmp = 0;
	char key = _getch();
	while (!tmp) {
		if (key >= 'A' && key <= 'Z' || key >= 'a' && key <= 'z') {
			switch (key) {
			case 'Y':
			case 'y':
				turn_on_sound = 1;
			case 'N':
			case 'n':
				turn_on_sound = 0;
			}
		}
		if (key == 'y' || key == 'Y') {
			turn_on_sound = 1;
			tmp = 1;
		}
		else if (key == 'n' || key == 'N') {
			tmp = 1;
		}
	}
}

void welcome()
{
	GoTo(7, 0);
	set_color(2, 4);
	cout << "WELCOME TO TICTACTOE GAME!!!\n\n";
	Sound_Game_1();
}

void continue_game()
{
	char Continue = NULL;
	rep2 = 'N';

	ifs.open("saved_position.txt");
	ifs >> Continue;
	ifs.close();
	set_color(0, 15);
	if (Continue != NULL) {
		cout << "You have 1 unfinished game. Do you want to sign in the latest account and continue the game? Y(yes) or N(no)\n";
		cout << "Your choice: ";
		cin >> rep2;
		while (rep2 != 'N' && rep2 != 'n'
			&& rep2 != 'Y' && rep2 != 'y') {
			cout << "*SYNTAX ERROR*\n";
			cout << "Your choice: ";
			cin >> rep2;
		}
	}
}

void check_Y_N(char& re, bool& check)
{
	if (re == 'Y' || re == 'y') check = 1;
	else if (re == 'N' || re == 'n') check = 0;
	while (re != 'Y' && re != 'y' && re != 'N' && re != 'n') {
		cout << "*SYNTAX ERROR*\n";
		cout << "Your choice: ";
		cin >> re;
		if (re == 'Y' || re == 'y') check = 1;
		else if (re == 'N' || re == 'n') check = 0;
	}
}

void show_list()
{
	system("cls");
	char tmp[MAX];
	int tmp_;
	ifs.open("accounts.txt");
	while (!ifs.eof()) {
		ifs >> tmp;
		if (tmp[0] == '1' || tmp[0] == '0') {
			ifs >> tmp;
			ifs >> tmp_;
			ifs >> tmp_;
			ifs >> tmp_;
			ifs >> tmp_;
		}
		else {
			cout << "USERNAME: ";
			cout << tmp << endl;
			ifs >> tmp;
			cout << "PvE mode: ";
			ifs >> tmp_;
			cout << "LOSE: " << tmp_ << "     ";
			ifs >> tmp_;
			cout << "WIN: " << tmp_ << endl;
			cout << "PvP mode: ";
			ifs >> tmp_;
			cout << "P1 WIN: ";
			cout << tmp_ << "     ";
			ifs >> tmp_;
			cout << "P2 WIN: " << tmp_ << endl << endl;
		}
	}
	ifs.close();
	cout << "[Enter] to continue...";
}

void check_account(char& rep, bool& existence)
{
	char rep_tmp;
	ofs.open("accounts.txt", ofstream::app);
	ofs.close();
	set_color(0, 11);
	cout << "*You must sign to continue the game!*\n";
	cout << "Did you have an account? Y(yes) or N(no)\n";
	cout << "Your choice: ";
	cin >> rep;
	check_Y_N(rep, have_account);

	system("cls");

	while (have_account) {
		char username_existence[MAX], password_existence[MAX];
		system("cls");

		cout << "*SIGN IN USER*\n";
		cout << "Type your USERNAME: ";
		cin >> user.username;
		cout << "Type your PASSWORD: ";
		cin >> user.password;
		ifs.open("accounts.txt");
		while (!ifs.eof()) {
			int tmp;
			ifs >> username_existence;
			ifs >> password_existence;
			ifs >> tmp;
			ifs >> tmp;
			ifs >> tmp;
			ifs >> tmp;
			if (strcmp(user.username, username_existence) == 0
				&& strcmp(user.password, password_existence) == 0) {
				existence = 1;
				break;
			}
		}
		ifs.close();
		if (!existence) {
			system("cls");
			char rep_tmp;
			cout << "This account does not exist or maybe the password is wrong!\n";
			cout << "Do you want to sign up a new account? Y(yes) or N(no)\n";
			cout << "Your choice: ";
			cin >> rep_tmp;
			if (rep_tmp == 'Y' || rep_tmp == 'y') have_account = 0;
			else if (rep_tmp == 'N' || rep_tmp == 'n') have_account = 1;
			while (rep_tmp != 'Y' && rep_tmp != 'y' && rep_tmp != 'N' && rep_tmp != 'n') {
				cout << "*SYNTAX ERROR*\n";
				cout << "Your choice: ";
				cin >> rep_tmp;
				if (rep_tmp == 'Y' || rep_tmp == 'y') have_account = 0;
				else if (rep_tmp == 'N' || rep_tmp == 'n') have_account = 1;
			}
			if (have_account) {
				cout << "*Please type your USERNAME and PASSWORD again*\n";
				cout << "Enter to continue ...";
				_getch();
			}
		}
		else break;
	}
	ofs.open("latest_account.txt", ofstream::out);
	ofs << user.username;
	ofs.close();
	if (!have_account) {
		char un_tmp[MAX], pw_tmp[MAX] = "";
		int P1_w_tmp, P2_w_tmp;
		bool check_tmp = 1;
		ifs.open("accounts.txt");
		cout << "*SIGN UP USER*\n";
		cout << "Type your USERNAME: ";
		cin >> user.username;
		while (!ifs.eof()) {
			int tmp;
			ifs >> un_tmp;
			if (strcmp(un_tmp, user.username) == 0) {
				check_tmp = 0;
			}
			ifs >> pw_tmp;
			ifs >> tmp;
			ifs >> tmp;
			ifs >> P1_w_tmp;
			ifs >> P2_w_tmp;
		}
		ifs.close();
		ofs.open("accounts.txt", ofstream::app);
		while (check_tmp == 0) {
			check_tmp = 1;
			system("cls");
			cout << "*USERNAME has existed*\n";
			cout << "*SIGN UP USER*\n";
			cout << "Type your USERNAME: ";
			ifs.open("accounts.txt");
			cin >> user.username;
			while (!ifs.eof()) {
				int tmp;
				ifs >> un_tmp;
				if (strcmp(un_tmp, user.username) == 0) {
					check_tmp = 0;
				}
				ifs >> pw_tmp;
				ifs >> tmp;
				ifs >> tmp;
				ifs >> P1_w_tmp;
				ifs >> P2_w_tmp;
			}
			ifs.close();
		}
		ofs << user.username << " ";
		cout << "Type your PASWORD: ";
		cin >> user.password;
		ofs << user.password << " ";
		ofs << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
		ofs.close();
	}

	ifs.open("accounts.txt");
	while (!ifs.eof()) {
		char tmp[MAX];
		ifs >> tmp;
		ifs >> user.password;
		ifs >> user.win_of_E;
		ifs >> user.win_of_P;
		ifs >> user.win_of_P1;
		ifs >> user.win_of_P2;
		if (strcmp(tmp, user.username) == 0) break;
	}
	ifs.close();

	ofs.open("latest_account.txt", ofstream::out);
	ofs << user.username;
	ofs.close();

	system("cls");

	cout << "You have sign in game already! Enjoy the game!!!\n";
	cout << "Type [L] to see the Statistical Outcomes or [Enter] to continue...\n";
	char key = _getch();
		if (key >= 'A' && key <= 'Z' || key >= 'a' && key <= 'z') {
			switch (key) {
				case 'l':
				case 'L':
					show_list();
					break;
			}
		}
	_getch();
}

void disable_selection()
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(hStdin, ~ENABLE_QUICK_EDIT_MODE);
}

void set_color(int background_color, int text_color)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	int color_code = background_color * 16 + text_color;
	SetConsoleTextAttribute(hStdout, color_code);
}

void color_icon()
{
	int count_tmp = 1;

	GoTo(5, 0);
	cout << "*[Choose color for each icon]*\n";
	GoTo(0, 1);
	set_color(7, 1); cout << "1: Blue\n";
	set_color(7, 2); cout << "2: Green\n";
	set_color(7, 3); cout << "3: Aqua\n";
	set_color(7, 4); cout << "4: Red\n";
	set_color(7, 5); cout << "5: Purple\n";
	set_color(7, 6); cout << "6: Yellow\n";
	set_color(7, 8); cout << "7: Gray\n";
	set_color(7, 9); cout << "8: Light Blue\n";
	set_color(7, 10); cout << "9: Light Green\n";
	set_color(7, 11); cout << "10: Light Aqua\n";
	set_color(7, 12); cout << "11: Light Red\n";
	set_color(7, 13); cout << "12: Light Purple\n";
	set_color(7, 14); cout << "13: Light Yellow\n";
	set_color(7, 0); cout << "14: Black\n";
	for (int i = 1; i <= 14; i++) {
		GoTo(17, i);
		cout << "|";
	}
	GoTo(18, 1);
	cout << "Color of " << x.icon << " (type a number): ";
	cin >> x.color;
	while (x.color < 1 || x.color > 14) {
		count_tmp++;
		GoTo(18, count_tmp);
		cout << "*OUT OF RANGE*\n";
		count_tmp++;
		GoTo(18, count_tmp);
		cout << "Color of " << x.icon << " (type a number): ";
		cin >> x.color;
	}
	if (x.color >= 7 && x.color <= 13) x.color++;
	if (x.color == 14) x.color = 0;

	count_tmp++;
	GoTo(18, count_tmp);
	cout << "Color of " << o.icon << " (type a number): ";
	cin >> o.color;
	if (o.color >= 7 && o.color <= 13) o.color++;
	if (o.color == 14) o.color = 0;
	while (o.color < 1 || o.color > 14 || o.color == x.color) {
		count_tmp++;
		GoTo(18, count_tmp);
		if (o.color == x.color) cout << "*COLOR MUST BE DIFFERENT FROM THE OTHER*\n";
		else cout << "*OUT OF RANGE*\n";
		count_tmp++;
		GoTo(18, count_tmp);
		cout << "Color of " << o.icon << " (type a number): ";
		cin >> o.color;
	}
}

void init()
{
	int re;

	ifs.open("count_step_X.txt");
	ifs >> game.countX;
	ifs.close();
	ifs.open("count_step_O.txt");
	ifs >> game.countY;
	ifs.close();

	if (rep2 == 'N' || rep2 == 'n') {
		ofs.open("saved_position.txt", ofstream::out);
		ofs << "";
		ofs.close();
		ofs.open("setting.txt", ofstream::out);
		cout << "Board size (n x m)\n";
		cout << "      n (3 ... 17): ";
		cin >> n;
		while (n < 3 || n > 17) {
			cout << "*OUT OF RANGE*\n";
			cout << "Please type again\n";
			cout << "      n (3 ... 17): ";
			cin >> n;
		}

		cout << "      m (3 ... 17): ";
		cin >> m;
		while (m < 3 || m > 17) {
			cout << "*OUT OF RANGE*\n";
			cout << "Please type again\n";
			cout << "      m (3 ... 17): ";
			cin >> m;
		}

		ofs << n << endl;
		ofs << m << endl;

		condition_win = 3;

		if (n == 3) cout << "Number of continuous icons to win: 3\n";
		else if (n == 4) {
			cout << "How many continuous icons to win? (3 or 4). Your choice: ";
			cin >> condition_win;
			while (condition_win < 3 || condition_win > 4) {
				cout << "*OUT OF RANGE*\n";
				cout << "Please type again\n";
				cout << "Your choice: ";
				cin >> condition_win;
			}
		}
		else if (n >= 5) {
			cout << "How many continuous icons to win? (3, 4, 5). Your choice: ";
			cin >> condition_win;
			while (condition_win < 3 || condition_win > 5) {
				cout << "*OUT OF RANGE*\n";
				cout << "Please type again\n";
				cout << "Your choice: ";
				cin >> condition_win;
			}
		}
		ofs << condition_win << endl;

		cout << "Limit step of each Player (10 .. 289): ";
		cin >> limit_step;
		ofs << limit_step << endl;
		game.O_steps = limit_step;
		game.X_steps = limit_step;
		ofs << game.X_steps << endl;
		ofs << game.O_steps << endl;

		cout << "Type player 1's icon (1 character): ";
		cin >> x.icon;
		cout << "Type player 2's icon (1 character): ";
		cin >> o.icon;
		while (o.icon == x.icon) {
			cout << "*Icon must be different from the other*\n";
			cout << "Please type again\n";
			cout << "Type player 2's icon (1 character): ";
			cin >> o.icon;
		}

		system("cls");

		color_icon();

		ofs << x.icon << endl;
		ofs << x.color << endl;
		ofs << o.icon << endl;
		ofs << o.color << endl;

		system("cls");

		GoTo(5, 0);
		cout << "*MODE*\n";
		set_color(7, 3);
		cout << "1: PvP\n";
		cout << "2: PvE\n";
		set_color(7, 0);
		cout << "Your choice: ";
		cin >> re;
		while (re != 2 && re != 1) {
			cout << "*SYNTAX ERROR*\n";
			cout << "Your choice: ";
			cin >> re;
		}

		if (re == 1) { mode = 0; }
		else { mode = 1; }
		ofs << mode << endl;
		char rep;
		if (mode == 1) {
			cout << "Do you want to go first? Y(yes) or N(no)\n";
			cout << "Your choice: ";
			cin >> rep;
			if (rep == 'Y' || rep == 'y') go_first = 1;
			else if (rep == 'N' || rep == 'n') go_first = 0;
			else {
				while (true) {
					cout << "*SYNTAX ERROR*\n";
					cout << "Your choice: ";
					cin >> rep;
					if (rep == 'Y' || rep == 'y') {
						go_first = 1;
						break;
					}
					else if (rep == 'N' || rep == 'n') {
						go_first = 0;
						break;
					}
				}
			}
			ofs << go_first << endl;
		}
		ofs.close();
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				if (i >= n || j >= m) { game.pos[i][j].icon = ' '; }
				else { game.pos[i][j].icon = ' '; }
				game.pos[i][j].horizontal = 0;
				game.pos[i][j].vertical = 0;
				game.pos[i][j].mainDiagonal = 0;
				game.pos[i][j].oppositeDiagonal = 0;
			}
		}
	}
	else if (rep2 == 'Y' || rep2 == 'y') {

		ifs.open("setting.txt");
		ifs >> n;
		ifs >> m;
		ifs >> condition_win;
		ifs >> limit_step;
		ifs >> game.X_steps;
		game.X_steps -= game.countX;
		ifs >> game.O_steps;
		game.O_steps -= game.countY;
		ifs >> x.icon;
		ifs >> x.color;
		ifs >> o.icon;
		ifs >> o.color;
		ifs >> mode;
		ifs >> go_first;
		if (!go_first && mode == 1) {
			game.O_steps -= 1;
		}
		ifs.close();

		ifs.open("latest_account.txt");
		ifs >> user.username;
		ifs.close();
		signed_in = 1;

		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				if (i >= n || j >= m) { game.pos[i][j].icon = ' '; }
				else { game.pos[i][j].icon = ' '; }
				game.pos[i][j].horizontal = 0;
				game.pos[i][j].vertical = 0;
				game.pos[i][j].mainDiagonal = 0;
				game.pos[i][j].oppositeDiagonal = 0;
			}
		}
	}

	game.countX = 0;
	game.countY = 0;
}

void print_board()
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << " ---";
		}
		cout << endl;
		for (int j = 0; j < m; j++) {
			cout << "| " << game.pos[i][j].icon << " ";
		}
		cout << "|";
		cout << endl;
	}
	for (int j = 0; j < m; j++) {
		cout << " ---";
	}
	cout << endl;
	GoTo(0, 2 * n + 1);
	cout << "The rest of ";
	set_color(7, x.color);
	cout << x.icon;
	set_color(7, 0);
	cout << ":\n";
	GoTo(0, 2 * n + 2);
	cout << game.X_steps << endl;
	GoTo(0, 2 * n + 3);
	cout << "The rest of ";
	set_color(7, o.color);
	cout << o.icon;
	set_color(7, 0);
	cout << ":\n";
	GoTo(0, 2 * n + 4);
	cout << game.O_steps << endl;
}

void update_old_board()
{
	if (rep2 == 'y' || rep2 == 'Y') {
		int tmp1 = 0, tmp2 = 0, tmp3 = 0, tmp4 = 0;
		ifs.open("saved_position.txt");
		while (!ifs.eof()) {
			ifs >> saved.X;
			ifs >> saved.Y;
			ifs >> saved.X_on_screen;
			ifs >> saved.Y_on_screen;

			if (tmp1 == saved.X && tmp2 == saved.Y
				&& tmp3 == saved.X_on_screen && tmp4 == saved.Y_on_screen) break;
			tmp1 = saved.X;
			tmp2 = saved.Y;
			tmp3 = saved.X_on_screen;
			tmp4 = saved.Y_on_screen;

			if (Count % 2 == 0) {
				GoTo(saved.X_on_screen, saved.Y_on_screen);
				set_color(7, x.color);
				game.pos[saved.X][saved.Y].icon = x.icon;
				cout << game.pos[saved.X][saved.Y].icon;
				Count++;
			}
			else {
				GoTo(saved.X_on_screen, saved.Y_on_screen);
				set_color(7, o.color);
				game.pos[saved.X][saved.Y].icon = o.icon;
				cout << game.pos[saved.X][saved.Y].icon;
				Count++;
			}
			check_consecutive(saved.X, saved.Y);
		}
		ifs.close();
	}
}

void GoTo(SHORT posX, SHORT posY)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD Position;
	Position.X = posX;
	Position.Y = posY;
	SetConsoleCursorPosition(hStdout, Position);
}

void vertical_to_up(int x, int y, int current_y)
{
	if (game.pos[x][y].icon == game.pos[x][current_y].icon) {
		vertical_to_up(x, y - 1, current_y);
		game.pos[x][y].vertical++;
	}
	if (y == current_y) {
		if (game.pos[x][y - 1].vertical != 0
			&& game.pos[x][y - 1].icon == game.pos[x][y].icon) {
			game.pos[x][y].vertical = game.pos[x][y - 1].vertical;
		}
		else {
			game.pos[x][y].vertical = 1;
		}
	}
}

void vertical_to_down(int x, int y)
{
	if (game.pos[x][y + 1].icon == game.pos[x][y].icon) {
		vertical_to_up(x, y + 1, y + 1);
		vertical_to_down(x, y + 1);
	}
}

void horizontal_to_left(int x, int y, int current_x)
{
	if (game.pos[x][y].icon == game.pos[current_x][y].icon) {
		horizontal_to_left(x - 1, y, current_x);
		game.pos[x][y].horizontal++;
	}
	if (x == current_x) {
		if (game.pos[x - 1][y].horizontal != 0
			&& game.pos[x - 1][y].icon == game.pos[x][y].icon) {
			game.pos[x][y].horizontal = game.pos[x - 1][y].horizontal;
		}
		else {
			game.pos[x][y].horizontal = 1;
		}
	}
}

void horizontal_to_right(int x, int y)
{
	if (game.pos[x + 1][y].icon == game.pos[x][y].icon) {
		horizontal_to_left(x + 1, y, x + 1);
		horizontal_to_right(x + 1, y);
	}
}

void mainDiagonal_above(int x, int y, int current_x, int current_y)
{
	if (game.pos[x][y].icon == game.pos[current_x][current_y].icon) {
		mainDiagonal_above(x - 1, y - 1, current_x, current_y);
		game.pos[x][y].mainDiagonal++;
	}
	if (x == current_x) {
		if (game.pos[x - 1][y - 1].mainDiagonal != 0
			&& game.pos[x - 1][y - 1].icon == game.pos[x][y].icon) {
			game.pos[x][y].mainDiagonal = game.pos[x - 1][y - 1].mainDiagonal;
		}
		else {
			game.pos[x][y].mainDiagonal = 1;
		}
	}
}

void mainDiagonal_below(int x, int y)
{
	if (game.pos[x + 1][y + 1].icon == game.pos[x][y].icon) {
		mainDiagonal_above(x + 1, y + 1, x + 1, y + 1);
		mainDiagonal_below(x + 1, y + 1);
	}
}

void oppositeDiagonal_above(int x, int y, int current_x, int current_y)
{
	if (game.pos[x][y].icon == game.pos[current_x][current_y].icon) {
		oppositeDiagonal_above(x + 1, y - 1, current_x, current_y);
		game.pos[x][y].oppositeDiagonal++;
	}
	if (x == current_x) {
		if (game.pos[x + 1][y - 1].oppositeDiagonal != 0
			&& game.pos[x + 1][y - 1].icon == game.pos[x][y].icon) {
			game.pos[x][y].oppositeDiagonal = game.pos[x + 1][y - 1].oppositeDiagonal;
		}
		else {
			game.pos[x][y].oppositeDiagonal = 1;
		}
	}
}

void oppositeDiagonal_below(int x, int y)
{
	if (game.pos[x - 1][y + 1].icon == game.pos[x][y].icon) {
		oppositeDiagonal_above(x - 1, y + 1, x - 1, y + 1);
		oppositeDiagonal_below(x - 1, y + 1);
	}
}

void check_consecutive(int x, int y)
{

	if (game.pos[x][y].vertical == 0) {
		game.pos[x][y].vertical = 1;
		game.pos[x][y].horizontal = 1;
		game.pos[x][y].mainDiagonal = 1;
		game.pos[x][y].oppositeDiagonal = 1;
	}

	// check the continuous vertical line
	vertical_to_up(x, y, y);
	vertical_to_down(x, y);

	// check the continuous horizontal line
	horizontal_to_left(x, y, x);
	horizontal_to_right(x, y);

	// check the continuous main diagonal
	mainDiagonal_above(x, y, x, y);
	mainDiagonal_below(x, y);

	// check the continuous opposite diagonal
	oppositeDiagonal_above(x, y, x, y);
	oppositeDiagonal_below(x, y);
}

bool check_win(int x, int y)
{
	if (game.pos[x][y].horizontal >= condition_win
		|| game.pos[x][y].vertical >= condition_win
		|| game.pos[x][y].mainDiagonal >= condition_win
		|| game.pos[x][y].oppositeDiagonal >= condition_win) {

		return true;
	}
	return false;
}

bool check_turn(int move)
{
	if (move % 2 == 0) {
		return true;
	}
	return false;
}

void design_cursor(int i, int j)
{
	set_color(7, 13);
	GoTo(i - 1, j);
	cout << "[";
	GoTo(i + 1, j);
	cout << "]";
	GoTo(i, j);
}

void move_cursor_PvP(int& i, int& j, int& count_player_moves, int& pos_x, int& pos_y)
{
	bool keepMoving = true;
	while (keepMoving) {
		char key = _getch();
		if (check_turn(count_player_moves)) {
			if (key >= 'A' && key <= 'Z' || key >= 'a' && key <= 'z') {
				switch (key) {
				case 'W':
				case 'w':
					//KEY_UP:
					if (j == 1) break;
					GoTo(i - 1, j);
					cout << ' ';
					GoTo(i + 1, j);
					cout << ' ';
					j -= 2;
					pos_y--;
					design_cursor(i, j);
					break;
				case 'S':
				case 's':
					//KEY_DOWN:
					if (j == 2 * (n - 1) + 1) break;
					GoTo(i - 1, j);
					cout << ' ';
					GoTo(i + 1, j);
					cout << ' ';
					j += 2;
					pos_y++;
					design_cursor(i, j);
					break;
				case 'D':
				case 'd':
					//KEY_RIGHT:
					if (i == 4 * (m - 1) + 2) break;
					GoTo(i - 1, j);
					cout << ' ';
					GoTo(i + 1, j);
					cout << ' ';
					i += 4;
					pos_x++;
					design_cursor(i, j);
					break;
				case 'A':
				case 'a':
					//KEY_LEFT:
					if (i == 2) break;
					GoTo(i - 1, j);
					cout << ' ';
					GoTo(i + 1, j);
					cout << ' ';
					i -= 4;
					pos_x--;
					design_cursor(i, j);
					break;
				}
			}
			else if ((key == ' ' || key == '\n' || key == '\r')
				&& game.pos[pos_x][pos_y].icon == ' ') {
				set_color(7, x.color);
				game.pos[pos_x][pos_y].icon = x.icon;
				cout << game.pos[pos_x][pos_y].icon;
				GoTo(i, j);
				count_player_moves++;
				keepMoving = false;
			}
		}
		else {
			if ((key == '\n' || key == '\r' || key == ' ')
				&& game.pos[pos_x][pos_y].icon == ' ') {
				set_color(7, o.color);
				game.pos[pos_x][pos_y].icon = o.icon;
				cout << game.pos[pos_x][pos_y].icon;
				GoTo(i, j);
				count_player_moves++;
				keepMoving = false;
			}
			else {
				switch (key) {
				case KEY_UP:
					if (j == 1) break;
					GoTo(i - 1, j);
					cout << ' ';
					GoTo(i + 1, j);
					cout << ' ';
					j -= 2;
					pos_y--;
					design_cursor(i, j);
					break;
				case KEY_DOWN:
					if (j == 2 * (n - 1) + 1) break;
					GoTo(i - 1, j);
					cout << ' ';
					GoTo(i + 1, j);
					cout << ' ';
					j += 2;
					pos_y++;
					design_cursor(i, j);
					break;
				case KEY_LEFT:
					if (i == 4 * (m - 1) + 2) break;
					GoTo(i - 1, j);
					cout << ' ';
					GoTo(i + 1, j);
					cout << ' ';
					i += 4;
					pos_x++;
					design_cursor(i, j);
					break;
				case KEY_RIGHT:
					if (i == 2) break;
					GoTo(i - 1, j);
					cout << ' ';
					GoTo(i + 1, j);
					cout << ' ';
					i -= 4;
					pos_x--;
					design_cursor(i, j);
					break;
				}
			}
		}
	}
}

void choose_move_processing_mainDiagonal(int adjacent, int i, int j, int i1, int j1,
	int i2, int j2, int& posX, int& posY, bool& check_move, int check_step)
{
	if (check_step == 2) {
		if (game.pos[i1][j1].icon == game.pos[i2][j2].icon) {
			if (game.pos[i1][j1].mainDiagonal + game.pos[i2][j2].mainDiagonal >= adjacent - 1) {
				posX = i;
				posY = j;
				check_move = 1;
			}
		}
	}
	if (check_step == 1) {
		if (game.pos[i1][j1].icon != game.pos[i2][j2].icon) {
			if ((game.pos[i1][j1].mainDiagonal == adjacent - 1
				&& game.pos[i1 + adjacent - 1][j1 + adjacent - 1].icon == ' ')
				|| (game.pos[i2][j2].mainDiagonal == adjacent - 1
					&& game.pos[i2 - (adjacent - 1)][j2 - (adjacent - 1)].icon == ' ')) {
				posX = i;
				posY = j;
				check_move = 1;
			}
		}
	}
	if (check_step == 3) {
		if (game.pos[i1][j1].icon != game.pos[i2][j2].icon) {
			if ((game.pos[i1][j1].mainDiagonal == adjacent - 1
				&& game.pos[i1 + adjacent - 1][j1 + adjacent - 1].icon != ' ')
				|| (game.pos[i2][j2].mainDiagonal == adjacent - 1
					&& game.pos[i2 - (adjacent - 1)][j2 - (adjacent - 1)].icon != ' ')) {
				posX = i;
				posY = j;
				check_move = 1;
			}
		}
	}
}

void choose_move_processing_oppositeDiagonal(int adjacent, int i, int j, int i1, int j1,
	int i2, int j2, int& posX, int& posY, bool& check_move, int check_step)
{
	if (check_step == 2) {
		if (game.pos[i1][j1].icon == game.pos[i2][j2].icon) {
			if (game.pos[i1][j1].oppositeDiagonal + game.pos[i2][j2].oppositeDiagonal >= adjacent - 1) {
				posX = i;
				posY = j;
				check_move = 1;
			}
		}
	}
	if (check_step == 1) {
		if (game.pos[i1][j1].icon != game.pos[i2][j2].icon) {
			if ((game.pos[i1][j1].oppositeDiagonal == adjacent - 1
				&& game.pos[i1 + adjacent - 1][j1 - (adjacent - 1)].icon == ' ')
				|| (game.pos[i2][j2].oppositeDiagonal == adjacent - 1
					&& game.pos[i2 - (adjacent - 1)][j2 + (adjacent - 1)].icon == ' ')) {
				posX = i;
				posY = j;
				check_move = 1;
			}
		}
	}
	if (check_step == 3) {
		if (game.pos[i1][j1].icon != game.pos[i2][j2].icon) {
			if ((game.pos[i1][j1].oppositeDiagonal == adjacent - 1
				&& game.pos[i1 + adjacent - 1][j1 - (adjacent - 1)].icon != ' ')
				|| (game.pos[i2][j2].oppositeDiagonal == adjacent - 1
					&& game.pos[i2 - (adjacent - 1)][j2 + (adjacent - 1)].icon != ' ')) {
				posX = i;
				posY = j;
				check_move = 1;
			}
		}
	}
}

void choose_move_processing_horizontal(int adjacent, int i, int j, int i1, int j1,
	int i2, int j2, int& posX, int& posY, bool& check_move, int check_step)
{
	if (check_step == 2) {
		if (game.pos[i1][j1].icon == game.pos[i2][j2].icon) {
			if (game.pos[i1][j1].horizontal + game.pos[i2][j2].horizontal >= adjacent - 1) {
				posX = i;
				posY = j;
				check_move = 1;
			}
		}
	}
	if (check_step == 1) {
		if (game.pos[i1][j1].icon != game.pos[i2][j2].icon) {
			if ((game.pos[i1][j1].horizontal == adjacent - 1
				&& game.pos[i1 - (adjacent - 1)][j1].icon == ' ')
				|| (game.pos[i2][j2].horizontal == adjacent - 1
					&& game.pos[i2 + (adjacent - 1)][j2].icon == ' ')) {
				posX = i;
				posY = j;
				check_move = 1;
			}
		}
	}
	if (check_step == 3) {
		if (game.pos[i1][j1].icon != game.pos[i2][j2].icon) {
			if ((game.pos[i1][j1].horizontal == adjacent - 1
				&& game.pos[i1 - (adjacent - 1)][j1].icon != ' ')
				|| (game.pos[i2][j2].horizontal == adjacent - 1
					&& game.pos[i2 + (adjacent - 1)][j2].icon != ' ')) {
				posX = i;
				posY = j;
				check_move = 1;
			}
		}
	}
}

void choose_move_processing_vertical(int adjacent, int i, int j, int i1, int j1,
	int i2, int j2, int& posX, int& posY, bool& check_move, int check_step)
{
	if (check_step == 2) {
		if (game.pos[i1][j1].icon == game.pos[i2][j2].icon) {
			if (game.pos[i1][j1].vertical + game.pos[i2][j2].vertical >= adjacent - 1) {
				posX = i;
				posY = j;
				check_move = 1;
			}
		}
	}
	if (check_step == 1) {
		if (game.pos[i1][j1].icon != game.pos[i2][j2].icon) {
			if ((game.pos[i1][j1].vertical == adjacent - 1
				&& game.pos[i1][j1 - (adjacent - 1)].icon == ' ')
				|| (game.pos[i2][j2].vertical == adjacent - 1
					&& game.pos[i2][j2 + (adjacent - 1)].icon == ' ')) {
				posX = i;
				posY = j;
				check_move = 1;
			}
		}
	}
	if (check_step == 3) {
		if (game.pos[i1][j1].icon != game.pos[i2][j2].icon) {
			if ((game.pos[i1][j1].vertical == adjacent - 1
				&& game.pos[i1][j1 - (adjacent - 1)].icon != ' ')
				|| (game.pos[i2][j2].vertical == adjacent - 1
					&& game.pos[i2][j2 + (adjacent - 1)].icon != ' ')) {
				posX = i;
				posY = j;
				check_move = 1;
			}
		}
	}
}

void choose_move(int adjacent, int& posX, int& posY, bool& check_move)
{
	int check_step = 1;
	while (check_step <= 3) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (game.pos[i][j].icon == ' ') {
					// check the main Diagonal
					choose_move_processing_mainDiagonal(adjacent, i, j, i + 1, j + 1, i - 1, j - 1, posX, posY, check_move, check_step);
					if (check_move) break;
					// check the opposite Diagonal
					choose_move_processing_oppositeDiagonal(adjacent, i, j, i + 1, j - 1, i - 1, j + 1, posX, posY, check_move, check_step);
					if (check_move) break;
					// check the horizontal line
					choose_move_processing_horizontal(adjacent, i, j, i - 1, j, i + 1, j, posX, posY, check_move, check_step);
					if (check_move) break;
					// check the vertical line
					choose_move_processing_vertical(adjacent, i, j, i, j - 1, i, j + 1, posX, posY, check_move, check_step);
					if (check_move) break;
				}
			}
			if (check_move) break;
		}
		if (check_move) break;
		check_step++;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (game.pos[i][j].icon == ' ') {
					// check the main Diagonal
					choose_move_processing_mainDiagonal(adjacent, i, j, i + 1, j + 1, i - 1, j - 1, posX, posY, check_move, check_step);
					if (check_move) break;
					// check the opposite Diagonal
					choose_move_processing_oppositeDiagonal(adjacent, i, j, i + 1, j - 1, i - 1, j + 1, posX, posY, check_move, check_step);
					if (check_move) break;
					// check the horizontal line
					choose_move_processing_horizontal(adjacent, i, j, i - 1, j, i + 1, j, posX, posY, check_move, check_step);
					if (check_move) break;
					// check the vertical line
					choose_move_processing_vertical(adjacent, i, j, i, j - 1, i, j + 1, posX, posY, check_move, check_step);
					if (check_move) break;
				}
			}
			if (check_move) break;
		}
		if (check_move) break;
		check_step++;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (game.pos[i][j].icon == ' ') {
					// check the main Diagonal
					choose_move_processing_mainDiagonal(adjacent, i, j, i + 1, j + 1, i - 1, j - 1, posX, posY, check_move, check_step);
					if (check_move) break;
					// check the opposite Diagonal
					choose_move_processing_oppositeDiagonal(adjacent, i, j, i + 1, j - 1, i - 1, j + 1, posX, posY, check_move, check_step);
					if (check_move) break;
					// check the horizontal line
					choose_move_processing_horizontal(adjacent, i, j, i - 1, j, i + 1, j, posX, posY, check_move, check_step);
					if (check_move) break;
					// check the vertical line
					choose_move_processing_vertical(adjacent, i, j, i, j - 1, i, j + 1, posX, posY, check_move, check_step);
					if (check_move) break;
				}
			}
			if (check_move) break;
		}
		if (check_move) break;
	}
}

void move_cursor_PvE(int& i, int& j, int& count_player_moves, int& pos_x, int& pos_y)
{
	bool keepMoving = true;
	check_move = 0;

	if (!check_turn(count_player_moves)) {
		for (int k = 0; k < condition_win; k++) {
			choose_move(condition_win - k, pos_x, pos_y, check_move);
			if (check_move) break;
		}
		GoTo(i - 1, j);
		cout << ' ';
		GoTo(i + 1, j);
		cout << ' ';
		i = pos_x * 4 + 2;
		j = pos_y * 2 + 1;
		GoTo(i, j);
		set_color(7, o.color);
		game.pos[pos_x][pos_y].icon = o.icon;
		cout << game.pos[pos_x][pos_y].icon;
		GoTo(i - 1, j);
		set_color(7, 13);
		set_color(7, 13);
		cout << "[";
		GoTo(i + 1, j);
		cout << "]";
		GoTo(i, j);
		count_player_moves++;
		keepMoving = false;
	}
	else {
		GoTo(clue_i - 1, clue_j);
		cout << ' ';
		GoTo(clue_i + 1, clue_j);
		cout << ' ';
		for (int k = 0; k < condition_win; k++) {
			choose_move(condition_win - k, clueX, clueY, check_move);
			if (check_move) break;
		}
		clue_i = clueX * 4 + 2;
		clue_j = clueY * 2 + 1;
		GoTo(clue_i - 1, clue_j);
		set_color(7, 0);
		cout << '>';
		GoTo(clue_i + 1, clue_j);
		set_color(7, 0);
		cout << '<';
		GoTo(i, j);
		while (keepMoving) {
			char key = _getch();
			if ((key == ' ' || key == '\n' || key == '\r')
				&& game.pos[pos_x][pos_y].icon == ' ') {
				set_color(7, x.color);
				game.pos[pos_x][pos_y].icon = x.icon;
				cout << game.pos[pos_x][pos_y].icon;
				GoTo(i, j);
				count_player_moves++;
				keepMoving = false;
			}
			else {
				switch (key) {
				case 'W':
				case 'w':
					//KEY_UP:
					if (j == 1) break;
					GoTo(i - 1, j);
					cout << ' ';
					GoTo(i + 1, j);
					cout << ' ';
					j -= 2;
					pos_y--;
					design_cursor(i, j);
					break;
				case 'S':
				case 's':
					//KEY_DOWN:
					if (j == 2 * (n - 1) + 1) break;
					GoTo(i - 1, j);
					cout << ' ';
					GoTo(i + 1, j);
					cout << ' ';
					j += 2;
					pos_y++;
					design_cursor(i, j);
					break;
				case 'D':
				case 'd':
					//KEY_RIGHT:
					if (i == 4 * (m - 1) + 2) break;
					GoTo(i - 1, j);
					cout << ' ';
					GoTo(i + 1, j);
					cout << ' ';
					i += 4;
					pos_x++;
					design_cursor(i, j);
					break;
				case 'A':
				case 'a':
					//KEY_LEFT:
					if (i == 2) break;
					GoTo(i - 1, j);
					cout << ' ';
					GoTo(i + 1, j);
					cout << ' ';
					i -= 4;
					pos_x--;
					design_cursor(i, j);
					break;
				case KEY_UP:
					if (j == 1) break;
					GoTo(i - 1, j);
					cout << ' ';
					GoTo(i + 1, j);
					cout << ' ';
					j -= 2;
					pos_y--;
					design_cursor(i, j);
					break;
				case KEY_DOWN:
					if (j == 2 * (n - 1) + 1) break;
					GoTo(i - 1, j);
					cout << ' ';
					GoTo(i + 1, j);
					cout << ' ';
					j += 2;
					pos_y++;
					design_cursor(i, j);
					break;
				case KEY_LEFT:
					if (i == 4 * (m - 1) + 2) break;
					GoTo(i - 1, j);
					cout << ' ';
					GoTo(i + 1, j);
					cout << ' ';
					i += 4;
					pos_x++;
					design_cursor(i, j);
					break;
				case KEY_RIGHT:
					if (i == 2) break;
					GoTo(i - 1, j);
					cout << ' ';
					GoTo(i + 1, j);
					cout << ' ';
					i -= 4;
					pos_x--;
					design_cursor(i, j);
					break;
				}
			}
		}
	}
}

bool replay(char rep)
{
	bool right = 0;
	if (rep == 'Y' || rep == 'y') return true;
	else if (rep == 'N' || rep == 'n') return false;
	else {
		while (!right) {
			cout << "*SYNTAX ERROR*\n";
			cout << "Type your choice: ";
			cin >> rep;
			if (rep == 'Y' || rep == 'y') return true;
			if (rep == 'N' || rep == 'n') return false;
		}
	}
}

void Sound_Game_1()
{
	if (turn_on_sound) PlaySound(TEXT("TICTACTOE.wav"), NULL, SND_SYNC);
	else _getch();
}

void processing_game_PvP()
{
	int pos_x_on_screen = 2, pos_y_on_screen = 1, count_player_moves = Count;
	int pos_x_on_board = 0, pos_y_on_board = 0;
	bool win = 0;
	if (rep2 == 'Y' || rep2 == 'y') {
		pos_x_on_screen = saved.X_on_screen;
		pos_y_on_screen = saved.Y_on_screen;
		pos_x_on_board = saved.X;
		pos_y_on_board = saved.Y;
	}
	set_color(7, 13);
	GoTo(pos_x_on_screen - 1, pos_y_on_screen);
	cout << "[";
	GoTo(pos_x_on_screen + 1, pos_y_on_screen);
	cout << "]";
	GoTo(pos_x_on_screen, pos_y_on_screen);

	while (!win)
	{
		disable_selection(); // Disable mouse click and selection on screen

		set_color(7, 9);

		move_cursor_PvP(pos_x_on_screen, pos_y_on_screen,
			count_player_moves, pos_x_on_board, pos_y_on_board);

		ofs.open("saved_position.txt", ofstream::app);
		ofs << pos_x_on_board << " ";
		ofs << pos_y_on_board << " ";
		ofs << pos_x_on_screen << " ";
		ofs << pos_y_on_screen << endl;
		ofs.close();

		check_consecutive(pos_x_on_board, pos_y_on_board);
		win = check_win(pos_x_on_board, pos_y_on_board);

		// update rest move

		if (count_player_moves % 2 == 1) {
			game.X_steps--;
			game.countX++;
			ofs.open("count_step_X.txt", ofstream::out);
			ofs << game.countX << endl;
			ofs.close();
		}
		GoTo(0, 2 * n + 2);
		cout << game.X_steps;
		if (game.X_steps < 100 && game.X_steps > 9) {
			GoTo(2, 2 * n + 2);
			cout << ' ';
		}
		if (game.X_steps < 10) {
			GoTo(1, 2 * n + 2);
			cout << ' ';
		}
		if (count_player_moves % 2 == 0) {
			game.O_steps--;
			game.countY++;
			ofs.open("count_step_O.txt", ofstream::out);
			ofs << game.countY << endl;
			ofs.close();
		}
		GoTo(0, 2 * n + 4);
		cout << game.O_steps;
		if (game.O_steps < 100 && game.O_steps > 9) {
			GoTo(2, 2 * n + 4);
			cout << ' ';
		}
		if (game.O_steps < 10) {
			GoTo(1, 2 * n + 4);
			cout << ' ';
		}
		GoTo(pos_x_on_screen, pos_y_on_screen);

		if (count_player_moves == limit_step * 2 || count_player_moves == n * m 
			|| (game.X_steps == 0 && game.O_steps == 0)) break;
	}

	GoTo(0, 2 * n + 5);
	if (win) {
		if (count_player_moves % 2 == 0) {
			cout << o.icon;
			set_color(7, 0);
			cout << " is the winner!\n";
			user.win_of_P2++;
		}
		else {
			cout << x.icon;
			set_color(7, 0);
			cout << " is the winner!\n";
			user.win_of_P1++;
		}
	}
	else {
		set_color(7, 0);
		cout << "It's tie! Nobody is the winner!\n";
	}
	ofs.open("saved_position.txt", ofstream::out);
	ofs << "";
	ofs.close();

	ofs.open("accounts_2.txt", ofstream::out);
	ofs << "";
	ofs.close();

	ofs.open("accounts_2.txt", ofstream::app);
	ofs << user.username << " ";
	ofs << user.password << " ";
	ofs << user.win_of_E << " ";
	ofs << user.win_of_P << " ";
	ofs << user.win_of_P1 << " ";
	ofs << user.win_of_P2 << endl;
	ofs.close();

	char tmp[MAX] = "0", tmp2[MAX] = "0";
	int a, b, c, d;
	// Update statistical outcomes
	ofs.open("accounts_2.txt", ofstream::app);
	ifs.open("accounts.txt");
	while (!ifs.eof()) {
		ifs >> tmp;
		if (strcmp(tmp, user.username) == 0) {
			ifs >> tmp2;
			ifs >> a;
			ifs >> b;
			ifs >> c;
			ifs >> d;
		}
		else {
			ifs >> tmp2;
			ifs >> a;
			ifs >> b;
			ifs >> c;
			ifs >> d;
			ofs << tmp << " ";
			ofs << tmp2 << " ";
			ofs << a << " ";
			ofs << b << " ";
			ofs << c << " ";
			ofs << d << endl;
		}
	}
	ifs.close();
	ofs.close();

	ofs.open("accounts.txt", ofstream::out);
	ofs << "";
	ofs.close();

	ofs.open("accounts.txt", ofstream::app);
	ifs.open("accounts_2.txt");
	while (!ifs.eof()) {
		ifs >> tmp;
		ifs >> tmp2;
		ifs >> a;
		ifs >> b;
		ifs >> c;
		ifs >> d;
		ofs << tmp << " ";
		ofs << tmp2 << " ";
		ofs << a << " ";
		ofs << b << " ";
		ofs << c << " ";
		ofs << d << endl;
	}
	ifs.close();
	ofs.close();

	Sound_Game_1();
}

void processing_P_go_first()
{
	int pos_x_on_screen = 2, pos_y_on_screen = 1, count_player_moves = Count;
	int pos_x_on_board = 0, pos_y_on_board = 0;
	bool win = 0;
	set_color(7, 13);
	GoTo(1, 1);
	cout << "[";
	GoTo(3, 1);
	cout << "]";
	GoTo(2, 1);

	while (!win)
	{
		disable_selection(); // Disable mouse click and mouse selection on screen

		set_color(7, 9);

		move_cursor_PvE(pos_x_on_screen, pos_y_on_screen,
			count_player_moves, pos_x_on_board, pos_y_on_board);

		ofs.open("saved_position.txt", ofstream::app);
		ofs << pos_x_on_board << " ";
		ofs << pos_y_on_board << " ";
		ofs << pos_x_on_screen << " ";
		ofs << pos_y_on_screen << endl;
		ofs.close();

		check_consecutive(pos_x_on_board, pos_y_on_board);
		win = check_win(pos_x_on_board, pos_y_on_board);
		// update rest move
		if (count_player_moves % 2 == 1) {
			game.X_steps--;
			game.countX++;
			ofs.open("count_step_X.txt", ofstream::out);
			ofs << game.countX << endl;
			ofs.close();
		}
		GoTo(0, 2 * n + 2);
		cout << game.X_steps;
		if (game.X_steps < 100 && game.X_steps > 9) {
			GoTo(2, 2 * n + 2);
			cout << ' ';
		}
		if (game.X_steps < 10) {
			GoTo(1, 2 * n + 2);
			cout << ' ';
		}
		if (count_player_moves % 2 == 0) {
			game.O_steps--;
			game.countY++;
			ofs.open("count_step_O.txt", ofstream::out);
			ofs << game.countY << endl;
			ofs.close();
		}
		GoTo(0, 2 * n + 4);
		cout << game.O_steps;
		if (game.O_steps < 100 && game.O_steps > 9) {
			GoTo(2, 2 * n + 4);
			cout << ' ';
		}
		if (game.X_steps < 10) {
			GoTo(1, 2 * n + 4);
			cout << ' ';
		}
		GoTo(pos_x_on_screen, pos_y_on_screen);

		if (count_player_moves == limit_step * 2 || count_player_moves == n * m
			|| (game.X_steps == 0 && game.O_steps == 0)) break;
	}

	GoTo(0, 2 * n + 5);
	if (win) {
		if (count_player_moves % 2 == 0) {
			cout << "You lose!!!\n";
			user.win_of_E++;
		}
		else {
			cout << "You win!!!\n";
			user.win_of_P++;
		}
	}
	else {
		set_color(7, 0);
		cout << "It's tie! Nobody is the winner!\n";
	}

	ofs.open("saved_position.txt", ofstream::out);
	ofs << "";
	ofs.close();

	ofs.open("accounts_2.txt", ofstream::out);
	ofs << "";
	ofs.close();

	ofs.open("accounts_2.txt", ofstream::app);
	ofs << user.username << " ";
	ofs << user.password << " ";
	ofs << user.win_of_E << " ";
	ofs << user.win_of_P << " ";
	ofs << user.win_of_P1 << " ";
	ofs << user.win_of_P2 << endl;
	ofs.close();

	char tmp[MAX] = "0", tmp2[MAX] = "0";
	int a, b, c, d;
	// Update statistical outcomes
	ofs.open("accounts_2.txt", ofstream::app);
	ifs.open("accounts.txt");
	while (!ifs.eof()) {
		ifs >> tmp;
		if (strcmp(tmp, user.username) == 0) {
			ifs >> tmp2;
			ifs >> a;
			ifs >> b;
			ifs >> c;
			ifs >> d;
		}
		else {
			ifs >> tmp2;
			ifs >> a;
			ifs >> b;
			ifs >> c;
			ifs >> d;
			ofs << tmp << " ";
			ofs << tmp2 << " ";
			ofs << a << " ";
			ofs << b << " ";
			ofs << c << " ";
			ofs << d << endl;
		}
	}
	ifs.close();
	ofs.close();

	ofs.open("accounts.txt", ofstream::out);
	ofs << "";
	ofs.close();

	ofs.open("accounts.txt", ofstream::app);
	ifs.open("accounts_2.txt");
	while (!ifs.eof()) {
		ifs >> tmp;
		ifs >> tmp2;
		ifs >> a;
		ifs >> b;
		ifs >> c;
		ifs >> d;
		ofs << tmp << " ";
		ofs << tmp2 << " ";
		ofs << a << " ";
		ofs << b << " ";
		ofs << c << " ";
		ofs << d << endl;
	}
	ifs.close();
	ofs.close();

	Sound_Game_1();
}

void processing_E_go_first()
{
	int pos_x_on_screen, pos_y_on_screen, count_player_moves = Count;
	int pos_x_on_board, pos_y_on_board;
	bool win = 0;

	game.O_steps--;

	pos_x_on_screen = (n / 2) * 4 + 2;
	pos_y_on_screen = (m / 2) * 2 + 1;
	pos_x_on_board = n / 2;
	pos_y_on_board = m / 2;
	GoTo(pos_x_on_screen, pos_y_on_screen);
	set_color(7, o.color);
	game.pos[pos_x_on_board][pos_y_on_board].icon = o.icon;
	cout << game.pos[pos_x_on_board][pos_y_on_board].icon;

	game.pos[pos_x_on_board][pos_y_on_board].mainDiagonal = 1;
	game.pos[pos_x_on_board][pos_y_on_board].oppositeDiagonal = 1;
	game.pos[pos_x_on_board][pos_y_on_board].horizontal = 1;
	game.pos[pos_x_on_board][pos_y_on_board].vertical = 1;

	set_color(7, 13);
	GoTo(pos_x_on_screen - 1, pos_y_on_screen);
	cout << "[";
	GoTo(pos_x_on_screen + 1, pos_y_on_screen);
	cout << "]";
	GoTo(pos_x_on_screen, pos_y_on_screen);

	while (!win)
	{
		disable_selection(); // Disable mouse click and mouse selection on screen

		set_color(7, 9);

		move_cursor_PvE(pos_x_on_screen, pos_y_on_screen,
			count_player_moves, pos_x_on_board, pos_y_on_board);

		ofs.open("saved_position.txt", ofstream::app);
		ofs << pos_x_on_board << " ";
		ofs << pos_y_on_board << " ";
		ofs << pos_x_on_screen << " ";
		ofs << pos_y_on_screen << endl;
		ofs.close();

		check_consecutive(pos_x_on_board, pos_y_on_board);
		win = check_win(pos_x_on_board, pos_y_on_board);

		// update rest move
		if (count_player_moves % 2 == 0)
		{
			game.X_steps--;
			game.countX++;
			ofs.open("count_step_X.txt", ofstream::out);
			ofs << game.countX << endl;
			ofs.close();
		}
		GoTo(0, 2 * n + 2);
		cout << game.X_steps;
		if (game.X_steps < 100 && game.X_steps > 9) {
			GoTo(2, 2 * n + 2);
			cout << ' ';
		}
		if (game.X_steps < 10) {
			GoTo(1, 2 * n + 2);
			cout << ' ';
		}
		if (count_player_moves % 2 == 1) {
			game.O_steps--;
			game.countY++;
			ofs.open("count_step_O.txt", ofstream::out);
			ofs << game.countY << endl;
			ofs.close();
		}

		GoTo(0, 2 * n + 4);
		cout << game.O_steps;
		if (game.O_steps < 100 && game.O_steps > 9) {
			GoTo(2, 2 * n + 4);
			cout << ' ';
		}
		if (game.X_steps < 10) {
			GoTo(1, 2 * n + 4);
			cout << ' ';
		}
		GoTo(pos_x_on_screen, pos_y_on_screen);

		if (count_player_moves == n * m || (game.X_steps == 0 && game.O_steps == -1)) break;
	}

	GoTo(0, 2 * n + 5);
	if (win) {
		if (count_player_moves % 2 == 0) {
			cout << "You lose!!!\n";
			user.win_of_E++;
		}
		else {
			cout << "You win!!!\n";
			user.win_of_P++;
		}
	}
	else {
		set_color(7, 0);
		cout << "It's tie! Nobody is the winner!\n";
	}

	ofs.open("saved_position.txt", ofstream::out);
	ofs << "";
	ofs.close();

	ofs.open("accounts_2.txt", ofstream::out);
	ofs << "";
	ofs.close();

	ofs.open("accounts_2.txt", ofstream::app);
	ofs << user.username << " ";
	ofs << user.password << " ";
	ofs << user.win_of_E << " ";
	ofs << user.win_of_P << " ";
	ofs << user.win_of_P1 << " ";
	ofs << user.win_of_P2 << endl;
	ofs.close();

	char tmp[MAX] = "0", tmp2[MAX] = "0";
	int a, b, c, d;

	// Update statistical outcomes
	ofs.open("accounts_2.txt", ofstream::app);
	ifs.open("accounts.txt");
	while (!ifs.eof()) {
		ifs >> tmp;
		if (strcmp(tmp, user.username) == 0) {
			ifs >> tmp2;
			ifs >> a;
			ifs >> b;
			ifs >> c;
			ifs >> d;
		}
		else {
			ifs >> tmp2;
			ifs >> a;
			ifs >> b;
			ifs >> c;
			ifs >> d;
			ofs << tmp << " ";
			ofs << tmp2 << " ";
			ofs << a << " ";
			ofs << b << " ";
			ofs << c << " ";
			ofs << d << endl;
		}
	}
	ifs.close();
	ofs.close();

	ofs.open("accounts.txt", ofstream::out);
	ofs << "";
	ofs.close();

	ofs.open("accounts.txt", ofstream::app);
	ifs.open("accounts_2.txt");
	while (!ifs.eof()) {
		ifs >> tmp;
		ifs >> tmp2;
		ifs >> a;
		ifs >> b;
		ifs >> c;
		ifs >> d;
		ofs << tmp << " ";
		ofs << tmp2 << " ";
		ofs << a << " ";
		ofs << b << " ";
		ofs << c << " ";
		ofs << d << endl;
	}
	ifs.close();
	ofs.close();

	Sound_Game_1();
}