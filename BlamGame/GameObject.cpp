#include "GameObject.h"

int counter = 0, flag = 0;
int tempFigure[4][4];
int figureSize = 4;
int field[20][15];
int counterForDelete = 0;
GameObject::GameObject()
{

}
GameObject::GameObject(int f1, int f2, int f3, int f4, int f5, int f6, int f7, int f8, int rotate_) : Parent(30, 30) 
{
	figure[0][0] = f1;
	figure[0][1] = f2;
	figure[1][0] = f3;
	figure[1][1] = f4;
	figure[2][0] = f5;
	figure[2][1] = f6;
	figure[3][0] = f7;
	figure[3][1] = f8; 


	rotate = rotate_;
	rotateStep = false;
	stopStep = false;
	oldCoordLTX = coordLTX = 8;
	coordLTY = 0;
}

void GameObject::KeyPressed(int btnCode)
{
	if (btnCode == 77) // стрелка вправо	
		coordLTX++;

	else if (btnCode == 75) // стелка влево
		coordLTX--;

	else if (btnCode == 80) // ускорение стрелкой вниз
		counter = 25;

	else if (btnCode == 72 && rotate != 0) // поворот стрелкой вверх
	{
		for (int i = figureSize - 1, k = 0; i >= 0, k < figureSize; i--, k++)
			for (int j = 0; j < figureSize; j++)
				tempFigure[j][k] = figure[i][j];
		rotateStep = true;
	}
}
void GameObject::UpdateF(float deltaTime)
{
	if (!stopStep)
	{
		counter++;
		for (int i = 0; i < figureSize; i++) // стирание "следов"
			for (int j = 0; j < figureSize; j++)
				if (figure[j][i] == 1)
					SetChar(oldCoordLTX + i, coordLTY + j, L' ');	

		for (int i = 0; i < figureSize; i++) // проверка на столкновение справа или слева
			for (int j = 0; j < figureSize; j++)
				if (figure[i][j] == 1 && L' ' != GetChar(coordLTX + j, coordLTY + i))
					coordLTX = oldCoordLTX;

		for (int i = figureSize - 1; i >= 0; i--) // проверка на столкновение нижнего элемента фигуры		
			for (int j = figureSize - 1; j >= 0; j--)
				if (figure[i][j] == 1 && counter >= 25)
					if (GetChar(j + coordLTX, i + coordLTY + 1) != L' ')
					{			
						stopStep = true;
						return;
					}

								

		if (counter >= 25) // задержка для возможности передвигать по горизонтали быстрее чем по вертикали
		{
			if (!stopStep)
				coordLTY++;
			counter = 0;
		}
		
		if (rotateStep) // разрешение на поворот
		for (int i = 0; i < figureSize; i++)
			for (int j = 0; j < figureSize; j++)
				if (tempFigure[i][j] == 1 && GetChar(coordLTX + j, coordLTY + i) != L' ')
				{
					rotateStep = false;
					return;
				}
		draw();
		oldCoordLTX = coordLTX;		
	}

	if (rotateStep)
	{
		for (int i = 0; i < figureSize; i++) // удаление старых элементов
			for (int j = 0; j < figureSize; j++)
				if (figure[j][i] == 1)
					SetChar(coordLTX + i, coordLTY + j, L' ');
		for (int i = 0; i < figureSize; i++) //в массиве теперь фигура перевернута
			for (int j = 0; j < figureSize; j++)
				figure[i][j] = tempFigure[i][j];
		rotateStep = false;
	}	
}

void GameObject::draw()
{
	for (int i = 0; i < figureSize; i++) // рисование символов
		for (int j = 0; j < figureSize; j++)
			if (figure[j][i] == 1)
				SetChar(coordLTX + i, coordLTY + j, L'0');
}

int GameObject::checkLine()
{
	draw();
	for (int k = 0; k <= SIZE_Y; k++)
	{
		if (GetChar(1, k) == L'0')	// проверка по первому столбцу
			for (int m = 1; m < SIZE_X; m++)
				if (GetChar(m, k) != L'0')
					break;
				else
					++counterForDelete;
		if (counterForDelete == 16)
		{			
			counterForDelete = 0;
			return k;
		}
		counterForDelete = 0;
	}
	return 25;
}

void GameObject::deleteComponent(int y)
{
	int temp = y - coordLTY;
	if (beforeDelete(y))
	{		
		switch (temp)
		{
		case 0:
			std::replace(figure[0], figure[0] + 4, 1, 0); break;
		case 1:
			std::replace(figure[1], figure[1] + 4, 1, 0);
			std::swap(figure[1], figure[0]);
			break;
		case 2:
			std::replace(figure[2], figure[2] + 4, 1, 0);
			std::swap(figure[2], figure[1]);
			std::swap(figure[1], figure[0]);
			break;
		case 3:
			std::replace(figure[3], figure[3] + 4, 1, 0);
			std::swap(figure[3], figure[2]);
			std::swap(figure[2], figure[1]);
			std::swap(figure[1], figure[0]);
			break;
		default:
			break;
		}
	}
	else 
		++coordLTY;
}

bool GameObject::beforeDelete(int y)
{
	if (y >= coordLTY && y <= coordLTY + 3)
		return true;
	else
		return false;
}