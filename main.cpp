#include <iostream>
#include <list>
#include <thread>
using namespace std;

#include <Windows.h>

int nScreenWidth = 120;
int nScreenHeight = 80;

struct sSnakeSegment
{
   int x;
   int y;
};

int main()
{
	// Create Screen Buffer
	wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
	for (int i = 0; i < nScreenWidth*nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

   list<sSnakeSegment> snake = {{60, 15}, {61, 15}, {62, 15}, {63, 15}, {64, 15}, {65, 15}, {66, 15}, {67, 15}, {68, 15}, {69, 15}};
   int nFoodX = 30;
   int nFoodY = 15;
   int nScore = 0;
   int nSnakeDirection = 3;
   bool bDead = false;

   while (1)
   {

      // Timing & Input

      // Game Logic

      // Display Rendering
      // * Clear Screen
      for (int i = 0; i < nScreenWidth*nScreenHeight; i++) 
      { screen[i] = L' '; }

      
      // Display Frame
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
   }

   return 0;
}