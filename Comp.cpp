#ifndef iostream
	#include <iostream>
	#include <stdlib.h>
	#include <string.h>
	#include <time.h>
#endif
using namespace std;

#include "Comp.h"

//default constructor that passes the input to the parent constructor
Comp::Comp(int size) : Player(size){

}

//randomly places ships on the board
void Comp::compSetup(){
	Ship * tempShip = this->shipList;
	srand(time(0)); //set seed so its not the same every time
	while (tempShip != NULL){
		int col = rand() % boardSize;
		int row = rand() % boardSize;

		if(this->setShip(tempShip->name, col, row)){
			tempShip = tempShip->next;
		}
	}
}

//over-function determines if move is necessary, then fires
void Comp::compTurn(Player * opponent){
	//move ship if hit last turn
	if(opponent->fireResult && opponent->shipHit->hp > 0){
		bool moved = false;
		do{
			moved = this->moveShip(opponent->shipHit);
		} while (!moved);
	}
	this->fire(opponent);
}

//randomly determines a direction to move
bool Comp::moveShip(Ship * ship){
	char dirs[4] = {'u', 'd', 'l', 'r'};
	int r = rand() % 4;
	return ship->move(dirs[r]);
}

//logic for computer firing
//if a ship was hit last turn, fire around that square
//else fire randomly
/////With more time could make this better :(
bool Comp::fire(Player * opponent){
	//choose firing ship
	Ship * tempShip = this->shipList;
	bool subAlive = true;
	bool subFiring = false;
	
	//if the subcan fire
	//find sub and see if its alive
	if(this->subCanFire){
		while(tempShip != NULL){
			if(tempShip->name == SUBMARINE && tempShip->hp == 0)
				subAlive = false; //sub isnt alive
			else if (tempShip->name == SUBMARINE) //if sub is alive, set it to be the firing ship
				break;
			tempShip = tempShip->next;
		}
	}

	//if the subcan fire but the sub isn't alive
	//find any other ship to fire
	if(!this->subCanFire || !subAlive){
		tempShip = this->shipList;
		while (tempShip != NULL) {
			if(tempShip->hp > 0)
				break;
			else
				tempShip = tempShip->next;
		}
	} else {
		subFiring = true;
	}

	//if no ship is able to fire
	//(only case would be sub fired and is last ship alive)
	if(tempShip == NULL){
		this->fireResult = false;
		this->shipHit = NULL;
		this->fireCol = 0;
		this->fireRow = 0;
		return true;
	}

	//if hit a ship last turn, fire around the ship
	int col;
	int row;
	Tile * tempTile;

	//randomize if it thinks the player moved the ship or not
	int repeat = rand() % 2;

	if(this->fireResult && !repeat){
		do {
			char dirs[4] = {'u', 'd', 'l', 'r'};
			int r = rand() % 4;
			 
			col = this->fireCol;
			row = this->fireRow;
			
			switch(dirs[r]){
				case 'u':
					row--;
					break;
				case 'd':
					row++;
					break;
				case 'l':
					col--;
					break;
				case 'r':
				default:
					col++;
					break;
			}
			//check if desired target is in the board and is not a wreck
		} while (col < 0 || col > boardSize || row < 0 || row > boardSize && checkTarget(opponent, col, row));
	} else if (this->fireResult && repeat){
		col = this->fireCol;
		row = this->fireRow;
	} else {
		//fire at a random, non-wrecked square
		bool validFiring = false;

		do{
			col = (rand() % boardSize) + 1;
			row = (rand() % boardSize) + 1;
			//check if desired target has a wreck in it
		} while(!checkTarget(opponent, col, row));
	}

	//find the tile and set last turn variables
	this->fireResult = tempShip->fire(opponent, subFiring, col, row);
	this->fireCol = col;
	this->fireRow = row;

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
}

//check target tile and see if there is a wreck there
bool Comp::checkTarget(Player * opponent, int col, int row){
	Tile * tempTile = this->findTile(opponent, col, row);
	if(tempTile->wreckage)
		return false;
	else
		return true;
}

//overridden function to hide ship's locations on the board
//still prints wrecks
void Comp::printRow(Tile * currPoint, int rowNum){
	cout << rowNum << (rowNum >= 10 ? "": " ") << "|";
	while (currPoint != NULL){
		currPoint->print(false);
		currPoint = currPoint->right;
	}
	cout << "|" << endl;
}

//print board and ship status
//used when the user chooses to fire
void Comp::printStatus(){
	this->printBoard();
	this->printShipStatus();
}

//overridden function to not show location of ships
void Comp::printShipStatus(){
	cout << "\n\nShipname (Abbr) | HP " << endl;
	cout << "-------------------------" << endl;
	
	Ship * tempShip = this->shipList;
	while (tempShip != NULL){
		tempShip->statusPrint(false);
		tempShip = tempShip->next;
	}
	cout << endl;
}