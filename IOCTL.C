#include <devhelp.h>
#include <devdebug.h>
#include <devtype.h>
#include <devrp.h>
#include "AspiRout.h"
#include <i86.h>


static WORD32 aspiIDC;
BYTE FAR* SRBPtr;                       // SRBPtr2 added 6.7.97 for v. 1.01 (abort SRB)
WORD32 SRBPhys, SRBPtr2, SRBPhys2;      // SRBPhys2 added 6.7.97 for v. 1.01 (abort SRB)
static WORD32 DATAPtr, DATAPhys;
WORD32 aspiPostAddr;
WORD16 codeseg, dataseg, bufseg, result;
BYTE   SRBCommand;
extern "C" BYTE   postInProgress=0;     // determines wheter post routine is currently active

#if defined(DEBUG)

static const char* MSG_IOCTL              = "Watcom Sample Device Driver IOCtl";
static const char* MSG_CATEGORY           = " Category = ";
static const char* MSG_FUNCTION           = " Function = ";

#else

#define            MSG_IOCTL              0
#define            MSG_CATEGORY           0
#define            MSG_FUNCTION           0

#endif // DEBUG

#define IOCTLCAT  0x92

// Dispatch IOCtl requests received from the Strategy routine
short data;

// IDC to OS2ASPI.DMD with SRB pushed on stack first
void callAspi(void);
#pragma aux callAspi = \
   "push SRBPtr" \
   "push AspiData" \
   "call dword ptr AspiEntry" \
   "add  sp,6";

// Get data segment
WORD16 getDataSeg(void);
#pragma aux getDataSeg = "mov ax,ds" value [ax] modify [ax];

// Context hook called from posting routine to post semaphore
VOID __far __saveregs __loadds ctx_hand(void)   // this is our context hook
{
  DevEventPost(postSema);
}

// posting routine called from OS2ASPI.DMD to notify that SRB is completed.
// See POST.ASM
extern "C" void __cdecl aspiPost(WORD32 SRBPointer)
{
#if defined(DEBUG)
  cdbg << "aspiPost called." << endl;
  cdbg << "SRBPtr: " << SRBPointer << endl;
#endif
  DevCtxArm(Hook1, (WORD32) 0);         // call our hook which will trigger back to app
}

// Entry code for aspiPost. Located in POST.ASM
extern "C" void __far postEntry(WORD16, WORD32);

