#include<iostream>
#include<conio.h>  //so that we can use getch();
#include<windows.h>

using namespace std;

void gotoxy(short col, short row)     //this function is made global so that all the functions in parent class as well as in child classes
									  //can use this
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { col,row };
	SetConsoleCursorPosition(h, position);
}

class sudoku_grid  //abstract class so that we can do polymorphism
{
protected:
	char grid_unsolved[9][9];
	char grid_solved[9][9];
	char row_number[9];
	char column_number[9];
	char check_fixed_numbers[36][2];
	int position_pointer;

public:
	virtual void temp_func() = 0;   //this function is made pure virtual intentionally to make
									//the class virtual so that objects of this class can
									//not be instantiated

	sudoku_grid()
	{
		position_pointer = 1;
		char row_number_copy[9] = { 'a','b','c','d','e','f','g','h','i' };   //so that we can use initializer list
		char column_number_copy[9] = { 'A','B','C','D','E','F','G','H','I' };

		for (int i = 0; i < 9; i++)
			row_number[i] = row_number_copy[i];
		for (int i = 0; i < 9; i++)
			column_number[i] = column_number_copy[i];
	}

	int return_pointer_position()
	{
		return position_pointer;
	}
	void box(int x_start, int y_start, int col, int row)
	{
		int c = 1;
		int r = 1;
		gotoxy(x_start, y_start);
		cout << static_cast<char>(201);
		gotoxy(x_start + col + 1, y_start);
		cout << static_cast<char>(187);
		gotoxy(x_start, y_start + row + 1);
		cout << static_cast<char>(200);
		gotoxy(x_start + col + 1, y_start + row + 1);
		cout << static_cast<char>(188);


		while (c <= col + 2)
		{
			gotoxy(x_start + c - 1, y_start);
			cout << static_cast<char>(254);
			gotoxy(x_start + c - 1, y_start + row + 1);
			cout << static_cast<char>(254);
			c++;
		}

		while (r <= row)
		{
			gotoxy(x_start, y_start + r);
			cout << static_cast<char>(219);
			gotoxy(x_start + col + 1, y_start + r);
			cout << static_cast<char>(219);
			r++;
		}
	}
	void internal_line_h(int x_coord, int y_coord, int number_lines) //give start x and y position of box    number_lines=number of horizontal lines in grid
	{
		int y = y_coord + 4;  //to store the position of starting x_coord and change the value after every while loop iteration
							  //so that the new line prints at new position     -> +4 so that the first line starts four pixel below the box boundary

		for (int i = 1; i <= number_lines; i++, y = y + 4)  //loop runs to number_lines to generate required number of lines
		{
			for (int j = x_coord + 1; j < x_coord + 81; j++)  //x_coord + number determines number of characters on each line
			{
				gotoxy(j, y);
				if (i % 3 != 0)
					cout << static_cast<char>(196);
				else
					cout << static_cast<char>(254);
			}
		}
	}
	void internal_line_v(int x_coord, int y_coord, int number_lines)
	{
		int x = x_coord + 9;
		for (int i = 1; i <= number_lines; i++, x = x + 9)
		{
			for (int j = y_coord + 1; j < y_coord + 36; j++)
			{
				gotoxy(x, j);
				if (i % 3 != 0)
					cout << static_cast<char>(179);
				else
					cout << static_cast<char>(219);
			}
		}
	}
	void display_grid(int x_coord_box, int y_coord_box, int box_width, int box_height) //to display the grid based on difficulty level
	{
		for (int i = 0, y = y_coord_box + 2; y < box_height, i < 9; y = y + 4, i++)
		{
			for (int j = 0, x = x_coord_box + 4; x<box_width, j<9; x = x + 9, j++)
			{
				gotoxy(x, y);
				for (int u = 0; u < 36; u++)
				{
					if ((check_fixed_numbers[u][0] - 97) == i && (check_fixed_numbers[u][1] - 65) == j)
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
						break;
					}
					else
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				cout << grid_unsolved[i][j];
			}
		}
		for (int i = 0, x = x_coord_box + 4; i <9, x<x_coord_box + box_width; i++, x = x + 9)  //for printing column number A-I
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			gotoxy(x, y_coord_box - 1);
			cout << column_number[i];
		}

