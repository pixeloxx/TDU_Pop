#include "TDU.h"

/*
  A simple demonstration of Tongue Display Unit with basic patterns
*/

int array[MegaShield_CHANNELS];

// two arrays I used to generate geometric patterns

int cElectrodeMap[5][5] = {
    {0, 1, 2, 4, 5},
    {6, 7, 8, 9, 10},
    {11, 12, 13, 14, 15},
    {16, 17, 18, 19, 20},
    {21, 22, 23, 24, 25}};

int on[5][5] = {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0}};

int active[5][5] = {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0}};

// these are waveform parameter as described by Kurt Kaczmarek here https://www.sciencedirect.com/science/article/pii/S10MegaShield_CHANNELS309811001702
// than can be changed to create a large range of sensations on the tongue.
int PP[] = {57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57};
int Pp[] = {57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57};
int IN[] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
int IP[] = {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150};
int ON[] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
TDU mytdu; // creating an instance of TDU

// a function to update geometric patterns in the demo at the beginning of the program
void updatepattern()
{

  // if the On array is ative, set the corresponding electrode active.
  for (int j = 0; j < 5; j++)
  {
    for (int i = 0; i < 5; i++)
    {
      array[cElectrodeMap[i][j]] = on[i][j];
    }
  }
}

void resetParams()
{
  // assign default waveform parameters element-wise
  for (int i = 0; i < MegaShield_CHANNELS; i++)
  {
    PP[i] = 57;
    Pp[i] = 57;
    IN[i] = 3;
    IP[i] = 150;
    ON[i] = 5;
  }
}
void clearPattern()
{
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      on[i][j] = 0;
    }
  }
}
// a demonstration of geometric patterns (lines, sweeps, pulsing X's
void demonstration()
{
  resetParams();
  // lines top to bottom 5x
  for (int m = 0; m < 5; m++)
  {
    Serial.println("Sweeping lines top to bottom.");
    for (int i = 0; i < 5; i++)
    {
      on[i][0] = 1;
      on[i][1] = 1;
      on[i][2] = 1;
      on[i][3] = 1;
      on[i][4] = 1;
      updatepattern();

      mytdu.UpdateStimuli(array, PP, Pp, IN, IP, ON);

      for (int k = 0; k < 3; k++)
      {
        mytdu.Stimulate();
      }
      Serial.println("check 3");
      on[i][0] = 0;
      on[i][1] = 0;
      on[i][2] = 0;
      on[i][3] = 0;
      on[i][4] = 0;
      updatepattern();
      mytdu.UpdateStimuli(array, PP, Pp, IN, IP, ON);
    }
  }

  // lines left to right 5x
  for (int m = 0; m < 5; m++)
  {
    Serial.println("Sweeping lines left to right.");
    for (int i = 0; i < 5; i++)
    {
      on[0][i] = 1;
      on[1][i] = 1;
      on[2][i] = 1;
      on[3][i] = 1;
      on[4][i] = 1;
      updatepattern();
      mytdu.UpdateStimuli(array, PP, Pp, IN, IP, ON);
      for (int k = 0; k < 3; k++)
      {
        mytdu.Stimulate();
      }
      on[0][i] = 0;
      on[1][i] = 0;
      on[2][i] = 0;
      on[3][i] = 0;
      on[4][i] = 0;
      updatepattern();
      mytdu.UpdateStimuli(array, PP, Pp, IN, IP, ON);
    }
  }

  clearPattern();
  // single points left to right

  for (int i = 0; i < MegaShield_CHANNELS; i++)
  {
    ON[i] = 5;
  }

  for (int index = 0; index < MegaShield_CHANNELS; index++)
  {
    // one by one, set each electrode active
    clearPattern();
    int x = index / 5;
    int y = index % 5;
    on[x][y] = 1;

    for (int m = 0; m < 5; m++)
    {
      Serial.print("single point");
      Serial.println(index);
      mytdu.UpdateStimuli(array, PP, Pp, IN, IP, ON);
      for (int k = 0; k < 6; k++)
      {
        mytdu.Stimulate();
      }
    }

    updatepattern();
    mytdu.UpdateStimuli(array, PP, Pp, IN, IP, ON);
  }

  for (int i = 0; i < MegaShield_CHANNELS; i++)
  {
    ON[i] = 5;
  }

  // Pulsing X 5x
  on[1][0] = 1;
  on[2][1] = 1;
  on[3][2] = 1;
  on[4][3] = 1;
  on[4][0] = 1;
  on[3][1] = 1;
  on[2][2] = 1;
  on[1][3] = 1;
  updatepattern();
  for (int m = 0; m < 5; m++)
  {
    Serial.println("Pulsing 'X' with varying intensity.");
    for (int i = 1; i < 50; i++)
    {
      for (int x = 0; x < MegaShield_CHANNELS; x++)
      {
        Pp[x] = i + 7;
        PP[x] = i + 8;
        ON[x] = (i + 10) / 10;
        IP[x] = 100 + i;
      }
      mytdu.UpdateStimuli(array, PP, Pp, IN, IP, ON);
      for (int k = 0; k < 1; k++)
      {
        mytdu.Stimulate();
      }
    }
  }
  for (int x = 0; x < MegaShield_CHANNELS; x++)
  {
    Pp[x] = 9;
  }
  on[1][0] = 0;
  on[2][1] = 0;
  on[3][2] = 0;
  on[4][3] = 0;
  on[4][4] = 0;
  on[4][0] = 0;
  on[3][1] = 0;
  on[2][2] = 0;
  on[1][3] = 0;
  on[0][4] = 0;
  updatepattern();
  mytdu.UpdateStimuli(array, PP, Pp, IN, IP, ON);
}

void setup()
{
  mytdu.Begin();
  Serial.begin(9600);
  delay(1000);
  Serial.println("sizeof(Pp): ");
  Serial.println(sizeof(Pp) / sizeof(Pp[0]));
  Serial.println("sizeof(ON): ");
  Serial.println(sizeof(ON) / sizeof(ON[0]));
  Serial.println("sizeof(PP): ");
  Serial.println(sizeof(PP) / sizeof(PP[0]));
  Serial.println("sizeof(IN): ");
  Serial.println(sizeof(IN) / sizeof(IN[0]));
  Serial.println("sizeof(IP): ");
  Serial.println(sizeof(IP) / sizeof(IP[0]));
  Serial.println("Pattern Demonstration begin...");
  delay(1000);
}

void loop()
{
  // do nothing
  demonstration(); // run through demonstration once
}
