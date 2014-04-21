#include <devhelp.h>
#include <devdebug.h>
#include <devtype.h>
#include <devrp.h>


#if defined(DEBUG)

static const char* MSG_WRITE     = "Watcom Sample Device Driver Write";

#else

#define            MSG_WRITE     0

#endif // DEBUG

WORD16 StratWrite(RP FAR* _rp)
  {
   RPIOCtl FAR* rp = (RPIOCtl FAR*)_rp;
  // This command should shutdown any hardware devices that were setup
  // during the initialization stage.  Also, this command should free
  // any resources that the driver owns.
  // Print a message to the debug terminal saying when write
  cdbg << SKELETON;
  cdbg << MSG_WRITE << hex << setw(4);
  cdbg << setw() << setb() << endl;

  return RPDONE;
  }
