#include "map.h"
#include "SFML/Graphics.hpp"
#include <stdlib.h>
#include "stdio.h"

Map::Map() {

	tile_ = nullptr;
	agent_ = nullptr;
	goal_ = nullptr;

}

Map::~Map(){

	delete[] tile_;
	delete[] agent_;
	delete[] goal_;

}

void Map::InitElementsData()
{

	tile_ = new Tile[mapWidth_ * mapHeight_];
	agent_ = new Agent[1];
	goal_ = new Goal[1];

}

void Map::LoadElementsData()
{

	for (int i = 0; i < mapHeight_; i++) {

		for (int j = 0; j < mapWidth_; j++) {

			tile_[(i * mapWidth_) + j].id_ = (i * mapWidth_) + j;
			tile_[(i * mapWidth_) + j].tileRect_.setPosition(sf::Vector2f(j * (tileSize_ + 4.0f), i * (tileSize_ + 4.0f)));
			tile_[(i * mapWidth_) + j].tileRect_.setSize(sf::Vector2f(tileSize_, tileSize_));
			tile_[(i * mapWidth_) + j].posX_ = tile_[(i * mapWidth_) + j].tileRect_.getPosition().x;
			tile_[(i * mapWidth_) + j].posY_ = tile_[(i * mapWidth_) + j].tileRect_.getPosition().y;
			tile_[(i * mapWidth_) + j].walkable_ = true;
			tile_[(i * mapWidth_) + j].occupied_ = false;
			tile_[(i * mapWidth_) + j].change_ = false;
			tile_[(i * mapWidth_) + j].parentID_ = 0;
			tile_[(i * mapWidth_) + j].f_ = 0.0f;
			tile_[(i * mapWidth_) + j].g_ = 0.0f;
			tile_[(i * mapWidth_) + j].h_ = 0.0f;

			if (i == 0 || i == mapHeight_ - 1 || j == 0 || j  == mapWidth_ - 1)
			{

				tile_[(i * mapWidth_) + j].tileRect_.setFillColor(sf::Color::Magenta);
				tile_[(i * mapWidth_) + j].isLimitWall_ = true;

			}
			else
			{

				tile_[(i * mapWidth_) + j].tileRect_.setFillColor(sf::Color::White);
				tile_[(i * mapWidth_) + j].isLimitWall_ = false;

			}

		}

	}

	agent_[0].id_ = tile_[53].id_;
	agent_[0].posX_ = tile_[53].posX_;
	agent_[0].posY_ = tile_[53].posY_;
	agent_[0].agentRect_.setPosition(agent_[0].posX_ - 2.0f, agent_[0].posY_ - 2.0f);
	agent_[0].agentRect_.setSize(sf::Vector2f(tileSize_ + 4.0f, tileSize_ + 4.0f));
	agent_[0].agentRect_.setFillColor(sf::Color::Green);
	tile_[53].occupied_ = true;

	goal_[0].id_ = tile_[(mapWidth_ * mapHeight_) - 54].id_;
	goal_[0].posX_ = tile_[(mapWidth_ * mapHeight_) - 54].posX_;
	goal_[0].posY_ = tile_[(mapWidth_ * mapHeight_) - 54].posY_;
	goal_[0].goalRect_.setPosition(goal_[0].posX_ - 2.0f, goal_[0].posY_ - 2.0f);
	goal_[0].goalRect_.setSize(sf::Vector2f(tileSize_ + 4.0f, tileSize_ + 4.0f));
	goal_[0].goalRect_.setFillColor(sf::Color::Red);
	tile_[(mapWidth_ * mapHeight_) - 54].occupied_ = true;

}

