#include <regex.h>
#include <stdio.h>

#define GROUP_NUMBER 5

int main(void)
{
  // const char regex[] = "(070|080|090)-[0-9]{4}-[0-9]{4}";
  const char regex[] = "(0[7,8,9]0)-([0-9]{3,4})-([0-9]{4,5})";
  regex_t preg;
  char str[40] = "070-1323-4500 090-9684-8271";
  regmatch_t pm[GROUP_NUMBER];

  if (regcomp(&preg, regex, REG_EXTENDED | REG_NEWLINE) != 0)
  {
    printf("Regex is not available.\n");
    return 1;
  }

  // printf("Enter a phone number: "); scanf("%s", str);
  if (regexec(&preg, &str[13], GROUP_NUMBER, pm, 0) == 0)
  {
    printf("It is Japanese one.\n");
    for (int i = 0; i < GROUP_NUMBER; i++)
    {
      printf("%d -> %d\n", pm[i].rm_so, pm[i].rm_eo);
    }
  }
  else
  {
    printf("It belongs to a foreign country.\n");
  }

  regfree(&preg);
  return 0;
}