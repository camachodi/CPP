#include "tboi.h"
#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include <time.h>

TBOI::TBOI()
{

	room_ = nullptr;

}

TBOI::~TBOI()
{

	delete[] room_;

}

void TBOI::initData()
{

	srand(time(NULL));

	if (mapSize_%2 != 1)
	{
		mapSize_++;
	}

	mapWidth_ = mapSize_;
	mapHeight_ = mapSize_;
	room_ = new Room[mapWidth_ * mapHeight_];

	for (int i = 0; i < mapHeight_; i++)
	{

		for (int j = 0; j < mapWidth_; j++)
		{

			room_[i * mapWidth_ + j].id_ = (i * mapWidth_ + j);
			room_[i * mapWidth_ + j].type_ = KRoomType_Void;
			room_[i * mapWidth_ + j].posX_ = j;
			room_[i * mapWidth_ + j].posY_ = i;
			room_[i * mapWidth_ + j].spawnHelp = rand()%3;

		}

	}

}

void TBOI::calculateStart()
{

	for (int i = 0; i < mapHeight_; i++)
	{

		for (int j = 0; j < mapWidth_; j++)
		{

			if ((i * mapWidth_ + j) == (mapWidth_ * mapHeight_) / 2)
			{

				room_[i * mapWidth_ + j].type_ = KRoomType_Start;

			}

		}

	}

}

void TBOI::calculateQuad()
{

	int auxX = 0;
	int auxY = 0;

	for (int i = 0; i < mapHeight_ * mapWidth_; i++)
	{

		if (room_[i].type_ == KRoomType_Start)
		{

			auxX = room_[i].posX_;
			auxY = room_[i].posY_;
			centerID = room_[i].id_;

		}

	}

	for (int i = 0; i < mapHeight_ * mapWidth_; i++)
	{

		if (room_[i].posX_ == auxX || room_[i].posY_ == auxY)
		{

			room_[i].quad_ = KQuad_Axis;

		}
		else if (room_[i].posX_ < auxX && room_[i].posY_ > auxY)
		{

			room_[i].quad_ = KQuad_First;

		}
		else if (room_[i].posX_ > auxX && room_[i].posY_ > auxY)
		{

			room_[i].quad_ = KQuad_Second;

		}
		else if (room_[i].posX_ < auxX && room_[i].posY_ < auxY)
		{

			room_[i].quad_ = KQuad_Third;

		}
		else if (room_[i].posX_ > auxX && room_[i].posY_ < auxY)
		{

			room_[i].quad_ = KQuad_Fourth;

		}

	}

}

float TBOI::CalculateDistance(float initX, float initY, float endX, float endY)
{

	return abs(sqrt(pow(endX - initX, 2) + pow(endY - initY, 2) * 1.0f));

}

void TBOI::calculateRoomSpawn()
{

	srand(time(NULL));

	float spawnRatePerQuad = 100 / (mapSize_ / 2);
	float spawnRate = 0;
	float distanceToRoom = 0;
	int spawnRateGenerated = 0;

	for (int i = 0; i < mapWidth_ * mapHeight_; i++)
	{
	
		distanceToRoom = CalculateDistance(room_[centerID].posX_, room_[centerID].posY_, room_[i].posX_, room_[i].posY_);
	
		spawnRate = 100 - (spawnRatePerQuad * distanceToRoom);
		if (room_[i].type_ != KRoomType_Start)
		{

			if (room_[i].spawnHelp == 0)
			{

				spawnRate -= 75.0f;

			}
			else if (room_[i].spawnHelp == 1)
			{

				spawnRate -= 15.0f;

			}
			else if (room_[i].spawnHelp == 2)
			{

				spawnRate += 15.0f;

			}

		}

		spawnRateGenerated = rand() % 100 + 1;

		if (spawnRateGenerated < spawnRate && room_[i].type_ != KRoomType_Start)
		{

			room_[i].type_ = KRoomType_Normal;

		}
		else if (spawnRateGenerated > spawnRate && room_[i].type_ != KRoomType_Start)
		{

			room_[i].type_ = KRoomType_Void;

		}

	}

	for (int i = 0; i < mapHeight_; i++)
	{

		for (int j = 0; j < mapWidth_; j++)
		{

			if (i == 0 || i == mapHeight_ - 1 || j == 0 || j == mapWidth_ - 1)
			{

				room_[i * mapWidth_ + j].type_ = KRoomType_Void;

			}

		}

	}

}

void TBOI::cleanDeadRooms()
{

	for (int i = 0; i < mapWidth_ * mapHeight_; i++)
	{

		if (room_[i].type_ != KRoomType_Void && room_[i].type_ != KRoomType_Start)
		{
			
			if (room_[i + 1].type_ == KRoomType_Void && room_[i + mapHeight_].type_ == KRoomType_Void && room_[i - 1].type_ == KRoomType_Void && room_[i - mapHeight_].type_ == KRoomType_Void)
			{

				room_[i].type_ = KRoomType_Void;

			}

		}

	}

}

void TBOI::setIcon()
{

	for (int i = 0; i < mapWidth_ * mapHeight_; i++)
	{

		if (room_[i].type_ == KRoomType_Void)
		{

			room_[i].icon_ = RoomIcon[0];

		}
		else if (room_[i].type_ == KRoomType_Start)
		{

			room_[i].icon_ = RoomIcon[1];

		}
		else if (room_[i].type_ == KRoomType_Normal)
		{

			room_[i].icon_ = RoomIcon[2];

		}
		else if (room_[i].type_ == KRoomType_Treasure)
		{

			room_[i].icon_ = RoomIcon[3];

		}
		else if (room_[i].type_ == KRoomType_Boss)
		{

			room_[i].icon_ = RoomIcon[4];

		}

	}

}

void TBOI::calculateData()
{

	calculateStart();
	calculateQuad();
	calculateRoomSpawn();
	cleanDeadRooms();
	setIcon();

}

void TBOI::drawData()
{

	for (int i = 0; i < mapHeight_; i++)
	{
		
		for (int j = 0; j < mapWidth_; j++)
		{

			//printf("%d ", room_[i * mapWidth_ + j].spawnHelp);
			printf("%c ", room_[i * mapWidth_ + j].icon_);
			//printf("%d,%d ", (int)room_[i * mapWidth_ + j].posX_, (int)room_[i * mapWidth_ + j].posY_);

		}

		printf("\n");

	}

}

