#pragma once
#include "Ship.h"

class Ship;

class Tile
{
	public:
		Tile(int c, int r);

		int col;
		int row;

		Tile * left;
		Tile * right;
		Tile * down;
		Tile * up;

		//Tile data
		Ship * resident;
		bool wreckage;

		void print(bool showShip);

		bool empty();
};

