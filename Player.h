#pragma once
#include "Tile.h"
#include "Ship.h"

class Ship;
enum shipType;
class Tile;

class Player {
	public:
		Player(int size);
		virtual ~Player();

		Ship * shipList;
		bool subCanFire;

		Tile * board;
		int boardSize;

		//LAST TURN PARAMETERS
		bool fireResult;
		int fireRow;
		int fireCol;
		Ship * shipHit;
		int previousHealth[5];

		bool shipsSet();
		bool shipsLeft();
		bool setShip(shipType ship, int col, int row);
		void createShipList();
		void createBoard(int size);
		
		virtual bool fire(Player * opponent);
		void updatePrevHealth();
		void printTurn(Player * opponent);
		virtual bool moveShip();
		virtual void printShipStatus();

		void printColHeaders();
		void printBoard();
		virtual void printRow(Tile * currPoint, int rowNum);
		virtual void printStatus();	
		Tile * findTile(Player * player, int col, int row);

		void deleteShips(Ship * ship);
		void deleteRow(Tile * tile);
};