		for (int i = 0, y = y_coord_box + 2; i < 9, y<y_coord_box + box_height; i++, y = y + 4)  //for printing row number a-i
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			gotoxy(x_coord_box - 2, y);
			cout << row_number[i];
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	}
	void display()
	{
		box(35, 2, 80, 35);
		internal_line_h(35, 2, 8);
		internal_line_v(35, 2, 8);
		display_grid(35, 2, 80, 35);
	}
	void check_win_func()
	{
		bool check_win = false;

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (grid_unsolved[i][j] != grid_solved[i][j])
					check_win = false;
				else
					check_win = true;
			}
		}

		if (check_win)
		{
			gotoxy(0, 40);
			cout << "Congratulations!! You have won!!" << endl;
			cout << "Press any key to continue......" << endl;
			getch();
			exit(0);
		}
		else
		{
			gotoxy(0, 40);
			cout << "There is some wrong placement of number in your program" << endl;
			cout << "Press any key to continue" << endl;
			getch();
			exit(0);
		}
	}
	void check_complete()
	{
		bool complete = false;
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (grid_unsolved[i][j] == ' ')
					return;
			}
		}

		check_win_func();
	}

	void menu()
	{
		system("cls");

		int selection_pointer = 0;

		while (selection_pointer != 13)   //the value of return key is 13 it is written to get out of loop when enter is pressed
		{                                   //note that 13 is used because me getch use kar raha hu conio.h ka
			system("cls");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			cout << "------Select difficulty level------" << endl;
			selecter(1, position_pointer);     cout << "EASY TO SOLVE  (FOR BEGINNERS)" << endl;  //the position is given so that the arrow is placed up down
			selecter(2, position_pointer);     cout << "MEDIUM LEVEL BOARD  (FOR PLAYERS WHO HAVE PREVIOUS KNOWLEDGE OF SUDOKU)" << endl;
			selecter(3, position_pointer);     cout << "TRICKY  (FOR PLAYERS WHO WANT SOME CHALLENGE)" << endl;

			selection_pointer = getch();
//			fflush(stdin);  //iska pata nahi kyaa use hai lekin enter wala masla khatam kar deta hai

			if (selection_pointer == 80 && position_pointer != 3)  //the value of down key is 80 and !=3 is used because
				position_pointer++;                                //we have only three lines and we do not need
			else                                                  //value of our position to go beyond 3
				if (selection_pointer == 72 && position_pointer != 1)  //value of up key is 72
					position_pointer--;
		}
	}
	void selecter(int selection_value, int position)
	{
		if (selection_value == position)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		else
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	}

	void input()
	{
		char row_position;
		char column_position;
		char number_input;

		gotoxy(0, 40);
		cout << "Enter row number (a-i):";
		row_position = getch();
		cout << row_position << endl;
		cout << "Enter column number (A-I):";
		column_position = getch();
		cout << column_position << endl;

		if (check_valid_input(row_position, column_position))
		{
			cout << "Enter number (1-9):";
			number_input = getch();
			if (number_input > '9' || number_input < '1')
			{
				cout << "Invalid input" << endl;
				cout << "Press any key to input again......";
				getch();
				return;
			}
			else
				grid_unsolved[row_position - 97][column_position - 65] = number_input;
		}
		else
		{
			cout << "Invalid position" << endl;
			cout << "Press any key to input again......";
			getch();
			return;
		}
	}

	bool check_valid_input(int row_pos, int column_pos)
	{
		if (row_pos<'a' || row_pos>'i')
			return false;
		if (column_pos<'A' || column_pos>'I')
			return false;

		for (int i = 0; i < 29; i++)
		{
			if (check_fixed_numbers[i][0] == row_pos&&check_fixed_numbers[i][1] == column_pos)
				return false;
		}
		return true;
	}
};

