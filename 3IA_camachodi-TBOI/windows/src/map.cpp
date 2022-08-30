#include "map.h"
#include "SFML/Graphics.hpp"
#include <stdlib.h>

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

			tile_[(i * mapWidth_) + j].tileRect_.setPosition(sf::Vector2f(j * (tileSize_ + 4.0f), i * (tileSize_ + 4.0f)));
			tile_[(i * mapWidth_) + j].tileRect_.setSize(sf::Vector2f(tileSize_, tileSize_));
			tile_[(i * mapWidth_) + j].posX_ = tile_[(i * mapWidth_) + j].tileRect_.getPosition().x;
			tile_[(i * mapWidth_) + j].posY_ = tile_[(i * mapWidth_) + j].tileRect_.getPosition().y;
			tile_[(i * mapWidth_) + j].walkable_ = true;
			tile_[(i * mapWidth_) + j].occupied_ = false;
			tile_[(i * mapWidth_) + j].change_ = false;
			tile_[(i * mapWidth_) + j].tileRect_.setFillColor(sf::Color::White);

		}

	}

	agent_[0].posX_ = tile_[0].posX_;
	agent_[0].posY_ = tile_[0].posY_;
	agent_[0].agentRect_.setPosition(agent_[0].posX_ - 2.0f, agent_[0].posY_ - 2.0f);
	agent_[0].agentRect_.setSize(sf::Vector2f(tileSize_ + 4.0f, tileSize_ + 4.0f));
	agent_[0].agentRect_.setFillColor(sf::Color::Green);
	tile_[0].occupied_ = true;

	goal_[0].posX_ = tile_[(mapWidth_ * mapHeight_) - 1].posX_;
	goal_[0].posY_ = tile_[(mapWidth_ * mapHeight_) - 1].posY_;
	goal_[0].goalRect_.setPosition(goal_[0].posX_ - 2.0f, goal_[0].posY_ - 2.0f);
	goal_[0].goalRect_.setSize(sf::Vector2f(tileSize_ + 4.0f, tileSize_ + 4.0f));
	goal_[0].goalRect_.setFillColor(sf::Color::Red);
	tile_[(mapWidth_ * mapHeight_) - 1].occupied_ = true;

}

void Map::ReplaceAgent(sf::RenderWindow* window)
{

	if (!agentChangeDone_)
	{

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{

			float auxX = 0.0f;
			float auxY = 0.0f;

			int auxTileA = 0;
			int auxTileB = 0;

			for (int i = 0; i < mapWidth_ * mapHeight_; i++)
			{

				if (tile_[i].tileRect_.getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
				{

					if (!tile_[i].occupied_ && tile_[i].walkable_)
					{

						auxX = tile_[i].tileRect_.getPosition().x;
						auxY = tile_[i].tileRect_.getPosition().y;

						agent_[0].posX_ = auxX;
						agent_[0].posY_ = auxY;
						agent_[0].agentRect_.setPosition(agent_[0].posX_ - 2.0f, agent_[0].posY_ - 2.0f);

						for (int i = 0; i < mapWidth_ * mapHeight_; i++)
						{

							if (agent_[0].posX_ == tile_[i].posX_ && agent_[0].posY_ == tile_[i].posY_)
							{

								auxTileA = i;

							}
							else
							{

								tile_[i].occupied_ = false;

							}

							if (goal_[0].posX_ == tile_[i].posX_ && goal_[0].posY_ == tile_[i].posY_)
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

			float auxX = 0.0f;
			float auxY = 0.0f;

			int auxTileA = 0;
			int auxTileB = 0;

			for (int i = 0; i < mapWidth_ * mapHeight_; i++)
			{

				if (tile_[i].tileRect_.getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
				{

					if (!tile_[i].occupied_ && tile_[i].walkable_)
					{

						auxX = tile_[i].tileRect_.getPosition().x;
						auxY = tile_[i].tileRect_.getPosition().y;

						goal_[0].posX_ = auxX;
						goal_[0].posY_ = auxY;
						goal_[0].goalRect_.setPosition(goal_[0].posX_ - 2.0f, goal_[0].posY_ - 2.0f);

						for (int i = 0; i < mapWidth_ * mapHeight_; i++)
						{

							if (agent_[0].posX_ == tile_[i].posX_ && agent_[0].posY_ == tile_[i].posY_)
							{

								auxTileA = i;

							}
							else
							{

								tile_[i].occupied_ = false;

							}

							if (goal_[0].posX_ == tile_[i].posX_ && goal_[0].posY_ == tile_[i].posY_)
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

float Map::CalculateDistance(float initX, float initY, float endX, float endY)
{

	return abs(sqrt(pow(endX - initX, 2) + pow(endY - initY, 2) * 1.0f));

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

				if (tile_[i].change_ == false && !tile_[i].occupied_)
				{

					tile_[i].walkable_ = !tile_[i].walkable_;
					tile_[i].change_ = true;

				}

			}
			else
			{

				tile_[i].change_ = false;

			}

			if (tile_[i].walkable_)
			{

				tile_[i].tileRect_.setFillColor(sf::Color::White);

			}
			else
			{

				tile_[i].tileRect_.setFillColor(sf::Color::Blue);

			}

		}

	}

}