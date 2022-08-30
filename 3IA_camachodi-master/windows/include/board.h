#ifndef __BOARD_H__
#define __BOARD_H__ 1

#include "common_def.h"
#include <SFML/Graphics.hpp>

const int kBoardMaxAgents = 5;
const int kNPatrols = 4;

enum MoveType {
	kMoveType_NONE = 0,
	kMoveType_RANDOM,
	kMoveType_PATROL,
	kMoveType_PACMAN,
};

enum Commands {
	kCommands_NONE = 0,
	kCommands_NORTH,
	kCommands_SOUTH,
	kCommands_EAST,
	kCommands_WEST,
};

enum TileType {

	kTileType_NON_WALKABLE = 0,
	kTileType_WALL,
	kTileType_WALKABLE,
	kTileType_DOOR,

};

struct Patrol {
	int size;
	Commands* commandArray;
};

struct Cell {

	int type;

	float posX;
	float posY;

	Cell::Cell() {

		type = 0;
		posX = 0.0f;
		posY = 0.0f;

	}

};

struct Agent {

	int id;

	int lastCell;
	int nextCell;

	float posX;
	float posY;

	int step;
	int patrolID;

	int targetx;
	int targety;

	MoveType mType;

	Agent::Agent() {

		id = 0;

		lastCell = 0;
		nextCell = 0;

		posX = 0.0f;
		posY = 0.0f;

		targetx = 0;
		targety = 0;

		step = 0;
		patrolID = 0;

		mType = kMoveType_RANDOM;

	}

};

class Board {

public:
	Board();
	~Board();

	void initData();

	void initImgData(const char* filename);

	Cell& cell(int row, int col);

	int north(int id);
	int east(int id);
	int south(int id);
	int west(int id);

	bool isWall(int id);

	void idToRowCol(int* row, int* col, int id);
	
	int findAgent(int agent_id);

	void movementType();
	void placeAgent(int agent_id, int dest_id);
	void randomMovement(int id);
	void patrolMovement(int id);
	void pacmanMovement(int id);

	int walkableTile();
	void agentSpriteLoad();
	void drawAgents(sf::RenderWindow* window);
	void loadBoard();
	void drawBoard(sf::RenderWindow* window);
	int manhattanDistance(int originx, int originy, int destx, int desty);

	int boardWidth;
	int boardHeight;

	Patrol pat[kNPatrols];

	Cell* cells;
	Agent* agentss;

	sf::RectangleShape* boardRects;

	sf::Sprite* agentSprite;
	sf::Texture* agentTexture;

	sf::Image boardImg;

	sf::Texture falseBoardImg;
	sf::Sprite boardTexture;

	float cellSize = 16.0f;

	int pixelColor;
	
	//bool checkAgentMovement(int agent_id, int origin_id, int dest_id);

	//void killUnit(int agent_id);

	//int euclideanDistance(int origin_id, int dest_id);

	//bool isDoor(int id);

};

#endif