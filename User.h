#pragma once
#include "Player.h"
#include "Ship.h"
#include "Tile.h"

class User : public Player {
	public:
		User(int size);

		char name[1024];
		int score;
		
		void setShips();
		void printStatus();
		bool moveShip();
		void printShipStatus();

		bool fire(Player * ai);
};

