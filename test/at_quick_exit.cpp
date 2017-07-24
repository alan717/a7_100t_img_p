/* at_quick_exit example */
#include <stdio.h>      /* puts */
#include <stdlib.h>     /* at_quick_exit, quick_exit, EXIT_SUCCESS */

void fnQExit (void)
{
  puts ("Quick exit function.");
}

int main ()
{
  at_quick_exit (fnQExit);
  puts ("Main function: Beginning");
  quick_exit (EXIT_SUCCESS);
  puts ("Main function: End");  // never executed
  return 0;
}