#include "SEGGER_RTT.h"
#include "log.h"

#ifdef USE_RTT_PRINT
void log_init() {
    SEGGER_RTT_Init();
}
#endif
