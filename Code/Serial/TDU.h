/*
  TDU.h - Library for stimulating the tongue using electrotactile stimulation.
  Based on original code by Joel Moritz Jr Jan 2019
*/

#include "Arduino.h"

#ifndef TDU_h
#define TDU_h

#define MegaShield_CHANNELS 26
#define REFRESH_RATE_HZ 36

class TDU
{
public:
    int Begin();
    int Stimulate();

    int UpdateStimuli(int electrode[], int PP[], int Pplus[], int IBN[], int IBP[], int OBN[]);
    int StopStimulus();
    int CheckWaveform(int electrode[], int PP[], int Pplus[], int IBN[], int IBP[], int OBN[]);

private:
    int _electrode[MegaShield_CHANNELS] = {0};
    int _PP[MegaShield_CHANNELS] = {10};
    int _Pplus[MegaShield_CHANNELS] = {9}; // this unit in microseconds
    int _IBN[MegaShield_CHANNELS] = {3};
    int _IBP[MegaShield_CHANNELS] = {150}; // this unit in microseconds
    int _OBN[MegaShield_CHANNELS] = {5};

    int _channels;
    int _refreshrate;

    // per-channel period in microseconds (computed from refresh rate and channel count)
    long _channelPeriodUs;

    int _Pminus;
    long _RCP; // remaining channel period in microseconds
    // references for locating location on matrix disply
    // todo: needs to be maped out for Mega shield
    int _PinMap[5][5] = {
        {A1, A2, A3, A4, A5},
        {A6, A7, A8, A9, A10},
        {A11, A12, A13, A14, A15},
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10}};
    // default row-major electrode numbering (0..24) to match a 5x5 grid
    int _ElectrodeMap[5][5] = {
        {0, 1, 2, 3, 4},
        {5, 6, 7, 8, 9},
        {10, 11, 12, 13, 14},
        {15, 16, 17, 18, 19},
        {20, 21, 22, 23, 24}};

    // expanded to MegaShield_CHANNELS entries; entries should be mapped to physical pins
    int _pins[MegaShield_CHANNELS] = {A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
};

#endif
