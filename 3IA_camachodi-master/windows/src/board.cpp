#include "board.h"
#include "SFML/Graphics.hpp"
#include <stdlib.h>

Board::Board() {
	cells = nullptr;
	agentss = nullptr;
	boardRects = nullptr;
	agentSprite = nullptr;
	agentTexture = nullptr;

	boardWidth = 0;
	boardHeight = 0;
	pixelColor = 0;
}

Board::~Board(){

	delete[] agentss;
	delete[] agentSprite;
	delete[] agentTexture;
	delete[] cells;
	delete[] boardRects;

}

void Board::initData() {

	cells = new Cell[boardWidth * boardHeight];
	agentss = new Agent[kBoardMaxAgents];
	boardRects = new sf::RectangleShape[boardWidth * boardHeight];
	agentTexture = new sf::Texture[kBoardMaxAgents];
	agentSprite = new sf::Sprite[kBoardMaxAgents];


	//Init patrol
	pat[0].size = 7;
	pat[1].size = 8;
	pat[2].size = 9;
	pat[3].size = 10;

	for (int i = 0; i < kNPatrols; i ++)
	{
		pat[i].commandArray = (Commands*)malloc(sizeof(Commands) * pat[i].size);
	}

	pat[0].commandArray[0] = kCommands_NORTH;
	pat[0].commandArray[1] = kCommands_SOUTH;
	pat[0].commandArray[2] = kCommands_NORTH;
	pat[0].commandArray[3] = kCommands_EAST;
	pat[0].commandArray[4] = kCommands_SOUTH;
	pat[0].commandArray[5] = kCommands_WEST;
	pat[0].commandArray[6] = kCommands_NORTH;

	pat[1].commandArray[0] = kCommands_NORTH;
	pat[1].commandArray[1] = kCommands_WEST;
	pat[1].commandArray[2] = kCommands_EAST;
	pat[1].commandArray[3] = kCommands_NORTH;
	pat[1].commandArray[4] = kCommands_SOUTH;
	pat[1].commandArray[5] = kCommands_EAST;
	pat[1].commandArray[6] = kCommands_SOUTH;
	pat[1].commandArray[7] = kCommands_EAST;

	pat[2].commandArray[0] = kCommands_NORTH;
	pat[2].commandArray[1] = kCommands_EAST;
	pat[2].commandArray[2] = kCommands_SOUTH;
	pat[2].commandArray[3] = kCommands_WEST;
	pat[2].commandArray[4] = kCommands_SOUTH;
	pat[2].commandArray[5] = kCommands_NORTH;
	pat[2].commandArray[6] = kCommands_SOUTH;
	pat[2].commandArray[7] = kCommands_EAST;
	pat[2].commandArray[8] = kCommands_NORTH;

	pat[3].commandArray[0] = kCommands_NORTH;
	pat[3].commandArray[1] = kCommands_EAST;
	pat[3].commandArray[2] = kCommands_WEST;
	pat[3].commandArray[3] = kCommands_NORTH;
	pat[3].commandArray[4] = kCommands_SOUTH;
	pat[3].commandArray[5] = kCommands_EAST;
	pat[3].commandArray[6] = kCommands_NORTH;
	pat[3].commandArray[7] = kCommands_EAST;
	pat[3].commandArray[8] = kCommands_NORTH;
	pat[3].commandArray[9] = kCommands_EAST;


}

void Board::agentSpriteLoad() {

	agentTexture[0].loadFromFile("../../data/gfx/agents/allied_engineer.bmp");
	agentTexture[1].loadFromFile("../../data/gfx/agents/allied_flag.bmp");
	agentTexture[2].loadFromFile("../../data/gfx/agents/allied_medic.bmp");
	agentTexture[3].loadFromFile("../../data/gfx/agents/allied_soldier.bmp");
	agentTexture[4].loadFromFile("../../data/gfx/agents/axis_engineer.bmp");


	for (int i = 0; i < 5; i++)
	{
		agentSprite[i].setTexture(agentTexture[i]);
		agentSprite[i].setScale(1.0f, 1.0f);
	}
}

