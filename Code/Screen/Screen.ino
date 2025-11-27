#include "TDU.h"
#include <SparkFun_Qwiic_OLED.h> //http://librarymanager/All#SparkFun_Qwiic_OLED

QwiicMicroOLED myOLED;

int width;
int height;


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
    for (int i = 1; i < 5; i++)
    {
      array[cElectrodeMap[i][j]] = on[i][j];
    }
  }
  //  updateDisplay();
}

void setup()
{
  mytdu.Begin();
  Serial.begin(9600);
  Wire.begin();

  // Initalize the OLED device and related graphics system
  if (myOLED.begin() == false)
  {
    Serial.println("Device begin failed. Freezing...");
    while (true)
      ;
  }
  Serial.println("Begin success");

  // save device dims for the test routines
  width = myOLED.getWidth();
  height = myOLED.getHeight();
}

void loop()
{
  // Loop over our test functions

  // updateDisplay();

  demonstration();
}

void updateDisplay()
{

  myOLED.erase();
  int x, y;
  int spaceX = width / 5;
  int spaceY = height / 5;

  for (int j = 0; j < 5; j++)
  {

    x = spaceX / 2 + (spaceX * j);

    for (int i = 0; i < 5; i++)
    {
      y = spaceY / 2 + (spaceY * i);
      if (on[j][i])
      {
        myOLED.line(x - 3, y - 3, x + 3, y + 3);
        myOLED.line(x - 3, y + 3, x + 3, y - 3);
      }
      else
      {
        myOLED.pixel(x, y);
      }
    }
  }
  myOLED.display();
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
      updateDisplay();
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
      updateDisplay();
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
  updateDisplay();
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