class easy_grid :public sudoku_grid
{
protected:

public:
	void temp_func()
	{
		cout << "temporary function" << endl;
	}
	easy_grid()
	{
		char grid_unsolved_copy[9][9] = { { '6',' ','9','3','8','2','4',' ',' ' },{ ' ',' ',' ',' ','7',' ',' ','8',' ' },{ ' ',' ',' ',' ',' ',' ','3',' ','5' },{ '4','6',' ',' ',' ','1','8',' ','9' },{ '9',' ','7','6',' ','3','2',' ','1' },{ '5',' ','3','8',' ',' ',' ','7','4' },{ '1',' ','6',' ',' ',' ',' ',' ',' ' },{ ' ','9',' ',' ','6',' ',' ',' ',' ' },{ ' ',' ','4','2','1','5','7',' ','6' } };
		char grid_solved_copy[9][9] = { { '6','5','9','3','8','2','4','1','7' },{ '3','4','1','5','7','6','9','8','2' },{ '2','7','8','1','9','4','3','6','5' },{ '4','6','2','7','5','1','8','3','9' },{ '9','8','7','6','4','3','2','5','1' },{ '5','1','3','8','2','9','6','7','4' },{ '1','2','6','9','3','7','5','4','8' },{ '7','9','5','4','6','8','1','2','3' },{ '8','3','4','2','1','5','7','9','6' } };
		char check_fixed_numbers_copy[36][2] = { { 'a','A' },{ 'a','C' },{ 'a','D' },{ 'a','E' } ,{ 'a','F' },{ 'a','G' } ,{ 'b','E' },{ 'b','H' },{ 'c','G' },{ 'c','I' },{ 'd','A' },{ 'd','B' },{ 'd','F' },{ 'd','G' },{ 'd','I' },{ 'e','A' },{ 'e','C' },{ 'e','D' },{ 'e','F' },{ 'e','G' },{ 'e','I' },{ 'f','A' },{ 'f','C' },{ 'f','D' },{ 'f','H' },{ 'f','I' },{ 'g','A' },{ 'g','C' },{ 'h','B' },{ 'h','E' },{ 'i','C' },{ 'i','D' },{ 'i','E' },{ 'i','F' },{ 'i','G' },{ 'i','I' } };

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				grid_unsolved[i][j] = grid_unsolved_copy[i][j];
			}
		}

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				grid_solved[i][j] = grid_solved_copy[i][j];
			}
		}

		for (int i = 0; i < 36; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				check_fixed_numbers[i][j] = check_fixed_numbers_copy[i][j];
			}
		}
	}
};

class medium_grid :public sudoku_grid
{
protected:

public:
	void temp_func()
	{
		cout << "temporary function" << endl;
	}

	medium_grid()
	{
		char grid_unsolved_copy[9][9] = { { '2','9',' ',' ',' ',' ','5','3',' ' },{ ' ',' ',' ',' ','5','6',' ',' ',' ' },{ ' ',' ','5','7',' ','9','1',' ','2' },{ '3',' ','7','9','4',' ','2','1',' ' },{ ' ',' ','4','1',' ',' ','8',' ',' ' },{ ' ','8',' ',' ',' ',' ',' ','4','9' },{ ' ','6',' ','5',' ',' ',' ',' ',' ' },{ ' ',' ',' ',' ','1',' ','4',' ',' ' },{ ' ',' ',' ',' ','7','3',' ',' ',' ' } };
		char grid_solved_copy[9][9] = { { '2','9','6','4','8','1','5','3','7' },{ '1','7','3','2','5','6','9','8','4' },{ '8','4','5','7','3','9','1','6','2' },{ '3','5','7','9','4','8','2','1','6' },{ '9','2','4','1','6','7','8','5','3' },{ '6','8','1','3','2','5','7','4','9' },{ '7','6','8','5','9','4','3','2','1' },{ '5','3','9','6','1','2','4','7','8' },{ '4','1','2','8','7','3','6','9','5' } };
		char check_fixed_numbers_copy[36][2] = { { 'a','A' },{ 'a','B' },{ 'a','G' },{ 'a','H' },{ 'b','E' },{ 'b','F' },{ 'c','C' },{ 'c','D' },{ 'c','F' },{ 'c','G' },{ 'c','I' },{ 'd','A' },{ 'd','C' },{ 'd','D' },{ 'd','E' },{ 'd','G' },{ 'd','H' },{ 'e','C' },{ 'e','D' },{ 'e','G' },{ 'f','B' },{ 'f','H' },{ 'f','I' },{ 'g','B' },{ 'g','D' },{ 'h','E' },{ 'h','G' },{ 'i','E' },{ 'i','F' } };

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				grid_unsolved[i][j] = grid_unsolved_copy[i][j];
			}
		}

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				grid_solved[i][j] = grid_solved_copy[i][j];
			}
		}

		for (int i = 0; i < 36; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				check_fixed_numbers[i][j] = check_fixed_numbers_copy[i][j];
			}
		}
	}
};

