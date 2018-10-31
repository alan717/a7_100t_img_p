/* atoll example */
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atoll */

int main ()
{
  long long int lli;
  char buffer[256];
  printf ("Enter a long number: ");
  fgets (buffer, 256, stdin);
  lli = atoll(buffer);
  printf ("The value entered is %lld. Its double is %lld.\n",lli,lli*2);
  return 0;
}