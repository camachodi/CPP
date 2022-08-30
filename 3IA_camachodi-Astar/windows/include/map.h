#ifndef __MAP_H__
#define __MAP_H__ 1

#include "common_def.h"
#include <SFML/Graphics.hpp>

struct Tile
{

	int id_;
	int parentID_;

	bool walkable_;
	bool occupied_;

	float posX_;
	float posY_;

	bool change_;

	bool isLimitWall_;

	float f_;
	float g_;
	float h_;

	sf::RectangleShape tileRect_;


};

struct Agent {

	int id_;
	float posX_;
	float posY_;
	sf::RectangleShape agentRect_;

};

struct Goal {

	int id_;
	float posX_;
	float posY_;
	sf::RectangleShape goalRect_;

};

class Map {

public:

	Map();
	~Map();

	int mapWidth_ = 52;
	int mapHeight_ = 52;
	float tileSize_ = 14.0f;
	bool agentChangeDone_ = true;
	bool goalChangeDone_ = true;
	bool seekDone_ = true;

	Tile* tile_;
	Agent* agent_;
	Goal* goal_;

	std::vector<Tile> OpenList;
	std::vector<Tile> ClosedList;

	void InitElementsData();
	void LoadElementsData();
	void ReplaceAgent(sf::RenderWindow* window);
	void ReplaceGoal(sf::RenderWindow* window);
	void DrawElements(sf::RenderWindow* window);
	void ChangeTile(sf::RenderWindow* window);
	float CalculateDistance(int id);
	void StartSeek();

};

#endif