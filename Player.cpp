#ifndef iostream
	#include <iostream>
	#include <stdlib.h>
	#include <string.h>
#endif
using namespace std;

#include "Player.h"

Player:: Player(int size) {

	this->createShipList();
	this->createBoard(size);

	this->subCanFire = true;
	this->boardSize = size;

	this->fireResult = false;

}

Player::~Player(){
	//delete the board
	for(int i = 0; i < boardSize; i++){
		Tile * currRow = this->board;
		this->board = this->board->down;
		deleteRow(currRow);
	}
	//delete the ship list
	deleteShips(this->shipList);
}

//recursive call for deleting board
void Player::deleteRow(Tile * currRow){
	if(currRow->right != NULL)
		deleteRow(currRow->right);
	delete currRow;
}

//recursive call for deleting shipList
void Player::deleteShips(Ship * ship){
	if(ship->next != NULL)
		deleteShips(ship->next);
	delete ship;
}

//Used to check ships
//if one ship isn't set, returns false.
bool Player::shipsSet(){

	Ship * tempShip = this->shipList;
	bool flag = true; 

	while(tempShip != NULL)
	{
		if(tempShip->location == NULL){
			flag = false;
			break;
		}
		tempShip = tempShip->next;
	}
	return flag;
}

//Used to check ships
//if any ships are left, if one ship is left, returns true.
bool Player::shipsLeft(){
	Ship * tempShip = this->shipList;
	bool flag = false; 

	while(tempShip != NULL)
	{
		if(tempShip->hp > 0){
			flag = true;
			break;
		}
		tempShip = tempShip->next;
	}
	return flag;
}

//places a ship for the user
//returns if it was a valid placement
bool Player::setShip(shipType ship, int col, int row){

	bool validSet = false;

	//get the right tile
	Tile * tile = this->findTile(this, col, row);
	
	//get the right ship
	Ship * tempShip = this->shipList;
	while(tempShip->name != ship)
		tempShip = tempShip->next;

	//if not occupied, assign ship and tile to each other
	if(tile->resident == NULL){
		Tile * tempTile = tempShip->location;

		tile->resident = tempShip;
		tempShip->location = tile;

		//if ship was previously on a tile, clear the resident pointer
		if (tempTile != NULL){
			tempTile->resident = NULL;
		}
		validSet = true;
	}

	return validSet;
}

//setup function that builds the player's shiplist
void Player::createShipList(){
	shipType type[5] = {BATTLESHIP, CARRIER, CRUISER, DESTROYER, SUBMARINE};

	Ship * shipPtr;
	for (int i = 0; i < 5; i++){
		Ship * tempShip = new Ship(type[i]);
		this->previousHealth[i] = tempShip->hp;
		if(i == 0){
			this->shipList = tempShip;
			shipPtr = tempShip;
		} else {
			shipPtr->next = tempShip;
			shipPtr = tempShip;
		}
		tempShip->next = NULL;
	}
}

//setup function that builds the player's board
void Player::createBoard(int size){
	
	//counter variables
	int rowsize = size;
	size = size;
	int startsize = size; 
	int row = 1;

	//counter pointers
	Tile * temp;
	Tile * prevRow, * currRow;
	Tile * rowPoint = NULL;

	//run as long as we still have a row to build
	while (size > 0){
		int col = 1;
		//if this is the first row total
		//we need to setup all the counter pointers
		//and assign the main board pointer to the first created tile
		if (startsize == size){
			int rowplace = rowsize; //row counter loop variable
			while (rowplace > 0){
				temp = new Tile(col, row); //create the new tile
				if(rowplace == rowsize){ //if first tile
					currRow = temp;
					board = temp;
					rowPoint = temp;
				}
				if(col != 1){ //if not the first column
					rowPoint->right = temp;
					temp->left = rowPoint;
				}
				rowPoint = temp;
				col++;
				rowplace--;
			}
		} else {
			int rowplace = rowsize;
			rowPoint = NULL;
			while (rowplace > 0){
				temp = new Tile(col, row);
				if(rowplace == rowsize){
					currRow = temp;
				}
				if(rowPoint != NULL){
					rowPoint->right = temp;
					temp->left = rowPoint;
				}

				rowPoint = temp;
				prevRow->down = temp;
				temp->up = prevRow;
				prevRow = prevRow->right;
				col++;
				rowplace--;
			}
		}
		prevRow = currRow;
		size--;
		row++;
	}
}

//prints the board
void Player::printBoard() {
	Tile * currPoint = this->board;
	int rowNum = 1;

	printColHeaders();

	while (currPoint != NULL) {
		printRow(currPoint, rowNum); //printboard overridden by COMP subclass
		currPoint = currPoint->down;
		rowNum++;
	}
	
	cout << "   ";
	for (int i = 0; i < boardSize; i++){
		cout << "--";
	}
	cout << endl;
}

//recursive call used by printboard
//can be overridden if information needs to be hidden
void Player::printRow(Tile * currPoint, int rowNum){
	cout << rowNum << (rowNum >= 10 ? "": " ") << "|";
	while (currPoint != NULL){
		currPoint->print(true);
		currPoint = currPoint->right;
	}
	cout << "|" << endl;
}

//used to find a specific tile
Tile * Player::findTile(Player * player, int col, int row){
	Tile * tempTile = player->board;
	int c = 1;
	int r = 1;
	while (c < col){
		tempTile = tempTile->right;
		c++;
	}
	while (r < row){
		tempTile = tempTile->down;
		r++;
	}
	return tempTile;
}

//prints the column information before printing the board
void Player::printColHeaders() {
	int i = 0;
	int end = boardSize;

	cout << "\n\n   ";
	while (i < end){
		if (i%2 != 0 || i <= 8){
			cout << i+1 << (i > 8 ? "  " : " ");
		}
		i++;
	}

	i = 0;
	cout << endl << "   ";
	while (i < end) {
		cout << "--";
		i++;
	}
	cout << endl;
}

//print the results of the last turn
void Player::printTurn(Player * player){
	cout << "Target Tile (col row): " << this->fireCol << " " << this->fireRow << endl;
	cout << "Ships Hit [hp left]: " << endl;
	Ship * ship = player->shipList;
	bool newLine = false;
	for(int i = 0; i < 5; i++){
		if(player->previousHealth[i] != ship->hp){
			if(newLine){
				cout << "\n";
			}
			cout << ship->printName << " [" << ship->hp << "] "; 
			newLine = true;
		}
		ship = ship->next;
	}
	cout << endl << endl;

	player->updatePrevHealth();
}

void Player::updatePrevHealth(){
	Ship * ship = this->shipList;

	for(int i = 0; i < 5; i++){
		this->previousHealth[i] = ship->hp;
		ship = ship->next;
	}
}
//following functions are made to be overridden by subclasses
bool Player::fire(Player * opponent){return false;}
bool Player::moveShip(){return false;}
void Player::printShipStatus(){}
void Player::printStatus(){}
