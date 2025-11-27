#include "TDU.h"

#define HighPeriod 400
#define LowPeriod 400
#define midDelay 1000
#define midDelay 1000
#define IntensityDelay 10 // adjust this to adjust intensity of tactile buttons.

#define serialSendDelay 20

long lastSerialSendTime = 0;

int _PinMap[5][5] = {
    {A1, A2, A3, A4, A5},
    {A6, A7, A8, A9, A10},
    {A11, A12, A13, A14, A15},
    {1, 2, 3, 4, 5},
    {6, 7, 8, 9, 10}};

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

// reverse lookup for electrode index -> row/col
int indexRow[MegaShield_CHANNELS];
int indexCol[MegaShield_CHANNELS];

void buildReverseMap()
{
  for (int k = 0; k < MegaShield_CHANNELS; k++)
  {
    indexRow[k] = -1;
    indexCol[k] = -1;
  }
  for (int r = 0; r < 5; r++)
  {
    for (int c = 0; c < 5; c++)
    {
      int idx = cElectrodeMap[r][c];
      if (idx >= 0 && idx < MegaShield_CHANNELS)
      {
        indexRow[idx] = r;
        indexCol[idx] = c;
      }
    }
  }
}

// these are waveform parameter as described by Kurt Kaczmarek here https://www.sciencedirect.com/science/article/pii/S10MegaShield_CHANNELS309811001702
// than can be changed to create a large range of sensations on the tongue.
int PP[] = {57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57};
int Pp[] = {57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57};
int IN[] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
int IP[] = {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150};
int ON[] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
TDU mytdu; // creating an instance of TDU

// a function to update geometric patterns in the demo at the beginning of the program
void updatepattern(int input[5][5])
{

  // if the On array is ative, set the corresponding electrode active.
  for (int j = 0; j < 5; j++)
  {
    for (int i = 0; i < 5; i++)
    {
      array[cElectrodeMap[i][j]] = input[i][j];
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

void printArrayCSV(const uint16_t *arr, size_t n)
{
  for (size_t i = 0; i < n; ++i)
  {
    Serial.print(arr[i]);
    if (i + 1 < n)
      Serial.print(','); // comma between values
  }
  Serial.println(); // newline after the line
}

void sensorsToSerial(int sensorCount = MegaShield_CHANNELS)
{

  uint16_t sensors[sensorCount] = {0};

  if (sensorCount <= 0)
    sensorCount = MegaShield_CHANNELS;
  if (sensorCount > MegaShield_CHANNELS)
    sensorCount = MegaShield_CHANNELS;

  // drive the first sensorCount pins HIGH (row-major)
  int idx = 0;
  for (int i = 0; i < 5 && idx < sensorCount; i++)
  {
    for (int j = 0; j < 5 && idx < sensorCount; j++)
    {
      pinMode(_PinMap[i][j], OUTPUT);
      digitalWrite(_PinMap[i][j], HIGH);
      idx++;
    }
  }

  delayMicroseconds(HighPeriod); // positive pulse

  // read back the values into sensors[0..sensorCount-1]
  idx = 0;
  for (int i = 0; i < 5 && idx < sensorCount; i++)
  {
    for (int j = 0; j < 5 && idx < sensorCount; j++)
    {
      pinMode(_PinMap[i][j], INPUT);
      sensors[idx] = floor((uint16_t)analogRead(_PinMap[i][j])/100);
      idx++;
    }
  }

  delay(IntensityDelay);

  // restore the driven pins to LOW output for the first sensorCount pins
  idx = 0;
  for (int i = 0; i < 5 && idx < sensorCount; i++)
  {
    for (int j = 0; j < 5 && idx < sensorCount; j++)
    {
      pinMode(_PinMap[i][j], OUTPUT);
      digitalWrite(_PinMap[i][j], LOW);
      idx++;
    }
  }

  delayMicroseconds(LowPeriod); // negative pulse
  delay(1);

  // print only the elements we read
  printArrayCSV(sensors, sensorCount);
}

void applyStimulus()
{
  updatepattern(active);
  mytdu.UpdateStimuli(array, PP, Pp, IN, IP, ON);

  for (int k = 0; k < 3; k++)
  {
    mytdu.Stimulate();
  }

  updatepattern(on);
  mytdu.UpdateStimuli(array, PP, Pp, IN, IP, ON);
}

// read a line of comma-separated uint16_t values from Serial and use them to set on[][]
void readSerialAndApply()
{
  if (!Serial.available())
    return;

  String line = Serial.readStringUntil('\n');
  line.trim();
  if (line.length() == 0)
    return;

  // parse tokens as strict binary values (0 or 1) separated by commas
  const int maxTokens = MegaShield_CHANNELS;
  uint8_t values[maxTokens];
  int count = 0;

  char buf[256];
  line.toCharArray(buf, sizeof(buf));
  char *tok = strtok(buf, ",");
  while (tok != NULL && count < maxTokens)
  {
    // skip leading whitespace
    char *p = tok;
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
      p++;

    // accept only '0' or '1' as valid token start. Ignore invalid tokens.
    if (*p == '0')
    {
      values[count++] = 0;
    }
    else if (*p == '1')
    {
      values[count++] = 1;
    }
    // otherwise token is ignored

    tok = strtok(NULL, ",");
  }

  // clear previous pattern
  for (int r = 0; r < 5; r++)
    for (int c = 0; c < 5; c++)
      active[r][c] = 0;

  // apply parsed binary values to electrodes 0..count-1 using reverse map
  for (int i = 0; i < count; i++)
  {
    int r = indexRow[i];
    int c = indexCol[i];
    if (r >= 0 && c >= 0)
    {
      active[r][c] = values[i] ? 1 : 0;
      Serial.print(active[r][c]);
    }
  }
  Serial.println();
  // printArrayCSV(sensors, sensorCount);
}

void setup()
{
  mytdu.Begin();
  buildReverseMap();
  Serial.begin(115200);
  Serial.println("Pattern Demonstration begin...");
}

void loop()
{
  // activate patterns, based on patern provided over serial.
  // handle reading values.

  readSerialAndApply();
  applyStimulus();

  if (lastSerialSendTime <= millis() - serialSendDelay)
  {
    sensorsToSerial(7); // only read the first 7 sensors
    lastSerialSendTime = millis();
  }
}