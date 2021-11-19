#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

struct vcd {
  int width;
  const char *id;
  const char *name;
  int value;
};

long long value_time = -1;

struct vcd variable[] = {
  { 9, "A", "E2_74164", -1 },
  { 4, "B", "E27_74161", -1 },
  { 4, "C", "E18_7490", -1 },
  { 4, "D", "E13_7493", -1 },
  { 4, "E", "E11_7493", -1 },
  { 4, "F", "E5_74161", -1 },
  { 4, "G", "E6_74161", -1 },
  { 1, "H", "horiz", -1 },
  { 1, "I", "scan", -1 },
  { 1, "J", "fly", -1 },
  { 1, "K", "vert", -1 },
  { 1, "L", "tos", -1 }
};

#define VALUES (sizeof variable / sizeof variable[0])

FILE *vcd;

static void vcd_binary (int value, int width)
{
  int mask;
  fputc ('b', vcd);
  for (mask = (1 << (width - 1)); mask != 0; mask >>= 1)
    fputc (value & mask ? '1' : '0', vcd);
}

void vcd_value (int index, int value)
{
  if (variable[index].value == value)
    return;

  if (cycles > value_time) {
    value_time = cycles;
    fprintf (vcd, "#%lld\n", (long long)(1e9 * value_time / 13.824e6));
  }

  variable[index].value = value;
  if (variable[index].width == 1)
    fprintf (vcd, "%d%s\n", value, variable[index].id);
  else {
    vcd_binary (value, variable[index].width);
    fprintf (vcd, " %s\n", variable[index].id);
  }
}

void vcd_start (const char *file)
{
  int i;

  vcd = fopen (file, "w");
  if (vcd == NULL) {
    fprintf (stderr, "Error opening %s\n", file);
    exit (1);
  }

  fprintf (vcd, "$version VT52 simulator $end\n");
  fprintf (vcd, "$timescale 1ns $end\n");
  for (i = 0; i < VALUES; i++) {
    fprintf (vcd, "$var wire %d %s %s",
             variable[i].width,
             variable[i].id,
             variable[i].name);
    if (variable[i].width > 1)
      fprintf (vcd, " [%d:0]", variable[i].width - 1);
    fprintf (vcd, " $end\n");
  }
  fprintf (vcd, "$enddefinitions $end\n");
  fprintf (vcd, "$dumpvars\n");
  fflush (vcd);
}