void Board::drawAgents(sf::RenderWindow* window) {
	for (int i = 0; i < kBoardMaxAgents; i++) {

		int agentPos = findAgent(i);
		int row, col;
		idToRowCol(&row, &col, agentPos);

		agentSprite[i].setPosition(sf::Vector2f(col * cellSize, row * cellSize));
		window->draw(agentSprite[i]);
		
	}
}

void Board::drawBoard(sf::RenderWindow* window) {
	for (int i = 0; i < boardWidth * boardHeight; i++) {
		window->draw(boardRects[i]);
	}
}

void Board::loadBoard() {
	for (int i = 0; i < boardHeight; i++) {
		for (int j = 0; j < boardWidth; j++) {

			boardRects[(i * boardWidth) + j].setPosition(sf::Vector2f(j * cellSize, i * cellSize));
			boardRects[(i * boardWidth) + j].setSize(sf::Vector2f(cellSize, cellSize));

			if (cell(i, j).type == kTileType_WALL) {
				boardRects[(i * boardWidth) + j].setFillColor(sf::Color::Black);
			}
			else {
				boardRects[(i * boardWidth) + j].setFillColor(sf::Color::Transparent);
			}
		}
	}

	for (int i = 0; i < boardWidth * boardHeight; i++)
	{
		if (cells[i].type != kTileType_WALL)
		{
			cells[i].posX = i % boardWidth;
			cells[i].posY = i / boardWidth;
		}
	}
}

void Board::initImgData(const char* filename)
{
	boardImg.loadFromFile(filename);

	boardWidth = boardImg.getSize().x;
	boardHeight = boardImg.getSize().y;

	initData();

	for (int i = 0; i < boardHeight; i++) {
		for (int j = 0; j < boardWidth; j++) {
			pixelColor = boardImg.getPixel(j, i).toInteger();
			if (pixelColor == 0xFFFFFFFF) {
				cell(i, j).type = kTileType_WALKABLE;
			}
			else {
				if (pixelColor == 0x000000FF) {
					cell(i, j).type = kTileType_WALL;
				}
			}
		}
	}

	for (int i = 0; i < kBoardMaxAgents; i++) {

		agentss[i].id = walkableTile();
		agentss[i].patrolID = i % kNPatrols;

	}
}

Cell& Board::cell(int row, int col)
{
	return *(cells + (row * boardWidth + col));
}

int Board::north(int id)
{
	if (id - boardWidth >= 0) {
		return id - boardWidth;
	}
	else {
		return id;
	}
}

int Board::east(int id)
{
	if (!((id + 1) % boardWidth == 0)) {
		return id + 1;
	}
	else {
		return id;
	}
}

int Board::south(int id)
{
	if ((id + boardWidth) >= (boardWidth * boardHeight)) {
		return id;
	}
	else {
		return id + boardWidth;
	}
}

int Board::west(int id)
{
	if (id % boardWidth == 0) {
		return id;
	}
	else {
		return id - 1;
	}
}

bool Board::isWall(int id)
{
	return cells[id].type == kTileType_WALL;
}

void Board::idToRowCol(int* row, int* col, int id)
{
	*row = id / boardWidth;
	*col = id % boardWidth;
}

int Board::findAgent(int agent_id)
{
	return agentss[agent_id].id;
}

void Board::placeAgent(int agent_id, int dest_id)
{
	if (cells[dest_id].type != kTileType_WALL)
	{
		agentss[agent_id].id = dest_id;

		agentss[agent_id].posX = cells[dest_id].posX;
		agentss[agent_id].posY = cells[dest_id].posY;

		printf("%f\n", agentss[agent_id].posX);
		printf("%f\n\n\n", agentss[agent_id].posY);
	}
}