WORD16 StratIOCtl(RP FAR* _rp)
  {
  RPIOCtl FAR* rp = (RPIOCtl FAR*)_rp;
  // Print a message to the debug terminal saying which IOCtl we just
  // received.  Of course, if DEBUG is not defined, then the following
  // instructions will generate _NO_ code!
  cdbg << SKELETON;
  cdbg << MSG_IOCTL << hex << setw(4);
  cdbg << MSG_CATEGORY << rp->Category;
  cdbg << MSG_FUNCTION << rp->Function;
  cdbg << setw() << setb() << endl;

  if(rp->Category != IOCTLCAT)
     return RPDONE;// | RPERR_COMMAND;
  switch (rp->Function)
     {
     case 0x02:
     // This function 'sends' a SRB to OS2ASPI after converting and completing
     // address fields
       DATAPtr=0;                                               // initialization
       SRBPtr = rp->ParmPacket;                                 // get pointer of SRB
       SRBCommand=*((BYTE FAR*)((WORD32)SRBPtr));               // get SRB command code
       switch (SRBCommand)
       {
         case SRB_Command:
#if defined(DEBUG)
           cdbg << "SRBPtr: " << hex << (WORD32)SRBPtr << endl;
           cdbg << "aspiPost: " << hex << (WORD32)&aspiPost << endl;
#endif
           codeseg=FP_SEG(aspiPost);                                // get code segment
           dataseg=getDataSeg();                                    // get data segment
#if defined(DEBUG)
           cdbg << "code segment: " << hex << codeseg << endl;
           cdbg << "data segment: " << hex << dataseg << endl;
#endif
           DevVirtToPhys(SRBPtr, &SRBPhys);                         // convert address
           aspiPostAddr=(WORD32)(void _far *)postEntry;
           aspiPostAddr=(aspiPostAddr) + (codeseg << 16);   // build address of post routine
           *((WORD32 FAR*)(((WORD32)SRBPtr)+0x26))=SRBPhys; // insert phys. address of SRB
           *((WORD32 FAR*)(((WORD32)SRBPtr)+0x20))=aspiPostAddr; // insert addr of post routine
           *((WORD16 FAR*)(((WORD32)SRBPtr)+0x24))=dataseg; // insert data segment
#if defined(DEBUG)
           cdbg << "SRBPhys: " << (WORD32)SRBPhys << endl;
#endif
           DATAPtr=*((WORD32 FAR*)(((WORD32)SRBPtr)+0x0F)); // get pointer of data buffer
#if defined(DEBUG)
           cdbg << "DATAPtr= " << hex << DATAPtr << endl;
           DevInt3();
#endif
           *((WORD32 FAR*)(((WORD32)SRBPtr)+0x0F))=DATAPhys; // insert in SRB
#if defined(DEBUG)
//         cdbg << "DATAPhys= " << hex << setw(8) << DATAPhys << setw() << dec << 'h' << endl;
#endif
#if defined(DEBUG)
           DevInt3();
#endif
           callAspi();                                      // call OS2ASPI.DMD (IDC)
           break;
         case SRB_Inquiry:
           SRBPtr = rp->ParmPacket;                                 // get pointer of SRB
                                                                    // added 6.7.97
           callAspi();                                      // call OS2ASPI.DMD (IDC)
           break;
         case SRB_Device:
           SRBPtr = rp->ParmPacket;                                 // get pointer of SRB
                                                                    // added 6.7.97
           callAspi();                                      // call OS2ASPI.DMD (IDC)
           break;
         // Abort added 7.6.97 for version 1.01
         case SRB_Abort:
           while (postInProgress) {}                    // wait for post routine to complete
           SRBPtr = rp->ParmPacket;                                 // get pointer of SRB
                                                                    // added 6.7.97
           SRBPtr2=*((WORD32 FAR*)(((WORD32)SRBPtr)+0x08)); // get address of SRB to abort
#if defined(DEBUG)
           cdbg << "SRB to abort address: " << hex << (WORD32)SRBPtr2 << endl;
#endif
           DevVirtToPhys((BYTE FAR*)SRBPtr2, &SRBPhys2);               // convert address
#if defined(DEBUG)
           cdbg << "SRB to abort address (phys): " << hex << (WORD32)SRBPhys2 << endl;
#endif
                                                            // (assume its locked by DosDevIOCtl)
           *((WORD32 FAR*)(((WORD32)SRBPtr)+0x08))=SRBPhys2; // insert addr of SRB to abort
           callAspi();                                      // call OS2ASPI.DMD (IDC)
           break;
         case SRB_Reset:
           SRBPtr = rp->ParmPacket;                                 // get pointer of SRB
                                                                    // added 6.7.97
           codeseg=FP_SEG(aspiPost);                                // get code segment
           dataseg=getDataSeg();                                    // get data segment
           aspiPostAddr=(WORD32)(void _far *)postEntry;
           aspiPostAddr=(aspiPostAddr) + (codeseg << 16);   // build address of post routine
           *((WORD32 FAR*)(((WORD32)SRBPtr)+0x20))=aspiPostAddr; // insert addr of post routine
           *((WORD16 FAR*)(((WORD32)SRBPtr)+0x24))=dataseg; // insert data segment
           callAspi();                                      // call OS2ASPI.DMD (IDC)
           break;
         default:
           break;
       }
       break;
     case 0x03:
     // This function is to hand over an event semaphore handle to the device driver

       //Get handle of semaphore from app
       *((WORD32 *) &postSema) = *((WORD32 FAR*)rp->ParmPacket);

       //Open Event Semaphore and return error to app
       result = DevEventOpen(postSema);
#if defined(DEBUG)
       cdbg << "DevEventOpen returned with: " << result << endl;
#endif
       *((WORD16 FAR*)rp->DataPacket) = *((WORD16 *) &result);  // return result to app
       break;
     case 0x04:
       //Get buffer pointer from app
       *((WORD32 *) &DATAPtr) = (WORD32)(WORD32 FAR*)rp->ParmPacket;
       bufseg=FP_SEG(DATAPtr);
#if defined(DEBUG)
       cdbg << "Buffer segment: " << hex << bufseg << endl;
       cdbg << "Bufer: " << hex << DATAPtr << endl;
#endif
       result=DevSegLock((SEL)bufseg, 1, 0, &LockHandle);     // lock segment
       *((WORD16 FAR*)rp->DataPacket) = *((WORD16 *) &result);  // return result to app
#if defined(DEBUG)
       if (result)
       {
         cdbg << "DevSegLock failed!" << endl;
       }
       else
       {
         cdbg << "DevSegLock succeeded." << endl;
       }
#endif
       DevVirtToPhys((BYTE FAR*) DATAPtr, &DATAPhys);   // convert ptr to data buffer
#if defined(DEBUG)
       cdbg << "DATAPhys: (hi) " << hex << (DATAPhys >> 16) << endl;
       cdbg << "DATAPhys: (lo) " << hex << (DATAPhys & 0xFFFF) << endl;
       cdbg << "LockHandle: " << hex << (WORD32)LockHandle << endl;
#endif
       break;
     default:
       break;
     }
  return RPDONE; // | RPERR_COMMAND;
  }


