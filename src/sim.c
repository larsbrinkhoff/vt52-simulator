#include <stdio.h>
#include "defs.h"

long long cycles;

void trace (const char *string)
{
  fprintf (stderr, " %s", string);
}

int main (void)
{
  FILE *f;

  f = fopen ("rom/image", "rb");
  fread (ROM, 1, 1024, f);
  fclose (f);

  vcd_start ("dump.vcd");

  for (cycles = 0; ; cycles++) {
    if ((cycles % 18) == 0)
      fprintf (stderr, "%7lld/%9.3fms:", cycles, 1e3 * cycles / 13.824e6);
    timing ();
    step ();
  }

  return 0;
}
