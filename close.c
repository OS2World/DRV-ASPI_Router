#include <devhelp.h>
#include <devdebug.h>
#include <devtype.h>
#include <devrp.h>
#include "aspiRout.h"


#if defined(DEBUG)

static const char* MSG_CLOSE     = "Watcom Sample Device Driver Close";

#else

#define            MSG_CLOSE      0

#endif // DEBUG

WORD16 StratClose(RP FAR* _rp)
  {
   RPIOCtl FAR* rp = (RPIOCtl FAR*)_rp;
  // This command should shutdown any hardware devices that were setup
  // during the initialization stage.  Also, this command should free
  // any resources that the driver owns.
  // Print a message to the debug terminal saying when shutdown
  cdbg << SKELETON;
  cdbg << MSG_CLOSE << hex << setw(4);
  cdbg << setw() << setb() << endl;
  WORD16 result;
  //close event semaphore
  result=DevEventClose(postSema);
#if defined(DEBUG)
  cdbg << "Semaphore closed with return code " << result << endl;
  cdbg << "trying to unlock segment... with LockHandle " << hex << (WORD32)LockHandle << endl;
#endif
  result=DevSegUnlock(LockHandle);
#if defined(DEBUG)
  cdbg << "DevSegUnlock returned with " << result << endl;
#endif

  return RPDONE;
  }
