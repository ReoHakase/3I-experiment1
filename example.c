#include <stdio.h>
#include <regex.h>

int main(void){
  const char regex[] = "^(070|080|090)-[0-9]{4}-[0-9]{4}$";
  regex_t preg; char str[20]; regmatch_t pm[1];

  if(regcomp(&preg, regex, REG_EXTENDED|REG_NEWLINE) != 0){
    printf("Regex is not available.\n");
    return 1;
  }

  printf("Enter a phone number: "); scanf("%s", str);
  if(regexec(&preg, str, 1, pm, 0) == 0){
    printf("It is Japanese one.\n");
  }else{
    printf("It belongs to a foreign country.\n");
  }

  regfree(&preg);
  return 0;
}