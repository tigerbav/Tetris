#pragma once
#include "BaseApp.h"
class GameObject : public BaseApp
{
	int figure[4][4];

	int coordLTX;
	int coordLTY;
	int oldCoordLTX;
	int rotate;
	bool rotateStep;
public:
	bool stopStep;
	typedef BaseApp Parent;
	GameObject();
	GameObject(int, int, int, int, int, int, int, int, int);
	virtual void UpdateF(float deltaTime);
	virtual void KeyPressed(int btnCode);
	void draw();
	int checkLine();
	void deleteComponent(int);
	bool beforeDelete(int);
};