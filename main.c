#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc.h"

void listMatches(char *regexText, char *source)
{
  char matches[MAX_MATCHES][MAX_GROUPS][MATCHED_TEXT_LENGTH];
  int entries = findMatches(regexText, source, MAX_MATCHES, matches);
  if (entries == ERROR_REGEX)
  {
    printf("正規表現は利用不可能です。\n");
  }
  else
  {
    for (int i = 0; i < entries; i++)
    {
      printf("[%d件目] %s\n", i + 1, matches[i][0]);
    }
  }
}

int main(int argc, char **argv)
{
  char *fileName = argv[1];
  char text[STR_LENGTH] = "";
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

  printf("含まれていた日本の携帯電話番号: \n");
  listMatches("(0[7,8,9]0)-([[:digit:]]{3,4})-([[:digit:]]{4,5})", text);

  printf("含まれていたメールアドレス: \n");
  listMatches("[a-zA-Z0-9.\\-]+@[a-zA-Z0-9.\\-]+\\.[a-zA-Z]+", text);

  return 0;
}