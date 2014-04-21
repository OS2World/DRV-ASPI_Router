#include <devhelp.h>
#include <devdebug.h>
#include <devtype.h>
#include <devrp.h>


#if defined(DEBUG)

static const char* MSG_ERROR         = "Watcom Sample Device Driver Remove [ERROR]";

#else

#define            MSG_ERROR         0

#endif // DEBUG

WORD16 StratError(RP FAR* _rp)
  {
  RPIOCtl FAR* rp = (RPIOCtl FAR*)_rp;
  // This command should shutdown any hardware devices that were setup
  // during the initialization stage.  Also, this command should free
  // any resources that the driver owns.
  // Print a message to the debug terminal saying when error
  cdbg << SKELETON;
  cdbg << MSG_ERROR << hex << setw(4);
  cdbg << setw() << setb() << endl;
  return RPDONE | RPERR_COMMAND;
  }
