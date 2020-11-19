#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
using namespace std;

#include <stdio.h>
#include <Windows.h>

int nScreenWidth = 120;
int nScreenHeight = 30;
int nFieldWidth = 12;
int nFieldHeight = 18;

wstring tetromino[7];
unsigned char *pField = nullptr;

int Rotate(int px, int py, int r)
{
  int nPieceIndex = 0;
  switch (r % 4)
  {
  case 0:
    nPieceIndex = py * 4 + px;
    break;
  case 1:
    nPieceIndex = 12 + py - (px * 4);
    break;
  case 2:
    nPieceIndex = 15 - (py * 4) - px;
    break;
  case 3:
    nPieceIndex = 3 - py + (px * 4);
    break;
  }

  return nPieceIndex;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
  for (int px = 0; px < 4; px++)
    for (int py = 0; py < 4; py++)
    {

      int nPieceIndex = Rotate(px, py, nRotation);
      int nFieldIndex = (nPosY + py) * nFieldWidth + (nPosX + px);

      if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
      {
        if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
        {
          if (tetromino[nTetromino][nPieceIndex] != L'.' && pField[nFieldIndex] != 0)
            return false;
        }
      }
    }

  return true;
}

int main()
{
   // Create Screen Buffer
   wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
   for (int i = 0; i < nScreenWidth*nScreenHeight; i++) screen[i] = L' ';
   HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
   SetConsoleActiveScreenBuffer(hConsole);
   DWORD dwBytesWritten = 0;

   tetromino[0].append(L"..X...X...X...X.");
   tetromino[1].append(L"..X..XX...X.....");
   tetromino[2].append(L".....XX..XX.....");
   tetromino[3].append(L"..X..XX..X......");
   tetromino[4].append(L".X...XX...X.....");
   tetromino[5].append(L".X...X...XX.....");
   tetromino[6].append(L"..X...X..XX.....");

   pField = new unsigned char[nFieldWidth * nFieldHeight];
   for (int x = 0; x < nFieldWidth; x++)
      for (int y = 0; y < nFieldHeight; y++)
         pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;

   // Game Logic
   bool bKey[4];
   bool bForceDown = false;
   bool bRotateHold = true;

   int nCurrentPiece = 0;
   int nCurrentRotation = 0;
   int nCurrentX = nFieldWidth / 2;
   int nCurrentY = 0;
   
   int nNextPiece = rand() % 7;

   int nSpeed = 20;
   int nSpeedCount = 0;
   int nPieceCount = 0;

   vector<int> vLines;

   std::ifstream input("Tetris.txt");

   int nHighScore;
   input >> nHighScore;

   input.close();

   wsprintf(&screen[9 * nScreenWidth + 43], L"  ______     __       _         __");
   wsprintf(&screen[10 * nScreenWidth + 43], L" /_  __/__  / /______(_)____   / /");
   wsprintf(&screen[11 * nScreenWidth + 43], L"  / / / _ \\/ __/ ___/ / ___/  / / ");
   wsprintf(&screen[12 * nScreenWidth + 43], L" / / /  __/ /_/ /  / (__  )  /_/  ");
   wsprintf(&screen[13 * nScreenWidth + 43], L"/_/  \\___/\\__/_/  /_/____/  (_)   ");
   wsprintf(&screen[15 * nScreenWidth + 40], L"         WELCOME TO TETRIS!         ");
   wsprintf(&screen[16 * nScreenWidth + 40], L"       CURRENT HIGHSCORE: %d       ", nHighScore);
   wsprintf(&screen[17 * nScreenWidth + 40], L"   PRESS 'SPACE' TO START PLAYING  ");
   WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
   while ((0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) == 0);


   while (1)
   {
      for (int i = 0; i < nScreenWidth*nScreenHeight; i++) screen[i] = L' ';
      pField = new unsigned char[nFieldWidth * nFieldHeight];
      for (int x = 0; x < nFieldWidth; x++)
         for (int y = 0; y < nFieldHeight; y++)
         pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;

      int nScore = 0;
      bool bGameOver = false;

      while (!bGameOver)
      {
         // Timing & Input
         // * Frame Timing
         this_thread::sleep_for(50ms);
         nSpeedCount++;
         bForceDown = (nSpeedCount == nSpeed);

         // * Get Input
         for (int k = 0; k < 4; k++)
         bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;

         // Game Logic
         // * Piece Movement
         nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
         nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
         nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

         if (bKey[3])
         {
         nCurrentRotation += (bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
         bRotateHold = false;
         }
         else
         bRotateHold = true;


         if (bForceDown)
         {
            nSpeedCount = 0;
            nPieceCount++;
            if (nPieceCount % 50 == 0 && nSpeed >= 10)
               nSpeed--;

            if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
               nCurrentY++;
            else
            {
               for (int px = 0; px < 4; px++)
                  for (int py = 0; py < 4; py++)
                  if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                     pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

               for (int py = 0; py < 4; py++)
                  if (nCurrentY + py < nFieldHeight - 1)
                  {
                  bool bLine = true;
                  for (int px = 1; px < nFieldWidth - 1; px++)
                     bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

                  if (bLine)
                  {
                     for (int px = 1; px < nFieldWidth - 1; px++)
                        pField[(nCurrentY + py) * nFieldWidth + px] = 8;
                     vLines.push_back(nCurrentY + py);
                  }
                  }

               nScore += 25;
               if (!vLines.empty())
                  nScore += (1 << vLines.size()) * 100;

               nCurrentX = nFieldWidth / 2;
               nCurrentY = 0;
               nCurrentRotation = 0;
               nCurrentPiece = nNextPiece;
               nNextPiece = rand() % 7;

               bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
            }
         }

         // Display Rendering
         // * Draw Field
         for (int x = 0; x < nFieldWidth; x++)
            for (int y = 0; y < nFieldHeight; y++)
               screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];

         // * Draw Current Piece
         for (int px = 0; px < 4; px++)
            for (int py = 0; py < 4; py++)
               if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                  screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;

         // * Draw Score
         wsprintf(&screen[2 * nScreenWidth + nFieldWidth + 6], L"SCORE: %d", nScore);
         wsprintf(&screen[3 * nScreenWidth + nFieldWidth + 6], L"HIGHSCORE: %d", nHighScore);

         // * Draw Blinking Line
         if (!vLines.empty())
         {
            WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, {0, 0}, &dwBytesWritten);
            this_thread::sleep_for(400ms);

            for (auto &v : vLines)
               for (int px = 1; px < nFieldWidth - 1; px++)
               {
                  for (int py = v; py > 0; py--)
                  pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
                  pField[px] = 0;
               }

            vLines.clear();
         }

         // * Death Screen
         if (bGameOver)
         {
            if (nScore > nHighScore) nHighScore = nScore;
            wsprintf(&screen[4 * nScreenWidth + nFieldWidth + 6], L"PRESS 'SPACE' TO PLAY AGAIN");
         }

         // Display Frame
         WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, {0, 0}, &dwBytesWritten);
      }

      std::ofstream output("Tetris.txt");
      output << nHighScore; 
      output.close();

      while ((0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) == 0);
   }

   return 0;
}