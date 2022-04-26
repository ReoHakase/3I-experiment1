#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc.h"

int readTextFile(char *dest, char *fileName)
{
  FILE *filePointer;
  filePointer = fopen(fileName, "r");
  if (filePointer == NULL)
    return ERROR_FILEHANDLE;

  // destに収まることができる最小の行数分だけくりかえす
  for (int i = 0; i < (STR_LENGTH / LINE_LENGTH); i++)
  {
    // printf("%d of %d -th reading**\n", i, (STRLEN / CHARSPERLINE));
    char currentLine[LINE_LENGTH];
    if (fgets(currentLine, LINE_LENGTH, filePointer) == NULL)
    {
      break;
    }
    else
    {
      strcat(dest, currentLine);
    }
  }

  if (fclose(filePointer) == EOF)
  {
    return ERROR_FILEHANDLE;
  }
  else
  {
    return 0;
  }
}
