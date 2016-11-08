#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "Player.h"
#include "User.h"
#include "Comp.h"
#include "Ship.h"
#include "Tile.h"

using namespace std;

//score struct used to manage scores
struct Score{
	char name[1024];
	int score;
	int turns;
	Score *next;
} * HEAD = NULL;

//global variables and classes
User * user;
Comp * ai;


//MAIN MENU METHODS
void setWindowSize(int width, int height);
void titleScreen();
void play();
void displayScores();
void menu();

//SCORES
void loadScores();
void saveScores();
void updateScores();
int insertion(char * name, int score, int turns, bool loading);

//TURN METHODS
void turnMenu(bool move);
bool gameOver();
//void printGameStatus();
void printPreviousTurn();

//ENDGAME
void gameResult();
void cleanup();

//Main function
int main()
{
	//Start program
	setWindowSize(120, 55);
	titleScreen();
	loadScores();

	//Main Program loop
	int run = 1;
	while (run){
		char userInput;

		menu();
		userInput = tolower(getchar()); 
		cin.ignore();
		switch (userInput) {
			case 'n':
				play();
				break;
			case 's':
				displayScores();
				break;
			case 'q':
				run = 0;
				saveScores();
				break;
			default:
				cout << "Unrecognized input";
		}
	}
	return 0;
}

void menu()		//menu displays the menu of options for the user
{
	cout << "\n\n Main Menu\n";
	cout << "------------------------------------------------------\n";
	cout << "n: New Game\n";
	cout << "s: View Scores\n";
	cout << "q: Quit\n";
	cout << "\n\nPlease enter a choice (n, s, or q) ---> "; 
}

//Game loop
//Controls turns and cleanup
void play(){

	//Get user input for size of board
	int size;
	bool failed = false;
	do{
		if(failed){
			cout << "Invalid board size! Please try again... \n";
			failed = false;
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}

		cout << "Please enter your desired size of gameboard (minimum 10, maximum 50): \n"; 
		cin >> size;
		cin.ignore();

		//board cannot be less than 10 or greater than 50
		if(size < 10 || size > 50){
			failed = true;
		}
	}while(failed);

	//create Player sub-classes for user and ai
	user = new User(size);
	ai = new Comp(size);
	
	//get user's name
	cout << "Please enter your name: \n"; 
	cin.getline(user->name, 1024, '\n');

	//give user the chance to setup their board
	user->setShips();
	//ai sets up their board
	ai->compSetup();


	bool play = true; //is set to zero when the user quits
	bool firstTurn = true; // used to prevent end of turn data from displaying

	while (play && !gameOver()){
		user->score++; //increment turn
		bool move = false; //set flag that they haven't moved yet

		//Display turn count
		cout << "---------------------------------------" << endl;
		cout << "             TURN " << user->score << endl;
		cout << "---------------------------------------" << endl;
		//previous turn results only shown on turns 2+
		if (!firstTurn){
			printPreviousTurn();
		}
		firstTurn = false; //no longer the first turn
		bool endturn = false; //this flag controls when the user's turn is over
		
		user->printStatus(); //prints the users board and ship status

		while (!endturn){
			turnMenu(move); //print options for the turn

			//get turn option choice
			char turnChoice;
			turnChoice = tolower(getchar()); 
			cin.ignore();
			
			switch (turnChoice){
				case 'm':
					if (!move){
						if (user->moveShip())
							move = true;
						else
							cout << "Invalid move order!";
					}
					break;
				case 'f':
					if (user->fire(ai))
						endturn = true;
					else
						cout << "Invalid firing Order!" << endl;
					break;
				case 'p':
					user->printStatus();
					break;
				case 'e':
					ai->printStatus();
					break;
				case 's':
					endturn = true;
					break;
				case 'q':
					endturn = true;
					play = false;
					break;
				default:
					//invalid user input
					cout << "Invalid input, please try again." << endl;
					cin.clear();
					cin.ignore(INT_MAX, '\n');
					break;
			}
		}
		//ai turn
		ai->compTurn(user);
	}
	//display game result (victory or defeat)
	gameResult();
	//add score to high scores
	updateScores();
	//show high scores
	displayScores();
	//delete user and ai boards and ship lists
	cleanup();
}

//Set window size larger so board size 50 can be seen
void setWindowSize(int Width, int Height){
    _COORD coord;
    coord.X = Width;
    coord.Y = Height;

    _SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
	Rect.Bottom = Height - 1;
    Rect.Right = Width - 1;

    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle
    SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size
    SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size 
}

//fun ascii art
void titleScreen(){
	cout << "         ########     ###    ######## ######## ##       ########  ######  ##     ## #### ########  " << endl
		 << "         ##     ##   ## ##      ##       ##    ##       ##       ##    ## ##     ##  ##  ##     ## " << endl
	 	 << "         ##     ##  ##   ##     ##       ##    ##       ##       ##       ##     ##  ##  ##     ## " << endl
	 	 << "         ########  ##     ##    ##       ##    ##       ######    ######  #########  ##  ########  " << endl
		 << "         ##     ## #########    ##       ##    ##       ##             ## ##     ##  ##  ##        " << endl
		 << "         ##     ## ##     ##    ##       ##    ##       ##       ##    ## ##     ##  ##  ##        " << endl
		 << "         ########  ##     ##    ##       ##    ######## ########  ######  ##     ## #### ##        " << endl
		 <<"\n                                          Steven Lowder \n\n\n\n\n\n\n" << endl;
}

