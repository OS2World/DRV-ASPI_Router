#include <devhelp.h>
#include <devdebug.h>
#include <devtype.h>
#include <devrp.h>

void aspiPost(WORD32 SRBPointer);

extern void __far aspiPostEntry(WORD16, WORD32);

#pragma aux aspiPostEntry = \
  "push bp" \
  "mov  bp,sp" \
  "pusha" \
  "mov  ds,[bp+6]" \
  "push dword ptr [bp+8]" \
  "call aspiPost" \
  "add  sp,4" \
  "popa" \
  "pop bp" \
  "ret";


