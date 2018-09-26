#include <stdio.h>
#include "defs.h"

/* VT52 simulator. */

unsigned int PC;
unsigned char IR;
unsigned char A;
unsigned char B;
unsigned char X;
unsigned char Y;
unsigned char R;
unsigned char U;
unsigned char M[2048];
unsigned char ROM[1024];
unsigned char MB;
unsigned char key[100];

int cursor;
int video;
int bell;
int done;
int write;

void trace (const char *);
static void TJ_cycle_mode0 (void);
static void TJ_cycle_mode1 (void);
static void (*TJ_cycle) (void);
#include "defs.h"

static void (*constant) (void);

#define RAM M[(X << 8) + Y]

static void constant_mode0 (void)
{
  trace ("CONSTANT0");
  A++;
  if (A >= RAM) {
    RAM = IR & 0x7F;
    done = 1;
  }
}

static void constant_mode1 (void)
{
  trace ("CONSTANT1");
  RAM = IR & 0x7F;
}

static void TE_cycle (void)
{
  switch (IR & 0xF8) {
  case 0x08:
    trace ("ZXZY");
    X = Y = 0;
    break;
  case 0x18:
    trace ("X8");
    X ^= 8;
    break;
  case 0x28:
    trace ("IXDY"); //Or IADY?
    X++;
    Y--;
    break;
  case 0x38:
    trace ("IX");
    X++;
    break;
  case 0x48:
    trace ("ZA");
    A = 0;
    break;
  case 0x58:
    trace ("M1");
    constant = constant_mode1;
    TJ_cycle = TJ_cycle_mode1;
    break;
  case 0x68:
    trace ("ZX"); //Or 2Z?
    X = 0;
    break;
  case 0x78:
    trace ("M0");
    constant = constant_mode0;
    TJ_cycle = TJ_cycle_mode0;
    break;
  }
}

static void TF_cycle (void)
{
  switch (IR & 0xF4) {
  case 0x04:
    trace ("DXDY");
    X--;
    Y--;
    break;
  case 0x14:
    trace ("IA");
    A++;
    break;
  case 0x24:
    trace ("IA1");
    A++;
    break;
  case 0x34:
    trace ("IY");
    Y++;
    break;
  case 0x44:
    trace ("DY");
    Y--;
    break;
  case 0x54:
    trace ("IROM");
    R++;
    break;
  case 0x64:
    trace ("DX");
    X--;
    break;
  case 0x74:
    trace ("DA");
    A--;
    break;
  }
}

static void TW_cycle (void)
{
  switch (IR & 0xFF) {
  case 0x00:
    trace ("SCFF");
    cursor = 1;
    break;
  case 0x10:
    trace ("SVID");
    video = 1;
    break;
  case 0x20:
    trace ("B2Y");
    //
    break;
  case 0x30:
    trace ("CBFF");
    bell ^= 1;
    break;
  case 0x40:
    trace ("ZCAV");
    cursor = video = 0;
    break;
  case 0x50:
    trace ("LPB");
    break;
  case 0x60:
    trace ("EPR");
    break;
  case 0x70:
    trace ("HPR!ZY");
    Y = 0;
    break;
  }
}

static void TG_cycle (void)
{
  write = 0;
}

static void TH_cycle (void)
{
  switch (IR & 0xF2) {
  case 0x02:
    trace ("M2A");
    A = RAM;
    break;
  case 0x12:
    trace ("A2M");
    RAM = A;
    break;
  case 0x22:
    trace ("M2U");
    U = RAM;
    break;
  case 0x32:
    trace ("B2M");
    RAM = B;
    break;
  case 0x42:
    trace ("M2X");
    X = RAM;
    break;
  case 0x52:
    trace ("U2M");
    RAM = U;
    break;
  case 0x62:
    trace ("M2B");
    B = RAM;
    break;
  case 0x72: 
    trace ("SPARE");
    break;
  }
}

static void TJ_cycle_mode0 (void)
{
  PC++;
  switch (IR & 0xF1) {
  case 0x01:
    trace ("PSCJ");
    break;
  case 0x11:
    trace ("TABJ");
    break;
  case 0x21:
    trace ("KCLJ");
    break;
  case 0x31:
    trace ("FRQJ");
    break;
  case 0x41:
    trace ("PRQJ");
    break;
  case 0x51:
    trace ("TRUJ"); //Or COPJ?
    break;
  case 0x61:
    trace ("UTJ");
    break;
  case 0x71:
    trace ("TOSJ");
    break;
  }
}

static void TJ_cycle_mode1 (void)
{
  int a;

  PC++;
  switch (IR & 0xF1) {
  case 0x01:
    trace ("URJ");
    break;
  case 0x11:
    trace ("AEMJ");
    break;
  case 0x21:
    trace ("ALMJ");
    a = ROM[PC++];
    if (A < RAM)
      PC = a;
    break;
  case 0x31:
    trace ("ADXJ");
    a = ROM[PC++];
    if (A != X)
      PC = a;
    break;
  case 0x41:
    trace ("AEM2J");
    break;
  case 0x51:
    trace ("");
    break;
  case 0x61:
    trace ("VSCJ");
    break;
  case 0x71:
    trace ("KEYJ");
    if (key[A])
      PC = 0;
    else
      A--;
    break;
  }
}

static void T2_cycle (void)
{
  MB = RAM;
  X++;
}

static void newline (void)
{
  char info[20];
  sprintf (info, "[%02X %02X %02X %03X %02X]\n", A, X, Y, (X << 8) + Y, RAM);
  trace (info);
}

void step (void)
{
  char info[20];

  IR = ROM[PC];
  
  sprintf (info, "%03X/ %02X >>", PC, IR);
  trace (info);

  if (IR & 0x80) {
    constant();
    PC++;
    return newline ();
  }

  TE_cycle ();
  TF_cycle ();
  //T2_cycle ();
  TW_cycle ();
  TG_cycle ();
  TH_cycle ();
  TJ_cycle ();

  return newline ();
}
