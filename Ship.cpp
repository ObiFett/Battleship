#ifndef iostream
	#include <iostream>
	#include <stdlib.h>
	#include <string.h>
#endif
using namespace std;

#include "Ship.h"

//constructor needs an enumerated type
//sets hp and name
Ship::Ship(shipType type){

	this->name = type;
	this->location = NULL;
	
	switch (type)
	{
		case CARRIER:
			this->hp = 5;
			strcpy(printName, "Carrier (Ca)   ");
			break;
		case BATTLESHIP:
			this->hp = 4;
			strcpy(printName, "Battleship (Ba)");
			break;
		case CRUISER:
			this->hp = 3;
			strcpy(printName, "Cruiser (Cr)   ");
			break;
		case SUBMARINE:
			this->hp = 2;
			strcpy(printName, "Submarine (Su) ");
			break;
		case DESTROYER:
			this->hp = 2;
			strcpy(printName, "Destroyer (De) ");
			break;
	}

}

//called when a ship gets hit
//decrements health and calls destroyed if health hits 0
void Ship::onHit(Player * player){
	
	this->hp--;

	if(this->hp==0)
		this->destroyed(player);

}

//called to fire on a specific tile
//player pointer = target board
//bool sub is whether the firing player's sub is allowed to fire
//col and row are target squares coords
//returns if the target tile caused a hit or not
bool Ship::fire(Player * opponent, bool sub, int col, int row){
	
	//find target tile
	Tile * tile = opponent->findTile(opponent, col, row);

	//if the sub is firing and it can, hit the two squares below the target square
	if(sub && this->name == SUBMARINE){
		if (tile->down != NULL) {
			this->fire(opponent, false, tile->col, tile->down->row);
			if(tile->down->down != NULL)
				this->fire(opponent, false, tile->col, tile->down->down->row);
		}
	}

	//check tile for ship, if ship there call its onHit function
	if(tile != NULL && tile->resident != NULL){
			tile->resident->onHit(opponent);
			return true;
	}
	return false;
}

//called when a ship determines it is destroyed
//if its a battleship or carrier, hit two squares to the right
void Ship::destroyed(Player * player){

	if(this->name == BATTLESHIP || this->name == CARRIER){
		this->fire(player, false, this->location->right->col, this->location->row);
		this->fire(player, false, this->location->right->right->col, this->location->row);
	}
	if(this->name != SUBMARINE) {
		this->location->wreckage = true;
	}
	this->location->resident = NULL;
	this->location = NULL;
}

//used to move a ship
//char dir is handled inside
//returns wether the move was successful or not
bool Ship::move(char dir){
	Tile * currTile = this->location;
	Tile * destTile;

	bool validMove = false;

	//if its dead
	if(!this->hp){
		return validMove;
	}

	switch (dir){
		case 'u':
			destTile = currTile->up;
			break;
		case 'd':
			destTile = currTile->down;
			break;
		case 'l':
			destTile = currTile->left;
			break;
		case 'r':
			destTile = currTile->right;
			break;
		default:
			destTile = NULL;
			break;
	}

	//is the destination tile empty and valid?
	if(destTile != NULL && destTile->empty()){
		destTile->resident = currTile->resident;
		currTile->resident = NULL;
		this->location = destTile;

		validMove = true;
	}
	return validMove;
}

//used to print the ship's ascii on the board
void Ship::boardPrint(){
	switch (this->name) {
		case BATTLESHIP:
			cout << "Ba";
			break;
		case CARRIER:
			cout << "Ca";
			break;
		case CRUISER:
			cout << "Cr";
			break;
		case DESTROYER:
			cout << "De";
			break;
		case SUBMARINE:
			cout << "Su";
			break;
		default:
			cout << "Print Error";
			break;
	}
}

//used to print the shiplist status
//bool showLoc is set to false when printing ai status
void Ship::statusPrint(bool showLoc){

	cout << printName << " | ";
	if (this->hp > 0)
		cout << this->hp; 
	cout << (showLoc ? "  | " : "");

	string locationInfo;

	if(this->location == NULL){
		if (this->hp > 0)
			cout << "Not Set Yet";
		else
			cout << "Destroyed";
	} else if (showLoc){
		cout << this->location->col << " " << this->location->row;
	}

	 cout << endl; 
}
