#include <stdio.h>
#include "defs.h"

void address (int address)
{
  fprintf (stderr, "%03X: ", address);
}

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

  for (i = 0; i < 1000; i++)
    step ();

  return 0;
}
