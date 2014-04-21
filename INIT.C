#include <devdebug.h>
#include <devhelp.h>
#include <devtype.h>
#include <devrp.h>
#include <string.h>
#include <conio.h>
#include "AspiRout.h"

// Constant debugging strings (only defined when DEBUG is defined)

#if defined(DEBUG)

static const char* MSG_CS =                     "Resident Code Size = ";
static const char* MSG_DS =                     "Resident Data Size = ";

#else

#define            MSG_DEBUG              0
#define            MSG_CS                 0
#define            MSG_DS                 0

#endif // DEBUG



// Sign on and installation messages

static const char WELCOME[] =
  "\r\n"
  "ASPI Router Version 1.01 installed\r\n"
  "1997 by Daniel Dorau\r\n";
const int WELCOME_LENGTH = sizeof(WELCOME) - 1;

static const char NO_SCSIMGR[] =
 "Could not get SCSIMGR$ entry point! ASPI Router NOT loaded.\r\n"
 "Check if OS2ASPI.DMD is loaded.\r\n";
const int NO_SCSIMGR_LENGTH = sizeof(NO_SCSIMGR) - 1;

static const char DEVCTXFAILED[] =
 "Device Helper call allocating context hook failed. ASPI Router NOT loaded.\r\n";
const int DEVCTXFAILED_LENGTH = sizeof(DEVCTXFAILED) - 1;

static char* AttachName =                       "SCSIMGR$";

// Initialize device driver

WORD16 StratInit(RP FAR* _rp)
  {

  RPInit FAR* rp = (RPInit FAR*)_rp;
  int result=0;

  // MANDATORY: Initialize the Device Driver libraries.
  DevInit(rp->In.DevHlp);

  // Signal that we've installed successfully by setting the size of
  // our code and data segments.
  rp->Out.FinalCS = OffsetFinalCS;
  rp->Out.FinalDS = OffsetFinalDS;

#if defined(DEBUG)
  // Print a sign on message to the debugging terminal.
  cdbg << ALL << endl;
  cdbg << WELCOME << endl;
  cdbg << MSG_CS << hex << setw(4) << OffsetFinalCS
       << setw() << dec << 'h' << endl;
  cdbg << MSG_DS << hex << setw(4) << OffsetFinalDS
       << setw() << dec << 'h' << endl;
  cdbg << endl;
#endif

  // Print a sign on message to the console.
  DosPutMessage(1, WELCOME_LENGTH, WELCOME);

  //user code
  if (DevAttachDD(AttachName, &idc))
  {
#if defined(DEBUG)
    cdbg << "Could not get SCSIMGR$ entry point!" << endl;
#endif
    DosPutMessage(1, NO_SCSIMGR_LENGTH, NO_SCSIMGR);
    rp->Out.FinalCS=0;
    rp->Out.FinalDS=0;
    return RPDONE | RPERR;
  }
  else
#if defined(DEBUG)
    cdbg << "DevAttachDD successful." << endl;
    cdbg << "Segment=" << hex << setw(4) << idc.Segment << setw() << dec << 'h' << endl;
    cdbg << "Offset=" << hex << setw(4) << idc.Offset << setw() << dec << 'h' << endl;
    cdbg << "Data=" << hex << setw(4) << idc.Data << setw() << dec << 'h' << endl;
#endif
    AspiEntry=idc.Segment;
    AspiEntry=AspiEntry << 16;
    AspiEntry=AspiEntry | (idc.Offset);
    AspiData=idc.Data;
#if defined(DEBUG)
    cdbg << "AspiEntry=" << hex << setw(8) << AspiEntry << setw() << dec << 'h' << endl;
    cdbg << "AspiData=" << hex << setw(4) << AspiData << setw() << dec << 'h' << endl;
#endif

  // Allocate Context hook
  Hook1 = DevCtxAllocate((VOID FAR*) ctx_hand);
  if (!Hook1)
  {
    // Signal that we will not initialize without a context hook
#if defined(DEBUG)
    cdbg << "DevCtxAllocate failed." << endl;
#endif
    DosPutMessage(1, DEVCTXFAILED_LENGTH, DEVCTXFAILED);
    rp->Out.FinalCS=0;
    rp->Out.FinalDS=0;
  }
#if defined(DEBUG)
  else cdbg << "DevCtxAllocate returned successfully." << endl;
#endif
  return RPDONE;
}

