/* VT52 simulator. */

unsigned int PC;
unsigned char IR;
unsigned char A;
unsigned char B;
unsigned char X;
unsigned char Y;
unsigned char R;
unsigned char U;
unsigned char M[256];

int cursor;
int video;
int bell;

void trace (const char *);
static void TH_cycle_mode0 (void);
static void TH_cycle_mode1 (void);
static void (*TH_cycle) (void);

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
    trace ("IADY"); //Or IXDY?
    A++;
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
    TH_cycle = TH_cycle_mode1;
    break;
  case 0x68:
    trace ("ZX");
    X = 0;
    break;
  case 0x78:
    trace ("M0");
    TH_cycle = TH_cycle_mode0;
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
  switch (IR & 0xF2) {
  case 0x02:
    trace ("M2A");
    A = M[0];
    break;
  case 0x12:
    trace ("A2M");
    M[0] = A;
    break;
  case 0x22:
    trace ("M2U");
    U = M[0];
    break;
  case 0x32:
    trace ("B2M");
    M[0] = B;
    break;
  case 0x42:
    trace ("M2X");
    X = M[0];
    break;
  case 0x52:
    trace ("U2M");
    M[0] = U;
    break;
  case 0x62:
    trace ("M2B");
    B = M[0];
    break;
  case 0x72: 
    trace ("SPARE");
    break;
  }
}

static void TH_cycle_mode0 (void)
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

static void TH_cycle_mode1 (void)
{
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
    break;
  case 0x31:
    trace ("ADXJ");
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
    break;
  }
}

static void TJ_cycle (void)
{
  /* Jump. */
}

static void step (void)
{
  TE_cycle ();
  TF_cycle ();
  TW_cycle ();
  TG_cycle ();
  TH_cycle ();
  TJ_cycle ();
}
