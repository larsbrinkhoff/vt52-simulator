/* VT52 simulator. */

typedef enum {
  TE,
  TF,
  TW,
  TG,
  TH,
  TJ
} cycle_t;

void TE_cycle (void)
{
  switch (IR & 0xF8) {
  case 0x08:
    log ("ZXZY");
    break;
  case 0x18:
    log ("X8");
    break;
  case 0x28:
    log ("IXDY");
    break;
  case 0x38:
    log ("IX");
    break;
  case 0x48:
    log ("ZA");
    break;
  case 0x58:
    log ("M1");
    break;
  case 0x68:
    log ("ZX");
    break;
  case 0x78:
    log ("M0");
    break;
  }
}

void TF_cycle (void)
{
  switch (IR & 0xF4) {
  case 0x04:
    log ("DZDY");
    break;
  case 0x14:
    log ("IA");
    break;
  case 0x24:
    log ("IA1");
    break;
  case 0x34:
    log ("IY");
    break;
  case 0x44:
    log ("DY");
    break;
  case 0x54:
    log ("IROM");
    break;
  case 0x64:
    log ("DX");
    break;
  case 0x74:
    log ("DA");
    break;
  }
}

void TW_cycle (void)
{
  switch (IR & 0xFF) {
  case 0x00:
    log ("SCFF");
    break;
  case 0x10:
    log ("SVID");
    break;
  case 0x20:
    log ("B2Y");
    break;
  case 0x30:
    log ("CBFF");
    break;
  case 0x40:
    log ("ZCAV");
    break;
  case 0x50:
    log ("LPB");
    break;
  case 0x60:
    log ("EPR");
    break;
  case 0x70:
    log ("HPRZY");
    break;
  }
}

void TG_cycle (void)
{
  switch (IR & 0xF2) {
  case 0x02:
    log ("M2A");
    break;
  case 0x12:
    log ("A2M");
    break;
  case 0x22:
    log ("M2U");
    break;
  case 0x32:
    log ("B2M");
    break;
  case 0x42:
    log ("M2X");
    break;
  case 0x52:
    log ("U2M");
    break;
  case 0x62:
    log ("M2B");
    break;
  case 0x72: 
    log ("(spare)");
    break;
  }
}

void TH_cycle_mode0 (void)
{
  PC++;
  switch (IR & 0xF1) {
  case 0x01:
    log ("PSCJ");
    break;
  case 0x11:
    log ("TABJ");
    break;
  case 0x21:
    log ("KCLJ");
    break;
  case 0x31:
    log ("FRQJ");
    break;
  case 0x41:
    log ("PRQJ");
    break;
  case 0x51:
    log ("COPJ");
    break;
  case 0x61:
    log ("UTJ");
    break;
  case 0x71:
    log ("TOSJ");
    break;
  }
}

void TH_cycle_mode1 (void)
{
  PC++;
  switch (IR & 0xF1) {
  case 0x01:
    log ("URJ");
    break;
  case 0x11:
    log ("AEMJ");
    break;
  case 0x21:
    log ("ALMJ");
    break;
  case 0x31:
    log ("ADXJ");
    break;
  case 0x41:
    log ("AEM2J");
    break;
  case 0x51:
    log ("TRUJ");
    break;
  case 0x61:
    log ("VSCJ");
    break;
  case 0x71:
    log ("KEYJ");
    break;
  }
}

void TJ_cycle (void)
{
  /* Jump. */
}
