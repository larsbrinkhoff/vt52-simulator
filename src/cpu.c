#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* VT52 simulator. */

unsigned int PC;		/* 10 bits */
unsigned char IR;		/* 8 bits */
unsigned char A;		/* 7 bits */
unsigned char B;		/* */
unsigned char X;		/* 7 bits */
unsigned char Y;		/* 5 bits */
unsigned char R;		/* 2 bits */
unsigned char U;		/* */
unsigned char M[2048];		/* 2048x7 */
unsigned char ROM[1024];	/* 1024x8 */
unsigned char CHAR[1024];	/* 1024x8 */
unsigned char MB;		/* 7 bits */
unsigned char key[100];	
unsigned char VSR;		/* 7 bits */

/* Flip-flops. */
int cursor;
int video;
int tos;
int scan;
int hz60 = 1;
int bell;
int done;
int write;
int fly;

void trace (const char *);
static unsigned int address (void);
static void target (unsigned int a);
static void TJ_cycle_mode0 (void);
static void TJ_cycle_mode1 (void);
static void (*TJ_cycle) (void);
#include "defs.h"

static void (*constant) (void);

#define ADDR address ()
#define RAM M[ADDR]

#define INCREMENT_PC                            \
  PC++;                                         \
  PC &= 01777;                                  \
  if ((PC & 0377) == 0)                         \
    R = (R + 1) & 3

#define JUMP(COND)                              \
  a = ROM[PC];                                  \
  INCREMENT_PC;                                 \
  target (a | (R << 8));			\
  if (COND)                                     \
    PC = a | (R << 8)

static void constant_mode0 (void)
{
  char info[20];
  sprintf (info, ".LD %03o ;mode 0", IR & 0177);
  trace (info);
  A++;
  A &= 0177;
  if (A >= RAM) {
    RAM = IR & 0x7F;
    done = 1;
  }
}

static void constant_mode1 (void)
{
  char info[20];
  sprintf (info, ".LD %03o", IR & 0177);
  trace (info);
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
    X &= 0177;
    Y &= 037;
    break;
  case 0x38:
    trace ("IX");
    X++;
    X &= 0177;
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
    X &= 0177;
    Y &= 037;
    break;
  case 0x14:
    trace ("IA");
    A++;
    A &= 0177;
    break;
  case 0x24:
    trace ("IA1");
    A++;
    A &= 0177;
    break;
  case 0x34:
    trace ("IY");
    Y++;
    Y &= 037;
    break;
  case 0x44:
    trace ("DY");
    Y--;
    Y &= 037;
    break;
  case 0x54:
    trace ("IROM");
    R++;
    R &= 3;
    break;
  case 0x64:
    trace ("DX");
    X--;
    X &= 0177;
    break;
  case 0x74:
    trace ("DA");
    A--;
    A &= 0177;
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
    Y = B & 037;
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
  default:
    if (IR & 0x80)
      constant();
    break;
  }
}

static void TG_cycle (void)
{
  write = 0;
}

static void TH_cycle (void)
{
  INCREMENT_PC;
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
  int a;

  switch (IR & 0xF1) {
  case 0x01:
    trace ("PSCJ");
    JUMP ((rand () & 1) == 0); //Printer scan.
    break;
  case 0x11:
    trace ("TABJ");
    JUMP ((A & 7) == 7);
    break;
  case 0x21:
    trace ("KCLJ");
    JUMP ((rand () & 1) == 0); //Key click.
    break;
  case 0x31:
    trace ("FRQJ");
    JUMP (hz60);
    break;
  case 0x41:
    trace ("PRQJ");
    JUMP ((rand () & 1) == 0); //Printer request.
    break;
  case 0x51:
    trace ("?");
    exit (1);
    break;
  case 0x61:
    trace ("UTJ");
    JUMP ((rand () & 1) == 0); //UART transmitting.
    break;
  case 0x71:
    tos = (rand () & 0xF) == 0;
    trace ("TOSJ");
    JUMP (tos);
    break;
  }
}

static void TJ_cycle_mode1 (void)
{
  int a;

  switch (IR & 0xF1) {
  case 0x01:
    trace ("URJ");
    JUMP ((rand () & 0xF) == 0); //UART received character.
    break;
  case 0x11:
  case 0x41:
    trace ("AEMJ");
    JUMP (A == RAM);
    break;
  case 0x21:
    trace ("ALMJ");
    JUMP (A < RAM);
    break;
  case 0x31:
    trace ("ADXJ");
    JUMP (A != X);
    break;
  case 0x51:
    trace ("TRUJ");
    JUMP (1);
    break;
  case 0x61:
    trace ("VSCJ");
    JUMP (scan);
    break;
  case 0x71:
    trace ("KEYJ");
    JUMP (key[A]);
    else
      A--;
    A &= 0177;
    break;
  }
}

static void T2_cycle (void)
{
  char info[20];
  VSR = CHAR[(MB << 3) + (A & 7)];
  MB = RAM;
  if (!video || fly)
    return;
  sprintf (info, "VID(%02d,%02d)", X, Y);
  //trace (info);
  X++;
  X &= 0177;
}

static void newline (void)
{
  char info[40];
  sprintf (info, "[A/%03o X/%02o Y/%02o %04o:%03o]\n", A, X, Y, ADDR, RAM);
  trace (info);
}

static void target (unsigned int a)
{
  char info[20];
  sprintf (info, "%04o", a);
  trace (info);
}

static unsigned int address (void)
{
  int x = X, y = Y >> 1;

  if (x >= 64 || y >= 12) {
    x = x & 017;
    x |= (y & 014) << 2;
    y |= 014;
  }

  /* Least significant bit of Y selects RAM page. */
  if (Y & 1)
    x |= 02000;

  return x | (y << 6);
}

static int t = 0;

void step (void)
{
  char info[20];

  if (t == 0) {
    IR = ROM[PC];
    sprintf (info, "%04o/ %03o >>", PC, IR);
    trace (info);
  }

  switch (t)
    {
    case 0:
      T2_cycle ();
      break;
    case 1:
      TE_cycle ();
      break;
    case 2:
      TF_cycle ();
      break;
    case 7:
      TW_cycle ();
      break;
    case 9:
      T2_cycle ();
      break;
    case 12:
      TG_cycle ();
      break;
    case 13:
      TH_cycle ();
      break;
    case 15:
      TJ_cycle ();
      newline ();
      break;
    }

  t++;
  if (t == 18)
    t = 0;
}
