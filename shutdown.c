#include <devhelp.h>
#include <devdebug.h>
#include <devtype.h>
#include <devrp.h>


#if defined(DEBUG)

static const char* MSG_SHUTDOWN         = "ASPI Router Device Driver Remove";

#else

#define            MSG_SHUTDOWN         0

#endif // DEBUG

WORD16 StratShutdown(RP FAR* _rp)
  {
   RPIOCtl FAR* rp = (RPIOCtl FAR*)_rp;
  // If any work needs to be done when the system is shutdown, it
  // should be performed here.
  // This command should shutdown any hardware devices that were setup
  // during the initialization stage.  Also, this command should free
  // any resources that the driver owns.
  // Print a message to the debug terminal saying when shutdown
  cdbg << SKELETON;
  cdbg << MSG_SHUTDOWN << hex << setw(4);
  cdbg << setw() << setb() << endl;

  return RPDONE;//| RPERR_COMMAND;
  }