void Board::movementType()
{
	for (int i = 0; i < kBoardMaxAgents; i++)
	{
		switch (agentss[i].mType)
		{
			case kMoveType_RANDOM:
				randomMovement(i);
				break;

			case kMoveType_PATROL:
				patrolMovement(i);
				break;

			case kMoveType_PACMAN:
				pacmanMovement(i);
				break;
		}
	}
}

void Board::randomMovement(int id) 
{
	int nextCell = rand() % 4;

	switch (nextCell) {

	case 0:
		nextCell = north(findAgent(id));
		break;
	case 1:
		nextCell = west(findAgent(id));
		break;
	case 2:
		nextCell = south(findAgent(id));
		break;
	case 3:
		nextCell = east(findAgent(id));
		break;

	}
	placeAgent(id, nextCell);
}

void Board::patrolMovement(int id)
{
	int nextCell;

	switch (pat[agentss[id].patrolID].commandArray[agentss[id].step]) {

	case kCommands_NORTH:
		nextCell = north(findAgent(id));
		break;
	case kCommands_SOUTH:
		nextCell = south(findAgent(id));
		break;
	case kCommands_EAST:
		nextCell = east(findAgent(id));
		break;
	case kCommands_WEST:
		nextCell = west(findAgent(id));
		break;
	default: printf("Invalid Command");

	}

	agentss[id].step++;

	if (agentss[id].step >= pat[agentss[id].patrolID].size)
	{
		agentss[id].step = 0;
	}

	placeAgent(id, nextCell);
}

void Board::pacmanMovement(int id)
{
	int distance = 1000000;
	int aux = 0;

	int auxrow = 0;
	int auxcol = 0;

	int nextCell = 0;

	aux = north(findAgent(id));
	idToRowCol(&auxrow, &auxcol, aux);

	if (!isWall(aux) && manhattanDistance(auxrow, auxcol, agentss[id].targetx, agentss[id].targety) < distance)
	{
		distance = manhattanDistance(auxrow, auxcol, agentss[id].targetx, agentss[id].targety);
		nextCell = aux;
	}

	aux = south(findAgent(id));
	idToRowCol(&auxrow, &auxcol, aux);

	if (!isWall(aux) && manhattanDistance(auxrow, auxcol, agentss[id].targetx, agentss[id].targety) < distance)
	{
		distance = manhattanDistance(auxrow, auxcol, agentss[id].targetx, agentss[id].targety);
		nextCell = aux;
	}

	aux = east(findAgent(id));
	idToRowCol(&auxrow, &auxcol, aux);

	if (!isWall(aux) && manhattanDistance(auxrow, auxcol, agentss[id].targetx, agentss[id].targety) < distance)
	{
		distance = manhattanDistance(auxrow, auxcol, agentss[id].targetx, agentss[id].targety);
		nextCell = aux;
	}

	aux = west(findAgent(id));
	idToRowCol(&auxrow, &auxcol, aux);

	if (!isWall(aux) && manhattanDistance(auxrow, auxcol, agentss[id].targetx, agentss[id].targety) < distance)
	{
		distance = manhattanDistance(auxrow, auxcol, agentss[id].targetx, agentss[id].targety);
		nextCell = aux;
	}

	placeAgent(id, nextCell);
}

int Board::walkableTile()
{
	int i;

	i = rand() % (boardWidth * boardHeight);

	while ((cells[i].type != kTileType_WALKABLE))
	{
		i = rand() % (boardWidth * boardHeight);
	}

	return i;
}

int Board::manhattanDistance(int originx, int originy, int destx, int desty)
{

	return abs(originx - destx) + abs(originy - desty);

}

/*void Board::killUnit(int agent_id)
{

}*/

/*bool Board::checkAgentMovement(int agent_id, int origin_id, int dest_id)
{
	return false;
}*/

/*bool Board::isDoor(int id)
{
	return false;
}*/