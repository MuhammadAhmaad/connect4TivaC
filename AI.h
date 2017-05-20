#include <stdlib.h> 
#include <time.h>
#include <stdio.h>
int provocation = 0;// used to display a provocative screen
char input[43]; // There are 42 places to play in the board .. this array represent them
void Board(void);
int PlayOut = 0;
int EVA = 0;
// EVA and PlayOut are working while NegaMax Function is working .. in NegaMax's way it will definitely make any winning move
// as it goes deep so when this happens PlayOut increases by 1 and EVA increases by 1 if its a winnning move or decreases by 1 if it the opposite
// so we can evaluate neutral moves for MiniMax by dividing EVA / PlayOut * 100 so we get a percentage
int winning(void);
int GetValue(int t);
int AIManager(void);
int NegaMax(int Depth);
void clean() {
  int i;
  provocation = 0;
  for (i = 0; i <= 80; i++)
    input[i] = ' ';
}

int GetValue(int column) // pass this function a column that you want to play in and it will return its value in input array ..
  {
    int i;
    int n = 0;
    if (column > 7)
      return 0;

    for (i = 0; i <= 6; i++) {
      if (input[column + 7 * i] == ' ') {
        n = column + 7 * i;
        break;
      }
    }
    if (n > 42)
      return 0;
    return n;
  }

int winning() // Winning algorithm
  {
    int temp = 0;
    int i;
    for (i = 1; i <= 42; i++) {
      if (input[i] != ' ') {
        int
        var = ((i - 1) / 7);
        temp++;
        if (i - var * 7 <= 4)
          if (input[i] == input[i + 1] && input[i] == input[i + 2] && input[i] == input[i + 3]) {
            if (input[i] == 'X')
              return 1;
            else
              return 2;
					}
        if (i <= 21)
          if (input[i] == input[i + 7] && input[i] == input[i + 14] && input[i] == input[i + 21]) {
            if (input[i] == 'X')
              return 1;
            else
              return 2;
					}
        if (i - var * 7 <= 4 && i <= 18) 
          if (input[i] == input[i + 8] && input[i] == input[i + 16] && input[i] == input[i + 24]) {
            if (input[i] == 'X')
              return 1;
            else
              return 2;
					}
        if (i - var * 7 >= 4 && i <= 21)
          if (input[i] == input[i + 6] && input[i] == input[i + 12] && input[i] == input[i + 18]) {
            if (input[i] == 'X')
              return 1;
            else
              return 2;
					}
      }
    }
    if (temp == 42)
      return 3;
    return 0;
  }
int PlayPosition(char XO, int sth) // Function that asks you to enter where you like to play
  {
      sth = GetValue(sth);
      if (sth != 0) {
        input[sth] = XO;
        return 1;
      }
           else 
						 return 0;
  }

int AIManager() // AI Algorithm
  {
    int column;
    float temp;
    int PlayNumber;
    float chance[2] = {
      9999999,
      0
    };
    for (column = 1; column <= 7; column++) {
      PlayOut = 0;
      EVA = 0;
      PlayNumber = GetValue(column);
      if (PlayNumber != 0) {
        input[PlayNumber] = 'O';
        if (winning() == 2) {
          input[PlayNumber] = ' ';
          return PlayNumber;
        }
        temp = -(100 * NegaMax(1));
        if (PlayOut != 0)
          temp -= ((100 * EVA) / PlayOut);
        if (-temp >= 100)
          provocation = 1;
        if (chance[0] > temp) {
          chance[0] = temp;
          chance[1] = PlayNumber;
        }
        input[PlayNumber] = ' ';
      }
    }
    return chance[1];
  }
int NegaMax(int Depth) // MiniMax algorithm in NegaMax form
  {
    char XO;
    int column;
    int PlayNumber[8] = {
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0
    }; // The values of the input[] for every column
    int chance = 0;
    if (Depth % 2 != 0)
      XO = 'X';
    else
      XO = 'O';

    for (column = 1; column <= 7; column++)
      PlayNumber[column] = GetValue(column);
    for (column = 1; column <= 7; column++) {
      if (PlayNumber[column] != 0) {
        input[PlayNumber[column]] = XO;
        if (winning() != 0) {
          PlayOut++;
          if (XO == 'O')
            EVA++;
          else
            EVA--;
          input[PlayNumber[column]] = ' ';
          return -1;
        }
        input[PlayNumber[column]] = ' ';
      }
    }
    if (Depth <= 3) {
      int column;
      for (column = 1; column <= 7; column++) {
        int temp = 0;
        if (PlayNumber[column] != 0) {
          input[PlayNumber[column]] = XO;
          if (winning() != 0) {
            PlayOut++;
            if (XO == 'O')
              EVA++;
            else
              EVA--;
            input[PlayNumber[column]] = ' ';
            return -1;
          }
          temp = NegaMax(Depth + 1);
          if (column == 1)
            chance = temp;
          if (chance < temp)
            chance = temp;
          input[PlayNumber[column]] = ' ';
        }
      }
    }
    return -chance;
  }
