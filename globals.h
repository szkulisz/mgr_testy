#ifndef GLOBALS
#define GLOBALS

#include <map>
#include <QAtomicInt>

extern std::map<int, long long> gDiffBuffer1,gDiffBuffer2;
extern QAtomicInt gBufferFull, gWhichBufferIsActive, gContinueLogging;


#endif // GLOBALS

