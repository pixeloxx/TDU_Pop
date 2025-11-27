/*
  TDU.cpp - Library implementation for electrotactile stimulation.
*/

#include "Arduino.h"
#include "TDU.h"

int TDU::Begin()
{
    _channels = MegaShield_CHANNELS;
    _refreshrate = REFRESH_RATE_HZ; // Hz

    // compute per-channel period in microseconds precisely
    float refreshPeriodUs_f = 1000000.0f / (float)_refreshrate; // e.g. 1e6 / 36
    long refreshPeriodUs = (long)(refreshPeriodUs_f + 0.5f);
    _channelPeriodUs = refreshPeriodUs / _channels; // integer microseconds per channel

    for (int i = 0; i < _channels; i++)
    {
        pinMode(_pins[i], OUTPUT);
    }
    return 0;
}

int TDU::Stimulate()
{
    for (int k = 0; k < _channels; k++)
    {
        if (_electrode[k] > 0)
        { // if a particular electrode is active stimulate it with the appropriate parameters

            // compute time taken by pulses and pauses in microseconds
            long pulsesUs = (long)_OBN[k] * ((long)_PP[k] * (long)_IBN[k] + (long)_IBP[k]);
            _RCP = _channelPeriodUs - pulsesUs; // remaining channel period in microseconds
            if (_RCP < 0)
                _RCP = 0; // guard: don't pass negative to delayMicroseconds

            _Pminus = _PP[k] - _Pplus[k];

            // create IBN pulses in an inner burst, and OBN inner bursts in the outer burst
            for (int j = 0; j < _OBN[k]; j++)
            {
                for (int i = 0; i < _IBN[k]; i++)
                {
                    digitalWrite(_pins[k], HIGH);
                    delayMicroseconds(_Pplus[k]);
                    digitalWrite(_pins[k], LOW);
                    delayMicroseconds(_Pminus);
                }
                // pause between inner bursts
                delayMicroseconds(_IBP[k]);
            }

            // pause for the remaining time left in the channel period
            if (_RCP > 0)
                delayMicroseconds(_RCP);
        }
        else
        {
            digitalWrite(_pins[k], LOW);
            // when electrode is inactive wait one channel period
            delayMicroseconds(_channelPeriodUs);
        }
    }
    return 0;
}

int TDU::StopStimulus()
{
    for (int k = 0; k < _channels; k++)
    {
        digitalWrite(_pins[k], LOW);
    }
    return 0;
}

int TDU::CheckWaveform(int electrode[], int PP[], int Pplus[], int IBN[], int IBP[], int OBN[])
{
    int error = 0;

    for (int i = 0; i < _channels; i++)
    {
        if (Pplus[i] >= PP[i])
        {
            error = 1;
        }
        else if (PP[i] * IBN[i] >= IBP[i])
        {
            error = 2;
        }
        else if ((long)IBP[i] * (long)OBN[i] >= _channelPeriodUs)
        {
            error = 3;
        }
    }
    return error;
}

int TDU::UpdateStimuli(int electrode[], int PP[], int Pplus[], int IBN[], int IBP[], int OBN[])
{
    for (int i = 0; i < _channels; i++)
    {
        _electrode[i] = electrode[i];
        _PP[i] = PP[i];
        _Pplus[i] = Pplus[i];
        _IBN[i] = IBN[i];
        _IBP[i] = IBP[i];
        _OBN[i] = OBN[i];
    }
    return 0;
}
