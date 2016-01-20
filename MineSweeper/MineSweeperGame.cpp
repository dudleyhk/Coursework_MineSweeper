/*
	Details: Main Game File

	Notes: 
	- Make program cleaner
	- Validate all inputs
	- check how many letters i can input when asked for F or D option
	- instructions?

	CURRENTLY WORKING THROUGH THE PROGRAM TRYING TO MAKE IT CLEANER, MORE READABLE AND SLIGHTLY MORE TECHNICAL.
	USING NAMESPACES TO BE MORE ORGANISED AND USING FOR LOOP TECHNIQUES TO MAKE THE PROGRAM MORE USER INTERFACE FRIENDLY.
	SEE CODING STYLE DOCUMENT. HEIGHT AND WIDTH COULD BE INPUT IN THE MINESWEEPER OBJECT AND THEREFORE CHECKED FOR ERRORS.
	ALSO USE TRY AND CATCH FUNCITONS TO CHECK FOR ERRORS

	SHORT INT HAS RANGE FROM -32,768 - 32767 AND IS ONLY 2 BYTES. NORMAL INT RANGES FROM -2MIL TO 2MIL AND IS 4 BYTES

*/

#include "MineSweeperGame.h"

#include <iostream>
#include <string>
using namespace std;


/**START**/
int main()
{
	// cerate instance of the mineSweeper class.
	MineSweeper msGame;

	int continueGame = 1;
	bool gameLoop = false;


	gameLoop = true;

	while (gameLoop)
	{
		System::welcome();							// Print title
		continueGame = msGame.mainMenu();	// Goto MainMenu


		// IF the player wishes to exit the game
		if (continueGame == 0)
		{
			// break out of the loop
			gameLoop = false;
		}
	} // END loop

	return 0;
}


/**************************Data Access******************************/

MineSweeper::MineSweeper()
{
	// set game mode variable to unused
	gameMode = -1;
	continueGame = 1;
}

MineSweeper::~MineSweeper()
{
}

/**THIS IS NOT LONGER VALID AS HEIGHT AND WIDTH ARE STORED IN THE MSGAME CLASS**/
void MineSweeper::getSize()
{
	height = settings.getHeight();
	width = settings.getWidth();
	numberOfMines = settings.calcMines(gameMode);
}

/*Pass Height, Width and Number of Mines to the Mine Grid and Visual Grid*/
void MineSweeper::passSize()
{
	mineGrid.setSize(height, width, numberOfMines);
	visualGrid.setSize(height, width, numberOfMines);
}


/*Create new coordinate variables to be used by the program.*/
void MineSweeper::createNewCoords()
{
	coordinates.r = inputCoordR - 1;
	coordinates.c = inputCoordC - 1;
}


/*************************Functionality*****************************/

int MineSweeper::mainMenu()
{
	int isValid = 1;		// Assume value is correct.
	int userInput = 0;

	bool mainMenuRepeat = false;


	mainMenuRepeat = true;

	// Enter menu loop
	while (mainMenuRepeat)
	{
		cout << "MAIN MENU" << endl;
		cout << "----------" << endl << endl;

		cout << "1. Play Game" << endl;
		cout << "2. Settings" << endl;
		cout << "3. Exit" << endl;
		cout << "Selection: ";

		// Read user input
		cin >> userInput;

		/*VALIDATE INPUT*/


		// continue if input is correct
		if (isValid == 1)
		{
			switch (userInput)
			{
			case 1:
				// IF difficultly has not been selected
				if (gameMode == -1)
				{
					//  set default mode
					gameMode = 0;
					settings.setDefaultSize(5, 5);
				}

				getSize();


				system("cls");
				System::welcome();
				continueGame = loadGame();
				

				break;
			case 2:
				// clear the screen
				system("cls"); 
				System::welcome();
				gameMode = settingsMenu();

				break;
			case 3:
				continueGame = 0;
				break;


			default:
				cout << "Enter value 1 - 3" << endl;
				break;
			} // END userInput switch
		}
		else
		{
			cout << "Invalid input. Please check and try agian." << endl;
		}


		// IF variable is 0
		if (continueGame == 0)
		{
			// break loop to exit
			mainMenuRepeat = false;
		}
	} // END menu loop


	return continueGame;
}

