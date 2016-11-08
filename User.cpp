#ifndef iostream
	#include <iostream>
	#include <stdlib.h>
	#include <string.h>
	#include <time.h>
#endif
using namespace std;

#include "User.h"

//set the turn/score to 0, passes size to parent constructor
User::User(int size) : Player(size){
	this->score = 0;
}

//calls printboard and print status
void User::printStatus(){
	this->printBoard();
	this->printShipStatus();
}

//input and error handling for setting ships on the board
//before the game starts
void User::setShips(){
	while(!this->shipsSet()){
		this->printShipStatus();
		this->printBoard();

		int col; //get userinput ship column
		int row; //get userinput ship row
		char abbr[3];

		cout << "Place (Re-place) a ship by entering the ship's abbreviation followed by desired column and row." << endl;
		cout << "Example: \"Ba 2 3\" would place your Battleship in column 2, row 3." << endl;
		cout << "Placing your final ship will end the setup process." << endl << endl;

		cin >> abbr >> col >> row;
		cin.ignore(INT_MAX, '\n');

		bool validInput = true;
		shipType ship;

		abbr[0] = tolower(abbr[0]);
		abbr[1] = tolower(abbr[1]);
	
		char * convert[5] = {"ba", "ca", "cr", "de", "su"};

		int p = -1;

		for(int i = 0; i < 5; i++){
			if(strcmp(abbr, convert[i]) == 0){
				p = i;
			}
		}

		switch(p){
			case 0:
				ship = BATTLESHIP;
				break;
			case 1:
				ship = CARRIER;
				break;
			case 2:
				ship = CRUISER;
				break;
			case 3:
				ship = DESTROYER;
				break;
			case 4:
				ship = SUBMARINE;
				break;
			default:
				validInput = false;
		}
		//makes sure the desired placement is on the board
		if(col <= 0 || col > boardSize || row <= 0 || row > boardSize){
			validInput = false;
		}

		//if passed all checks, try to set the ship
		if (validInput){
			cout << (this->setShip(ship, col, row) ? "" : "There is a ship already there!") << endl << endl;
		} else {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "Your input was invalid, please try again." << endl << endl;
		}
		cout << endl << endl;
	}
}

//input and error handling for user move option
bool User::moveShip(){

	char abbr[3];
	char dir;

	this->printShipStatus();
	cout << "Choose a ship to move by entering the ship abbreviation and the desired direction." << endl;
	cout << "Directions: u for UP, d for DOWN, l for LEFT and r for RIGHT" << endl;
	cout << "Example: \"Ba u\" would move your Battleship up one space." << endl;

	cin >> abbr >> dir;
	cin.ignore();

	bool validInput = true;
	shipType ship;

	abbr[0] = tolower(abbr[0]);
	abbr[1] = tolower(abbr[1]);
	
	char * convert[5] = {"ba", "ca", "cr", "de", "su"};

	int p = -1;

	for(int i = 0; i < 5; i++){
		if(strcmp(abbr, convert[i]) == 0){
			p = i;
		}
	}

	switch(p){
		case 0:
			ship = BATTLESHIP;
			break;
		case 1:
			ship = CARRIER;
			break;
		case 2:
			ship = CRUISER;
			break;
		case 3:
			ship = DESTROYER;
			break;
		case 4:
			ship = SUBMARINE;
			break;
		default:
			validInput = false;
	}

	switch (tolower(dir)){
		case 'u':
		case 'd':
		case 'l':
		case 'r':
			break;
		default:
			validInput = false;
	}

	//if input is valid so far
	if(validInput){
		//get ship pointer
		Ship * tempShip = this->shipList;
		while (tempShip != NULL){
			if(ship == tempShip->name)
			{
				validInput = tempShip->move(dir); //attempt to move the ship
			}
			tempShip = tempShip->next;
		}
	} else {
		cout << "Invalid move. Please try again.";
		cin.clear();
		cin.ignore(INT_MAX, '\n');
	}

	return validInput;
}

//print user ship status, displays health and location
void User::printShipStatus(){
	cout << "\n\nShipname (Abbr) | HP | Col Row" << endl;
	cout << "------------------------------" << endl;
	
	Ship * tempShip = this->shipList;
	while (tempShip != NULL){
		tempShip->statusPrint(true);
		tempShip = tempShip->next;
	}
	cout << endl;
}

bool User::fire(Player * opponent){

		//print info so they can see wreck placements before firing
		cout << endl << endl << "AI STATUS";
		opponent->printBoard();
		opponent->printShipStatus();

		cout << "YOUR SHIP STATUS";
		this->printShipStatus(); //show valid ships
		cout << "YOUR SUB CAN " << (this->subCanFire ? "" : "NOT ") << "FIRE" << endl; //can sub fire?

		int col; //get userinput ship column
		int row; //get userinput ship row
		char abbr[3];

		cout << "Fire at a tile by entering the firing ship's abbreviation followed by desired column and row." << endl;
		cout << "Example: \"Ba 2 3\" would have your Battleship fire at column 2 and row 3." << endl;
		cout << "Destroyed Ships may not fire!" << endl << endl;

		cin >> abbr >> col >> row;
		cin.ignore();

		bool validInput = true;
		shipType ship;

		//process input
		abbr[0] = tolower(abbr[0]);
		abbr[1] = tolower(abbr[1]);
	
		char * convert[5] = {"ba", "ca", "cr", "de", "su"};

		int p = -1;

		for(int i = 0; i < 5; i++){
			if(strcmp(abbr, convert[i]) == 0){
				p = i;
			}
		}

		switch(p){
			case 0:
				ship = BATTLESHIP;
				break;
			case 1:
				ship = CARRIER;
				break;
			case 2:
				ship = CRUISER;
				break;
			case 3:
				ship = DESTROYER;
				break;
			case 4:
				ship = SUBMARINE;
				break;
			default:
				return false;
		}

		//make sure the target is in bounds
		if(col <= 0 || col > boardSize || row <= 0 || row > boardSize){
			return false;
		}

		//get firing ship
		Ship * tempShip = this->shipList;
		while (tempShip != NULL){
			if(tempShip->name == ship && tempShip->hp != 0)
				break;
			tempShip = tempShip->next;
		}

		//make sure the ship is valid
		if(tempShip == NULL)
			return false;
		//if they chose sub, can it fire?
		if(tempShip->name == SUBMARINE && !this->subCanFire)
			return false;

		//if all previous checks passed, fire with the ship
		if (validInput){

			//fire and record result in last turn variables
			tempShip->fire(opponent, subCanFire, col, row);
			
			//toggle if their sub can fire next turn 
			//based on if sub is firing this turn
			if(tempShip->name == SUBMARINE){
				this->subCanFire = false;
			} else {
				this->subCanFire = true;
			}
						
			this->fireCol = col;
			this->fireRow = row;

			//determine if any ships were hit
			Ship * ship = opponent->shipList;
			this->fireResult = false;
			for(int i = 0; i < 5; i++){
				if (opponent->previousHealth[i] != ship->hp){
					this->fireResult = true;
					this->shipHit = ship;
				}
				ship = ship->next;
			}
			
			return true;
		} else {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
		cout << endl << endl;
		return false;
}
