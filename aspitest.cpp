#include "aspilib.h"
#include <stdio.h>
#include <conio.h>
#include <ibase.hpp>
#include <iapp.hpp>                             // IApplication class
#include <ithread.hpp>

scsiObj  aspi;

void _Optlink abtThread(void*)
{
  ULONG result;

  DosWaitEventSem(-1, aspi.Sema);
  printf("rewind() started\n");
  getch();
  result=aspi.abort();
  printf("abort result: %ld\n", result);
  printf("abort status: %ld\n", aspi.AbortSRB.status);
}

void main()
{
  FILE*    fhandle;
  ULONG    result, rc;
  ULONG    pos, partition;
  BOOL     BOP, EOP;
  int i;

  rc = DosCreateEventSem(NULL, &aspi.Sema, DC_SEM_SHARED, 0);
  if (rc) printf("DosCreateEventSem failed, error code = %ld\n", rc);

  if (aspi.init(65536))
    printf("Initializing ASPI library successful.\n");
  else
    printf("Initializing failed.\n");

  do
  {
    result=aspi.testUnitReady(6,0);
    printf("\nresult=%ld\n", result);
    printf("CDB+Sense: \n");
    for (i=0; i<64; i++) printf("%x ",aspi.SRBlock.u.cmd.cdb_st[i]);
    printf("\nStatus: %ld\n",aspi.SRBlock.status);
    printf("HA-Status: %ld\n", aspi.SRBlock.u.cmd.ha_status);
    printf("target status: %ld\n", aspi.SRBlock.u.cmd.target_status);
    getch();
  }
  while (result!=0);

  IThread abt(abtThread, NULL, FALSE);

  result=aspi.rewind(6, 0);
  if (result==0)
    printf("rewind() successful.\n");
  else
    printf("rewind() unsuccessful %ld.\n", result);

  printf("\nresult=%ld\n", result);
  printf("CDB+Sense: \n");
  for (i=0; i<64; i++) printf("%x ",aspi.SRBlock.u.cmd.cdb_st[i]);
  printf("\nStatus: %ld\n",aspi.SRBlock.status);
  printf("HA-Status: %ld\n", aspi.SRBlock.u.cmd.ha_status);
  printf("target status: %ld\n", aspi.SRBlock.u.cmd.target_status);
  getch();

/*  fhandle=fopen("test","rb");
  ULONG actRead;
  ULONG count;
//  do
  {
    actRead=fread((void*) aspi.buffer, 512, 128, fhandle);
    count=actRead;
//    if (actRead % 512) count++;
    result=aspi.write(6,0,count);
    printf("write %ld blocks ", count);
    if (result==0)
    {
      printf("ok\n");
      i++;
    }
  }
//  while ((result==0) && (count==128));
  printf("\nresult=%ld\n", result);

  printf("read %d blocks\n", i);
  printf("CDB+Sense: \n");
  for (i=0; i<64; i++) printf("%x ",aspi.SRBlock.u.cmd.cdb_st[i]);
  printf("Status: %ld\n",aspi.SRBlock.status);
  printf("HA-Status: %ld\n", aspi.SRBlock.u.cmd.ha_status);
  printf("target status: %ld\n", aspi.SRBlock.u.cmd.target_status);
  fclose(fhandle);

  result=aspi.read_position(6, 0, &pos, &partition, &BOP, &EOP);
  printf("result=%ld\n", result);
  printf("CDB+Sense: \n");
  for (i=0; i<64; i++) printf("%x ",aspi.SRBlock.u.cmd.cdb_st[i]);
  printf("\nStatus: %ld\n",aspi.SRBlock.status);
  printf("HA-Status: %ld\n", aspi.SRBlock.u.cmd.ha_status);
  printf("target status: %ld\n", aspi.SRBlock.u.cmd.target_status);
  printf("pos: %ld\n", pos);
  printf("partition: %d\n", partition);
  printf("BOP %d  EOP %d\n", BOP, EOP);

  result=aspi.resetDevice(6,0);
  printf("result=%ld\n", result);*/

//  result=aspi.unload(6,0);
//  printf("result=%ld\n", result);

  if (aspi.close())
    printf("Closing library successful.\n");
  else
    printf("Closing library unsuccessful.\n");
}