int MineSweeper::settingsMenu()
{
	int userInput = 0;
	bool settingsMenuRepeat = false;


	// Print out the settingsMenu
	System::settingsInterface();

	// Run loop until user input is valid
	settingsMenuRepeat = true;
	while (settingsMenuRepeat)
	{
		cout << "Selection: ";

		cin >> validateUserInput;
		getline(cin, validateUserInput);

		/**VALIDATE INPUT***/
		try
		{
			ErrorHandling::checkOptionSelect(validateUserInput);

			// if the input comes back valid
			// set isValid to true and parse answer and save as userInput
			userInput = stoi(validateUserInput);
			isValid = 1;
		}
		catch (int n)
		{
			// Print error message and SET isValid to falase;
			ErrorHandling::printMessage(n);
			isValid = 0;
		}


		if (isValid == 1)
		{
			switch (userInput)
			{
			case 0:
				gameMode = 0;
				settingsMenuRepeat = false;

				break;

			case 1:
				gameMode = 1;
				settingsMenuRepeat = false;
				break;

			case 2:
				gameMode = 2;
				settingsMenuRepeat = false;
				break;

			case 3:
				gameMode = 3;
				settingsMenuRepeat = false;
				break;

			default:
				cout << "Invaild input." << endl;
				break;

			}// END switch
		}
	}// END menu loop


	// IF default has NOT been selected
	if (gameMode != 0)
	{
		inputGridSize();				// USER INPUT size
	}
	else
	{
		settings.setDefaultSize(5, 5);	// Use DEFAULT size
	}

	// CHECK GETSIZE FUNCTION HEIGH AND WIDTH STORED LOCALLY
	getSize();


	
	system("cls");			// Clear console
	System::welcome();		// Reprint Welcome message


	return gameMode;
}


/*This function sets and initialise the objects/ variables need to run the game.
Including passing variable to the visual and mine objects. Initialsing both objects 
arrays, assigning mines random positions and finally runnning the game.*/
int MineSweeper::loadGame()
{
	passSize();
	mineGrid.initialiseArray();
	mineGrid.placeMines();
	visualGrid.initialiseArray();

	continueGame = playGame();

	return continueGame;
}


/*
Return Key (This relates to playGame() and find()) :
0 = Hit Bomb
1 = Valid
2 = Could not be Flagged
3 = Could not be Dug
4 = Invalid Letter
5 = Invalid Coordinates
*/
int MineSweeper::playGame()
{
	bool inGame = false;
	int valInput = 1;
	

	cout << "There are " << numberOfMines << " mines hidden. Enjoy :) :)" << endl;
	cout << "-----------------------------------------" << endl;

	inGame = true;

	while (inGame)
	{
		visualGrid.displayGrid();
		valInput = guessPosition();


		// check validation options
		if (valInput != 1)
		{
			switch (valInput)
			{

			case 0: 
				cout << "BOOOOOOOOM.. OOps. YOU LOOSE.." << endl;
				findMinePositions();
				visualGrid.displayGrid();

				continueGame = continueOrQuit();
				inGame = false;


				break;

				/*1 = VALID INPUT. UNUSED FOR ERROR CHECKING*/

			case 2:
				cout << "Position could not be Flagged. Please check and try again." << endl;
				break;
			case 3:
				cout << "Position could not be Dug. Please check and try again." << endl;
				break;
			case 4:
				cout << "Please select 'F' to Flag/ 'D' to Dig a position" << endl;
				break;

			case 5:
				cout << "Invalid coordinates. Please check and try again." << endl;
				break;
			}
		}
		else
		{
			// update number of correctly flagged mines
			updateCounter();


			// Check if game is won
			if (correctFlags == numberOfMines && totalFlags == numberOfMines)
			{
				System::winner();
				continueGame = continueOrQuit();
				continueGame = false;
			}
		}
	}
	
	return continueGame;
}


void MineSweeper::inputGridSize()
{
	bool isRepeat = false;

		cout << "Input size in format (height - width) For Example: 4 6 " << endl;
		

		cout << "----------------------------------------------" << endl;


	isRepeat = true;

	while (isRepeat)
	{
		/*****VALIDATE INPUT****/
		try
		{
			ErrorHandling::checkHeightWidth(validateUserInput, gameMode);	// test the input
			throw errorNumber;												// if error is found

			// IF it input is valid break out of loop
			isRepeat = false;
		}
		catch (int n)	// catch error number
		{
			ErrorHandling::printMessage(n);
		}
	}// END while
}


