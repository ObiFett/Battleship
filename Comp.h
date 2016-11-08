#pragma once
#include "Player.h"

class Comp : public Player {
	public:
		Comp(int size);

		void compSetup();
		void compTurn(Player * user);

		void printRow(Tile * currPoint, int rowNum);
		void printStatus();
		void printShipStatus();

		bool moveShip(Ship * ship);
		bool fire(Player * opponent);
		bool checkTarget(Player * opponent, int col, int row);
};