void Map::ReplaceAgent(sf::RenderWindow* window)
{

	if (!agentChangeDone_)
	{

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{

			int auxTileA = 0;
			int auxTileB = 0;

			for (int i = 0; i < mapWidth_ * mapHeight_; i++)
			{

				if (tile_[i].tileRect_.getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
				{

					if (!tile_[i].occupied_ && tile_[i].walkable_ && !tile_[i].isLimitWall_)
					{

						agent_[0].posX_ = tile_[i].tileRect_.getPosition().x;
						agent_[0].posY_ = tile_[i].tileRect_.getPosition().y;
						agent_[0].id_ = tile_[i].id_;

						agent_[0].agentRect_.setPosition(agent_[0].posX_ - 2.0f, agent_[0].posY_ - 2.0f);

						for (int i = 0; i < mapWidth_ * mapHeight_; i++)
						{

							if (agent_[0].id_ == i)
							{

								auxTileA = i;

							}
							else
							{

								tile_[i].occupied_ = false;

							}

							if (goal_[0].id_ == i)
							{

								auxTileB = i;

							}
							else
							{

								tile_[i].occupied_ = false;

							}


						}
						
						tile_[auxTileA].occupied_ = true;
						tile_[auxTileB].occupied_ = true;

						agentChangeDone_ = true;

					}

				}

			}

		}

	}

}

void Map::ReplaceGoal(sf::RenderWindow* window)
{

	if (!goalChangeDone_)
	{

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{

			int auxTileA = 0;
			int auxTileB = 0;

			for (int i = 0; i < mapWidth_ * mapHeight_; i++)
			{

				if (tile_[i].tileRect_.getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
				{

					if (!tile_[i].occupied_ && tile_[i].walkable_ && !tile_[i].isLimitWall_)
					{

						goal_[0].posX_ = tile_[i].tileRect_.getPosition().x;
						goal_[0].posY_ = tile_[i].tileRect_.getPosition().y;
						goal_[0].id_ = tile_[i].id_;

						goal_[0].goalRect_.setPosition(goal_[0].posX_ - 2.0f, goal_[0].posY_ - 2.0f);

						for (int i = 0; i < mapWidth_ * mapHeight_; i++)
						{

							if (agent_[0].id_ == i)
							{

								auxTileA = i;

							}
							else
							{

								tile_[i].occupied_ = false;

							}

							if (goal_[0].id_ == i)
							{

								auxTileB = i;

							}
							else
							{

								tile_[i].occupied_ = false;

							}


						}

						tile_[auxTileA].occupied_ = true;
						tile_[auxTileB].occupied_ = true;

						goalChangeDone_ = true;

					}

				}

			}

		}

	}

}
//ARREGLAR ESTO
float Map::CalculateDistance(int id)
{
	return abs(sqrt(pow(tile_[goal_[0].id_].posX_ - tile_[id].posX_, 2) + pow(tile_[goal_[0].id_].posY_ - tile_[id].posY_, 2) * 1.0f));

}

void Map::StartSeek()
{

	if (!seekDone_)
	{

		int q = 0;
		int qToDelete = 0;

		tile_[agent_[0].id_].g_ = 0.0f;

		tile_[agent_[0].id_].h_ = CalculateDistance(tile_[agent_[0].id_].id_);

		tile_[agent_[0].id_].f_ = tile_[agent_[0].id_].g_ + tile_[agent_[0].id_].h_;

		OpenList.push_back(tile_[agent_[0].id_]);

		while (!OpenList.empty())
		{

			float lowestF = 1000000.0f;

			for (int i = 0; i < OpenList.size(); i++)
			{

				if (OpenList[i].f_ < lowestF)
				{

					lowestF = OpenList[i].f_;

					q = OpenList[i].id_;
					qToDelete = i;

				}

			}

			OpenList.erase(OpenList.begin() + qToDelete);

			int Up_ = q - mapHeight_;
			int Right_ = q + 1;
			int Down_ = q + mapHeight_;
			int Left_ = q - 1;

			std::vector<Tile> succesors;

			tile_[Up_].parentID_ = tile_[q].id_;
			tile_[Right_].parentID_ = tile_[q].id_;
			tile_[Down_].parentID_ = tile_[q].id_;
			tile_[Left_].parentID_ = tile_[q].id_;

			succesors.push_back(tile_[Up_]);
			succesors.push_back(tile_[Right_]);
			succesors.push_back(tile_[Down_]);
			succesors.push_back(tile_[Left_]);

			for (int i = 0; i < succesors.size(); i++)
			{
				if (!succesors[i].isLimitWall_ && succesors[i].walkable_)
				{

					if (succesors[i].id_ == tile_[goal_[0].id_].id_)
					{

						OpenList.clear();

						break;

					}
					else
					{

						succesors[i].g_ = tile_[succesors[i].parentID_].g_ + 1.0f;

						succesors[i].h_ = CalculateDistance(tile_[succesors[i].id_].id_);

						succesors[i].f_ = succesors[i].g_ + succesors[i].h_;


					}

					bool skip = false;

					for (int j = 0; j < OpenList.size(); j++)
					{

						if (succesors[i].id_ == OpenList[j].id_)
						{

							if (OpenList[j].f_ < succesors[i].f_)
							{

								skip = true;

							}

						}

					}

					for (int k = 0; k < ClosedList.size(); k++)
					{

						if (succesors[i].id_ == ClosedList[k].id_)
						{

							if (ClosedList[k].f_ < succesors[i].f_)
							{

								skip = true;

							}

						}

					}

					if (!skip)
					{

						OpenList.push_back(tile_[succesors[i].id_]);


					}

				}

			}

			ClosedList.push_back(tile_[q]);



		}

		for (int i = 0; i < ClosedList.size(); i++)
		{
		
			tile_[ClosedList[i].id_].tileRect_.setFillColor(sf::Color::Cyan);
		
		}

		seekDone_ = true;

	}



	//int firstNode = tile_[agent_[0].id_].id_;
	//int actualNode = ClosedList[ClosedList.size() - 1].id_;
	//
	//while (actualNode != firstNode)
	//{
	//
	//	tile_[actualNode].tileRect_.setFillColor(sf::Color::Yellow);
	//	actualNode = tile_[actualNode].parentID_;
	//
	//}

}

void Map::DrawElements(sf::RenderWindow* window)
{

	for (int i = 0; i < mapWidth_ * mapHeight_; i++) {

		window->draw(tile_[i].tileRect_);

	}


	window->draw(agent_[0].agentRect_);
	window->draw(goal_[0].goalRect_);

}

void Map::ChangeTile(sf::RenderWindow* window)
{
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{

		for (int i = 0; i < mapWidth_ * mapHeight_; i++)
		{

			if (tile_[i].tileRect_.getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
			{

				if (tile_[i].change_ == false && !tile_[i].occupied_ && !tile_[i].isLimitWall_)
				{

					tile_[i].walkable_ = !tile_[i].walkable_;
					tile_[i].change_ = true;

				}

			}
			else
			{

				tile_[i].change_ = false;

			}

			if (tile_[i].walkable_ && !tile_[i].isLimitWall_)
			{

				tile_[i].tileRect_.setFillColor(sf::Color::White);

			}
			else if (!tile_[i].walkable_ && !tile_[i].isLimitWall_)
			{

				tile_[i].tileRect_.setFillColor(sf::Color::Blue);

			}

		}

	}

}