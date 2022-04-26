#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc.h"

int main(int argc, char **argv)
{
  char *fileName = argv[1];
  char text[STR_LENGTH] = "";
  char matches[MAX_MATCHES][MAX_GROUPS][MATCHED_TEXT_LENGTH];
  if (argc < 2)
  {
    printf("対象のファイル名を引数に明記してください。\n");
    return 1;
  }
  printf("ファイル %s から読み込み開始: \n", fileName);
  if (readTextFile(text, fileName) == ERROR_FILEHANDLE)
  {
    printf("読み込みに失敗しました。\n");
    return 1;
  }
  printf(
      "抽出対象テキスト: ==========\n%s\n=================================\n",
      text);
  printf("結果: \n");
  // char *regex = "(0[7,8,9]0)-([0-9]{3,4})-([0-9]{4,5})";
  // const char regex[] = "0[7,8,9]0-\\d{3,4}-\\d{4,5}";
  char regex[] = "(0[7,8,9]0)-([[:digit:]]{3,4})-([[:digit:]]{4,5})";
  // const char regex[] = "0[7,8,9]0-[0-9]{3,4}-[0-9]{4,5}";
  // char *regex = "代表";
  int count = findMatches(regex, text, MAX_MATCHES, matches);
  if (count == ERROR_REGEX)
  {
    printf("正規表現は利用不可能です。\n");
    return 1;
  }
  for (int i = 0; i < count; i++)
  {
    printf("[%d件目] %s\n", i + 1, matches[i][0]);
  }
  return 0;
}