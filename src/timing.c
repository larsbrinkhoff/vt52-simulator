#include "defs.h"

static unsigned int shifter = 0;   //E2  74164
static unsigned int counter1 = 0;  //E27 74161
static unsigned int counter2 = 0;  //E18 7490
static unsigned int counter3 = 0;  //E13 7493
static unsigned int counter4 = 0;  //E11 7493
static unsigned int counter5 = 0;  //E5  74161
// counter6 E7 74161

void timing (void)
{
  int old_horiz;

  if (shifter != 0377)
    shifter = (shifter << 1) | 1;
  else
    shifter <<= 1;
  shifter &= 0377;

  if (shifter == 0177) {
    counter1++;
  }

  if (counter1 == 10) {
    counter1 = 0;
    counter2++;
    counter3++;
  }

  if (counter2 == 5)
    counter2 = 8;
  else if (counter2 == 13)
    counter2 = 0;
  // 0000  1000
  // 0001  1001
  // 0010  1010
  // 0011  1011
  // 0100  1100

  if (counter3 == 16) {
    counter3 = 0;
    counter4++;
  }

  if (counter4 == 16) {
    counter4 = 0;
    counter5++;
  }

  if (counter5 == 16) {
    counter5 = 6; // 60Hz: 6, 50Hz: 4
  }

  old_horiz = horiz;
  horiz = (counter2 & 010) != 0;

  if (counter1 & 010)
    scan = 0;
  if (old_horiz && !horiz)
    scan = 1;

  if (counter1 & 2) {
    if (scan)
      fly = 1;
    if (counter2 & 2)
      fly = 0;
  }

  vert = (counter4 & 014) == 010 && counter5 == 017;

  if (counter1 & 010)
    tos = !1;
  if (counter5 == 0)
    tos = !0;

  vcd_value (INDEX_E2_74164,  shifter);
  vcd_value (INDEX_E27_74161, counter1);
  vcd_value (INDEX_E18_7490,  counter2);
  vcd_value (INDEX_E13_7493,  counter3);
  vcd_value (INDEX_E11_7493,  counter4);
  vcd_value (INDEX_E5_74161,  counter5);
  //vcd_value (INDEX_E6_74161, );
  vcd_value (INDEX_HORIZ,     horiz);
  vcd_value (INDEX_SCAN,      scan);
  vcd_value (INDEX_FLY,       fly);
  vcd_value (INDEX_VERT,      vert);
  vcd_value (INDEX_TOS,       tos);
}