//display turn menu
void turnMenu(bool move){
	cout << "\n\nTurn Options\n";
	cout << "------------------------------------------------------\n";
	cout << (!move ? "m: Move\n" : "");
	cout << "f: Fire!\n";
	cout << "p: Player Status\n";
	cout << "e: Enemy Status\n";
	cout << "s: Skip Turn\n";
	cout << "q: Quit\n";
	cout << "\n\nPlease enter a choice (m, f, p, e, s, or q) ---> "; 
}

//print recap of previous turn
void printPreviousTurn(){
	cout << "\nTURN RESULTS\n";
	cout << "---------------" << endl;
	cout << "Your turn" << endl;
	cout << "---------------" << endl;
	user->printTurn(ai);
	cout << "---------------" << endl;
	cout << "Opponent's turn" << endl;
	cout << "---------------" << endl;
	ai->printTurn(user);
}

//load scores from a file
void loadScores(){
	FILE *filename;
	char name[1024]; 
	int score; 
	int turns;

	filename = fopen("Scores.dbm", "rb");

	if(filename != NULL)
	{
		while(fread(name, sizeof(name), 1, filename) == 1)	// check if there's content in the file
		{
			fread(&score, sizeof(int), 1, filename);
			fread(&turns, sizeof(int), 1, filename);
			insertion(name, score, turns, true);
		}

		fclose(filename);
	}
}

//save scores to a file
void saveScores(){
	FILE *filename;
	struct Score *node = HEAD;

	filename = fopen("Scores.dbm", "wb");   // Please use relative path

	if(filename != NULL)
	{
		while(node != NULL)
		{
			fwrite(node->name, sizeof(node->name), 1, filename);
			fwrite(&node->score, sizeof(int), 1, filename);
			fwrite(&node->turns, sizeof(int), 1, filename);
			node = node->next;	// move to the next node in the linked list
		}
		fclose(filename); 
	}
	else
	{
		cout << "ERROR: Unable to save to the file \"Scores.dbm\"." << endl; 
	}
}

//insert new score into the list
//loading boolean tells the function (if true) to ignore trimming the list to 10
int insertion(char *name, int score, int turns, bool loading)
{
	struct Score *new_node, *iter_node;
	new_node = new Score;

	if (new_node == 0) {
		cout << "Out of memory" << endl;
		return -1;
	}

	strcpy(new_node->name, name);
	new_node->score = score;
	new_node->turns = turns;

	iter_node = HEAD;	// Assign temp to head

	if ((HEAD == NULL)||(new_node->score > iter_node->score)) {	
		new_node->next = HEAD;
		HEAD = new_node;
	} 
	else
	{
		while (iter_node->next != NULL) {	
			if (new_node->score > iter_node->next->score) {	
				new_node->next = iter_node->next;
				iter_node->next = new_node;
				return 0;
			} else 
				iter_node = iter_node->next;
		}

		new_node->next = NULL;
		iter_node->next = new_node; 
	}

	//remove any score past top ten
	if (!loading) {
		iter_node = HEAD;
		int i = 0;
		while (i < 10 && iter_node->next != NULL) {
			iter_node = iter_node->next;
			i++;
		}
		iter_node->next = NULL;
	}
	return 0;
}

//calculate the score and add it to the high score list
void updateScores(){
	//only add score if user won
	if(!ai->shipsLeft()){
		int score;
		int shipsLeft = 0;
		int turns = user->score;

		//determine # of ships left
		Ship * ship = user->shipList;
		while (ship != NULL){
			if(ship->hp > 0)
				shipsLeft++;
			ship = ship->next;
		}


		//calculate score = (10,000 - turns*50)/100 + shipsLeft*5
		score = (1000 - user->score * 30) + shipsLeft * 5;
		insertion(user->name, score, turns, false);
	}
}

//print the high scores to the screen
void displayScores() {
	struct Score *iter_node = HEAD; 

	cout << "\n\n\n\n---------------------";
	cout << endl << "HIGH SCORES " << endl; 
	cout << "---------------------" << endl;
	if(iter_node == NULL){
		cout << "No previous high scores." << endl;
	} else {
		cout << "Name - Score (Turns)" << endl;
		while (iter_node != NULL){
			cout << endl << iter_node->name << " - " 
				<< iter_node->score 
				<< " (" << iter_node->turns << ")" << endl;

			iter_node = iter_node->next;
		}
	}
}

//memory management
void cleanup(){
	delete user;
	delete ai;
}

//Checks each user's ship to ensure they have any left
//returns true if one player's ships are all destroyed
bool gameOver(){
	if(!ai->shipsLeft() || !user->shipsLeft())
		return true;
	else
		return false;
}

//print end of game result
void gameResult(){
	cout << "\n\n\n---------------------" << endl;
	if(!ai->shipsLeft()){
		cout << "VICTORY IN " << user->score << " TURNS" << endl;
	} else {
		cout << "    DEFEAT    " << endl;
	}
	cout << "---------------------" << endl;
}