class tricky_grid :public sudoku_grid
{
protected:

public:
	void temp_func()
	{
		cout << "temporary function" << endl;
	}
	tricky_grid()
	{
		char check_fixed_numbers_copy[36][2] = { { 'a','F' } ,{ 'a','G' },{ 'b','B' },{ 'b','C' },{ 'b','H' },{ 'c','A' },{ 'c','B' },{ 'c','D' },{ 'd','D' },{ 'd','E' },{ 'd','G' },{ 'e','B' },{ 'e','E' },{ 'e','H' },{ 'f','C' },{ 'f','E' },{ 'f','F' },{ 'g','F' },{ 'g','H' },{ 'g','I' },{ 'h','B' },{ 'h','G' },{ 'h','H' },{ 'i','C' },{ 'i','D' } };
		char grid_unsolved_copy[9][9] = { { ' ',' ',' ',' ',' ','7','9',' ',' ' },{ ' ','1','2',' ',' ',' ',' ','6',' ' },{ '3','4',' ','2',' ',' ',' ',' ',' ' },{ ' ',' ',' ','4','2',' ','5',' ',' ' },{ ' ','6',' ',' ','8',' ',' ','4',' ' },{ ' ',' ','9',' ','7','3',' ',' ',' ' },{ ' ',' ',' ',' ',' ','1',' ','8','5' },{ ' ','9',' ',' ',' ',' ','2','7',' ' },{ ' ',' ','4','7',' ',' ',' ',' ',' ' } };
		char grid_solved_copy[9][9] = { { '6','8','5','3','1','7','9','2','4' },{ '9','1','2','5','4','8','3','6','7' },{ '3','4','7','2','9','6','1','5','8' },{ '1','7','8','4','2','9','5','3','6' },{ '2','6','3','1','8','5','7','4','9' },{ '4','5','9','6','7','3','8','1','2' },{ '7','2','6','9','3','1','4','8','5' },{ '5','9','1','8','6','4','2','7','3' },{ '8','3','4','7','5','2','6','9','1' } };

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				grid_unsolved[i][j] = grid_unsolved_copy[i][j];
			}
		}

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				grid_solved[i][j] = grid_solved_copy[i][j];
			}
		}

		for (int i = 0; i < 36; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				check_fixed_numbers[i][j] = check_fixed_numbers_copy[i][j];
			}
		}
	}
};

int main()
{
	int selection_difficulty;

	sudoku_grid *grid1, *grid2, *grid3;
	easy_grid egrid;
	medium_grid mgrid;
	tricky_grid tgrid;

	grid1 = &egrid;
	grid2 = &mgrid;
	grid3 = &tgrid;

	grid2->menu();
	selection_difficulty = grid2->return_pointer_position();

	while (true)
	{
		system("cls");
		gotoxy(0, 0);
		switch (selection_difficulty)
		{
		case 1:
			gotoxy(0, 0);
			cout << "Easy to solve board" << endl;
			grid1->display();
			grid1->check_complete();
			grid1->input();
			break;

		case 2:
			gotoxy(0, 0);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			cout << "Medium level board" << endl;
			grid2->display();
			grid2->check_complete();
			grid2->input();
			break;

		case 3:
			gotoxy(0, 0);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			cout << "Tricky board" << endl;
			grid3->display();
			grid3->check_complete();
			grid3->input();
			break;
		default:
			cout << "<---Something is wrong--->" << endl;
		}
	}

	gotoxy(20, 52);
	system("pause");
}
