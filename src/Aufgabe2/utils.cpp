//
// Created by unclec on 17.11.22.
//


#include "utils.h"



double Utils::fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

bool Utils::bRand()
{
    return fRand(-1.0, 1.0) < 0;
}