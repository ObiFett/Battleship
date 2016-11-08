#ifndef iostream
	#include <iostream>
	#include <stdlib.h>
	#include <string.h>
#endif
using namespace std;

#include "Tile.h"

//constructor sets col and row equal to inputs
//all other pointers are set to null
//wreckage is set to false
Tile::Tile(int col, int row){
	this->col = col;
	this->row = row;

	this->resident = NULL;
	this->wreckage = false;

	this->left = NULL;
	this->right = NULL;
	this->up = NULL;
	this->down = NULL;
}

//print the tile for board purposes
void Tile::print(bool showShip){
	if (this->resident != NULL && showShip) {
		this->resident->boardPrint();
	} else if (this->wreckage){
		cout << "##";
	} else {
		cout << "~~";
	}
}

//check to see if tile is empty
bool Tile::empty(){
	if (this->resident == NULL && !this->wreckage)
		return true;
	return false;
}