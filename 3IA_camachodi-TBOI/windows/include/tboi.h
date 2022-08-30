#ifndef __TBOI_H__
#define __TBOI_H__ 1

enum RoomType
{

	KRoomType_Void = 0,
	KRoomType_Start,
	KRoomType_Normal,
	KRoomType_Treasure,
	KRoomType_Boss,

};

enum Quad
{

	KQuad_Axis = 0,
	KQuad_First,
	KQuad_Second,
	KQuad_Third,
	KQuad_Fourth,

};

struct Room
{

	int id_;
	int type_;
	char icon_;
	float posX_;
	float posY_;
	int quad_;
	int spawnHelp;

};

class TBOI {

public:

	TBOI();
	~TBOI();

	int centerID = 0;
	int mapSize_ = 110;
	int mapWidth_;
	int mapHeight_;
	char RoomIcon[5] = { ' ', 'S', '0', 'T', 'B' };
	Room* room_;

	void initData();
	void calculateStart();
	void calculateQuad();
	float CalculateDistance(float initX, float initY, float endX, float endY);
	void calculateRoomSpawn();
	void cleanDeadRooms();
	void setIcon();
	void calculateData();
	void drawData();

};

#endif