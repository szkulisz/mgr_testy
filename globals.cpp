#include "globals.h"

std::map<int, long long> gDiffBuffer1,gDiffBuffer2;
QAtomicInt gBufferFull, gWhichBufferIsActive, gContinueLogging = 1;
