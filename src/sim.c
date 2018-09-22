#include <stdio.h>
#include "defs.h"

void trace (const char *string)
{
  fprintf (stderr, " %s", string);
}

int main (void)
{
  FILE *f;
  int i;

  f = fopen ("rom/image", "rb");
  fread (ROM, 1, 1024, f);
  fclose (f);

  for (i = 0; ; i++) {
    if ((i % 18) == 0)
      fprintf (stderr, "%7d:", i);
    step ();
    // ON 5184, off 59616 ns.  Sum 64800 ns.
    //   900 cycles = 65104 ns = 5208 + 59896
    // OSC = 13.824 MHz
    // One microinstruction 18 cycles = 1302 ns.
    // vert goes on every 16588800 ns, stays on 414720.
    // 13.824 / 9 / 10 / 10 is horizontal frequency = 15.36 kHz
    scan = (i % 900) < 350;
    fly = (i % 900) > 350;
  }

  return 0;
}
