

#include "devdebug.h"



// Standard debugging streams

#if defined(DEBUG)

COMStream cdbg =
  {
  0x2f8,                      // Default to COM2
  10,                         // Base 10 (decimal)
  0,                          // Variable width
  USER0,                      // Default debugging level
  ALL                         // Don't mask any
  };

COMStream ccom1 =
  {
  0x3f8,                      // COM1 base port address
  10,                         // Base 10 (decimal)
  0,                          // Variable width
  USER0,                      // Default debugging level
  ALL                         // Don't mask any
  };

COMStream ccom2 =
  {
  0x2f8,                      // COM2 base port address
  10,                         // Base 10 (decimal)
  0,                          // Variable width
  USER0,                      // Default debugging level
  ALL                         // Don't mask any
  };

#endif // DEBUG
