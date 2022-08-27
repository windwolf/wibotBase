#include "log.h"
#include "SEGGER_RTT.h"
#ifdef USE_RTT_PRINT
void log_init()
{
    SEGGER_RTT_Init();
}
#endif