int MineSweeper::guessPosition()
{
	int fVal = 1, isSafe = 1, v = 1;
	char charInput = '*';
	bool inputValid = false;

	cout << "Please enter coordintes and option:" << endl;
	cin >> validateUserInput;

	/*****VALIDATE USER INPUT*****/



	// create coords to be used by the program.
	createNewCoords();

	// make upper case
	if (islower(charInput))
	{
		charInput = toupper(charInput);
	}

	switch (charInput)
	{
	case 'F':
		fVal = visualGrid.flag(coordinates.c, coordinates.r);

		// position not flagged.
		if (fVal == 0)
		{
			v = 2;
		}

		break;
	case 'D':
		isSafe = mineGrid.dig(coordinates.c, coordinates.r);


		if (isSafe == -1)
		{
			// hit Bomb :S oops
			v = 0;
		}
		else if (isSafe == 0)
		{
			v = 3;
		}
		else
		{
			// select char to display
			changeIntToChar();

			break;
	default:
		// char is invalid.
		v = 4;
		break;
		}
	}

	return v;
}

/*Check each element of the grids to check the number of total flags on the map, 
and the number of flags correctly positioned on the map. This will avoid players
being able to cheat by randomly placing 10 flags in the grid*/
void MineSweeper::updateCounter()
{
	//char vPos = '*';
	//int mPos = 0;
	int totalFlagsCounter = 0, correctFlagsCounter = 0;

	// 
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			// GET values at each position of the grid for both objects
			int vPos = visualGrid.getPos(col, row);
			int mPos = mineGrid.getPos(col, row);

			if (vPos == 'F')
			{
				totalFlagsCounter++;

				// IF flag is placed on a MINE
				if (mPos == -1)
				{
					correctFlagsCounter++;
				}
			}
		}
	}

	// SET global variables
	totalFlags = totalFlagsCounter;
	correctFlags = correctFlagsCounter;
}


void MineSweeper::changeIntToChar()
{
	int valAtPosition = 0;
	char setChar = '*';

	 
	// get value at mArray position
	valAtPosition = mineGrid.getPos(coordinates.c, coordinates.r);

	switch (valAtPosition)
	{
	case -1:
		setChar = '!';
		break;
	case 0:
		setChar = '0';
		break;
	case 1:
		setChar = '1';
		break;
	case 2:
		setChar = '2';
		break;
	case 3:
		setChar = '3';
		break;
	case 4:
		setChar = '4';
		break;
	case 5:
		setChar = '5';
		break;
	case 6:
		setChar = '6';
		break;
	case 7:
		setChar = '7';
		break;
	case 8:
		setChar = '8';
		break;
	}

	// set position of vArray = to char
	visualGrid.setPos(coordinates.c, coordinates.r, setChar);
}


void MineSweeper::findMinePositions()
{
	int mPos = 0;

	// FOR each position in mine array
	for (int r = 0; r < width; r++)
	{
		for (int c = 0; c < width; c++)
		{
			mPos = mineGrid.getPos(c, r);

			// if position in mArray = -1
			if (mPos == -1)
			{
				// call feedbk function with vArray coords
				changeIntToChar();
			}
		}
	}
}


int MineSweeper::continueOrQuit()
{
	char userInput = 'Y';
	bool loop = false;

	loop = true;

	while (loop)
	{
		cout << "Continue Y/N?" << endl;
		cin >> userInput;

		// make upper case
		if (islower(userInput))
		{
			userInput = toupper(userInput);
		}

		switch (userInput)
		{
		case 'Y':
			continueGame = 1;
			loop = false;

			// delete the memory 
			mineGrid.~MGrid();
			visualGrid.~VGrid();

			system("cls");
			welcome();

			break;

		case 'N':
			continueGame = 0;
			loop = false;
			break;

		default:
			cout << "Invalid input. Please check and try again :)" << endl;
			break;
		}
	}
	return continueGame;
}