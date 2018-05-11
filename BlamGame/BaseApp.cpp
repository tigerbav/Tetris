// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#include <time.h>
#include <conio.h>
#include <assert.h>
#include <strsafe.h>
#include "BaseApp.h"
#include "GameObject.h"
#define MY_PERFORMENCE_COUNTER

#include "PerformanceCounter.h"
CHAR_INFO* BaseApp::mChiBuffer;
COORD BaseApp::mDwBufferSize;
COORD BaseApp::mDwBufferCoord;
SMALL_RECT BaseApp::mLpWriteRegion;
HANDLE BaseApp::mConsole;
HANDLE BaseApp::mConsoleIn;
BaseApp:: BaseApp(int xSize, int ySize) : X_SIZE(xSize), Y_SIZE(ySize)
{
	SMALL_RECT windowSize = { 0, 0, X_SIZE, Y_SIZE };
	COORD windowBufSize = { X_SIZE + 1, Y_SIZE + 1 };

	mConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	mConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

	if (!SetConsoleScreenBufferSize(mConsole, windowBufSize))
	{
		cout << "SetConsoleScreenBufferSize failed with error " << GetLastError() << endl;
	}
	if (!SetConsoleWindowInfo(mConsole, TRUE, &windowSize))
	{
		cout << "SetConsoleWindowInfo failed with error " << GetLastError() << endl;
	}

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(mConsole, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(mConsole, &cursorInfo);


	mChiBuffer = (CHAR_INFO*)malloc((X_SIZE + 1)*(Y_SIZE + 1)*sizeof(CHAR_INFO));

	mDwBufferSize.X = X_SIZE + 1;
	mDwBufferSize.Y = Y_SIZE + 1;		// размер буфера данных

	mDwBufferCoord.X = 0;
	mDwBufferCoord.Y = 0;				// координаты €чейки

	mLpWriteRegion.Left = 0;
	mLpWriteRegion.Top = 0;
	mLpWriteRegion.Right = X_SIZE + 1;
	mLpWriteRegion.Bottom = Y_SIZE + 1;	// пр€моугольник дл€ чтени€


	for (int x = 0; x<X_SIZE + 1; x++)
	{
		for (int y = 0; y<Y_SIZE + 1; y++)
		{
			if ((x == SIZE_X || x == 0 || y == SIZE_Y ) && x <= SIZE_X && y <= SIZE_Y)
				SetChar(x, y, L'#');
			else
				SetChar(x, y, L' ');
		}
	}

}

BaseApp::~BaseApp()
{
	free(mChiBuffer);
}

void BaseApp::SetChar(int x, int y, wchar_t c)
{
	mChiBuffer[x + (X_SIZE + 1)*y].Char.UnicodeChar = c;
	mChiBuffer[x + (X_SIZE + 1)*y].Attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
}

wchar_t BaseApp::GetChar(int x, int y)
{
	return mChiBuffer[x + (X_SIZE + 1)*y].Char.AsciiChar;
}

void BaseApp::Render()
{
	if (!WriteConsoleOutput(mConsole, mChiBuffer, mDwBufferSize, mDwBufferCoord, &mLpWriteRegion))
	{
		printf("WriteConsoleOutput failed - (%d)\n", GetLastError());
	}
}

void BaseApp::Run()
{
	CStopwatch timer;
	int sum = 0;
	int counter = 0;

	int deltaTime = 0;
	GameObject* obj[1000];
	int num = 0, change = 0;
	obj[num] = NULL;
	while (1)
	{
		if (obj[num] == NULL)
		{
			int rNum = rand() % 8;
			switch (rNum)
			{
			case 0:	obj[num] = new GameObject(0, 0, 1, 1, 1, 1, 0, 0, 0);	break;
			case 1: obj[num] = new GameObject(0, 1, 0, 1, 0, 1, 0, 1, 1);	break;
			case 2: obj[num] = new GameObject(0, 1, 1, 1, 1, 0, 0, 0, 1);	break;
			case 3: obj[num] = new GameObject(1, 0, 1, 1, 0, 1, 0, 0, 1);	break;
			case 4: obj[num] = new GameObject(1, 0, 1, 0, 1, 1, 0, 0, 1);	break;
			case 5: obj[num] = new GameObject(0, 1, 0, 1, 1, 1, 0, 0, 1);	break;
			case 6: obj[num] = new GameObject(0, 0, 0, 0, 1, 1, 0, 0, 1);	break;
			case 7: obj[num] = new GameObject(0, 0, 0, 0, 0, 1, 0, 0, 0);	break;
			default:
				break;
			}
		}
		timer.Start();
		if (_kbhit())
		{
			obj[num]->KeyPressed(_getch());
			if (!FlushConsoleInputBuffer(mConsoleIn))
				cout << "FlushConsoleInputBuffer failed with error " << GetLastError();
		}		
		obj[num]->UpdateF((float)deltaTime / 1000.0f);
		
		do
		{
			obj[change]->draw();
			++change;
		} while (change <= num);
		change = 0;
		Render();
		if (obj[num]->stopStep) // в случае окончани€ хода
		{
			int temp = obj[num]->checkLine(); // провер€етс€ хоть одна заполненна€ лини€
				if (temp != 25)
				{		
					do
					{
						obj[change]->deleteComponent(temp);//изменени€ компонентов каждого объекта
						++change;
					} while (change <= num);
					change = 0;
				}
			++num;
			obj[num] = NULL;
			for (int i = 0; i < SIZE_X; i++)
				if (GetChar(i, 1) == L'0' || num == 1000)
				{
					system("cls");
					std::cout << "Game over!\n";
					Sleep(5000);
					exit(1);
				}				
			continue;
		}
		Sleep(1);

		while (1)
		{
			deltaTime = timer.Now();
			if (deltaTime > 20)
				break;
		}

		sum += deltaTime;
		counter++;
		if (sum >= 1000)
		{
			TCHAR  szbuff[255];
			StringCchPrintf(szbuff, 255, TEXT("FPS: %d"), counter);
			SetConsoleTitle(szbuff);

			counter = 0;
			sum = 0;
		}
	}
}