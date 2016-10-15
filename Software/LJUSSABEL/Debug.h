#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef DEBUG
#define DEBUG_ENABLED 1
#else
#define DEBUG_ENABLED 0
#endif // DEBUG

#define trace_begin(baud) \
  do { if (DEBUG_ENABLED) Serial.begin(baud);} while (0)

#define trace(x) \
  do { if (DEBUG_ENABLED) Serial.println(x);} while (0)

#endif // _DEBUG_H
