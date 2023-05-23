///
//

#ifndef _DEBUG_CTRL_H
#define _DEBUG_CTRL_H

//  Convienence macro for formatting debug output
#define DEBUG_OUT(x, y) { char pMsg[32]; \
    if((stDebugInfo.u16DebugFlags & CSV_DEBUG) == 0) \
      sprintf(pMsg, "%s:%d\r\n", x, y); \
    else \
      sprintf(pMsg, "%s:%d, ", x, y); \
    /* strcat(stDebugInfo.szDebugMsg, pMsg); */  \
      SerialWrite((uint8_t *)pMsg, 0); \
    }

///
// @brief DEBUG macros
//
#if defined (DEBUG) && !defined (NDEBUG)
  #define DEBUG_BREAK() do { __asm("bkpt #0"); } while(0)
  #define DEBUG_NOP() __no_operation()
  // this macro allows debugging local vars in debug mode.
  #define STATIC    static
#else
  #define DEBUG_BREAK()
  #define DEBUG_NOP()
  #define STATIC
#endif

#endif  // _DEBUG_CTRL_H
