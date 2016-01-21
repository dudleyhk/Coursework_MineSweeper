/*
		details: Settings functions


*/
#include "Settings.h"

#include <iostream>
using namespace std;


/*************************Data Access******************************/

Settings::Settings()
{
	// Default Settings
	defaultHeight = 5;
	defaultWidth = 5;
	numberOfMines = 5;
}

Settings::~Settings()
{
}

/*Functions managing the default settings*/
int Settings::getDefaultHeight()
{
	return defaultHeight;
}

int Settings::getDefaultWidth()
{
	return defaultWidth;
}

void Settings::setDefaultMines(int m)
{
	numberOfMines = m;
}

/************************Functionality*****************************/

int Settings::getNumberOfMines(int area, char gameMode)
{

	switch (gameMode)
	{
	case '0':
		setDefaultMines(5);
		break;
	case '1':
		//easy 15%
		numberOfMines = area * 0.15;
		break;
	case '2':
		// medium 25%
		numberOfMines = area * 0.25;
		break;
	case '3':
		// hard 60%
		numberOfMines = area * 0.4;
		break;
	}

	return numberOfMines;
}
