#pragma once
#include "Tile.h"
#include "Player.h"

enum shipType {CARRIER = 5, BATTLESHIP = 4, CRUISER = 3, SUBMARINE = 2, DESTROYER = 1};
class Tile;
class Player;

class Ship
{
	public:
		Ship(shipType type);

		shipType name;
		char printName[20];
		int hp;
		Tile * location;

		bool fire(Player * opponent, bool sub, int col, int row);
		void onHit(Player * opponent);
		void destroyed(Player * opponent);
		bool move(char dir);
		
		void boardPrint();
		void statusPrint(bool showLoc);

		Ship * next;
};

