/* abort example */
#include <stdio.h>      /* fopen, fputs, fclose, stderr */
#include <stdlib.h>     /* abort, NULL */

int main ()
{
  FILE *pFile;
  pFile= fopen ("myfile.txt","r");
  if (pFile == NULL)
  {
    fputs ("error opening file\n",stderr);
    abort();
  }

  /* regular process here */

  fclose (pFile);
  return 0;
}