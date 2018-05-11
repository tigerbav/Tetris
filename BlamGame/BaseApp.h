// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#pragma once

#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const int SIZE_X = 17;
const int SIZE_Y = 22;

class BaseApp
{
private:
	static HANDLE mConsole;
	static HANDLE mConsoleIn;

	static CHAR_INFO* mChiBuffer;
	static COORD mDwBufferSize;
	static COORD mDwBufferCoord;
	static SMALL_RECT mLpWriteRegion;

	static void Render();

public:

	//������� ������� ������ �� ����������� � ��������� � ��������
	const int X_SIZE;
	const int Y_SIZE;

	//����������� �������� ������� ������� ������ �� ����������� � ��������� � ��������
	BaseApp(int xSize = 100, int ySize = 80);
	virtual ~BaseApp();

	//��������� ������� ����
	void Run();

	//����� ��������� x,y-������ ������ ������������ ��������, ��� ������� ���
	void SetChar(int x, int y, wchar_t c);
	wchar_t GetChar(int x, int y);

	/*��� ������� ���������� ������ ������� ��������, � ����� ��������������, � ���������� ������.
	� �� �������� deltaTime - ������� �� ������� ����� ���������� ��������� � ����, � ��������*/
	virtual void UpdateF(float deltaTime){}
	/*��� ������� ���������� ��� ������� ������� �� ����������, � �� �������� ��� ������� - btnCode.
	���� ������������ ������� ��� �������������� �������, �� ������ ����� ��� ��� �����, �������� 224, � ��������� �� ��� -
	����� ��� ��������������� ��� ����� �������, ��� ����� ��������, ������ ����� getch().
	����� KeyPressed ��� �� ����� �������������� � ����������*/
	virtual void KeyPressed(int btnCode){}
};