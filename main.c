#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_FILEHANDLE -1
#define ERROR_REGEX -2

#define STR_LENGTH 1024
#define PHONENUMBER_LENGTH 16
#define LINE_LENGTH 128
#define PHONENUMBER_COUNT 8

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

int findMatches(char *regex, char *source, char (*dest)[PHONENUMBER_LENGTH])
{
  // REFER:
  // https://www.boost.org/doc/libs/1_60_0/libs/regex/doc/html/boost_regex/syntax/basic_extended.html

  static const unsigned int maxMatches = PHONENUMBER_COUNT;
  static const unsigned int maxSubGroups = 3;

  regex_t preg;
  regmatch_t resultGroups[maxMatches];
  int matchCount = 0;

  printf("maxMatches: %d\n", maxMatches);
  if (regcomp(&preg, regex, REG_EXTENDED | REG_NEWLINE) != 0)
  {
    return ERROR_REGEX;
  }

  // unsigned int offset = 0;
  char *cursor = source, *nextCursor;
  for (unsigned int match = 0; match < maxMatches; match++)
  {
    // 正規表現に該当する部分の数だけ、くりかえす
    // 文字列sourceのoffset以降の部分文字列に正規表現を実行
    if (regexec(&preg, cursor, (size_t)maxMatches, resultGroups, 0) != 0)
    {
      break; // 当該部分がみつからなかったら、終了
    }
    else
    {
      // 当該部分が見つかったら、
      matchCount++;
      for (unsigned int group = 0; group < 1 + maxSubGroups; group++)
      {
        // printf("Offset: %3u\n", offset);
        unsigned int startIndex = resultGroups[group].rm_so;
        unsigned int endIndex = resultGroups[group].rm_eo;
        if (resultGroups[group].rm_so == -1)
        {
          break; // 当該部分内の、グループが存在しなかったら終了
        }
        else
        {
          char matchedText[STR_LENGTH] = "";
          strncpy(matchedText, &cursor[startIndex],
                  endIndex - startIndex);
          if (group == 0)
          {
            // 当該部分全体を表すグループの場合
            printf("%2u 個目の当該部分: [%2u -> %2u] %s\n", match + 1, startIndex, endIndex, matchedText);
            strcpy(dest[match], matchedText);
            // offset = endIndex;
            nextCursor = cursor + endIndex; // 正規表現を実行する範囲をこの当該部分以降にする
          }
          else
          {
            // 当該部分内の一部分を指すグループの場合
            printf("| %2u 個目のグループ: [%2u -> %2u] %s\n", group, startIndex, endIndex, matchedText);
          }
        }
      }
    }
    cursor = nextCursor;
  }

  regfree(&preg);
  return matchCount;
}

int main(int argc, char **argv)
{
  char *fileName = argv[1];
  char text[STR_LENGTH] = "";
  char phoneNumbers[PHONENUMBER_COUNT][16];
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
  int count = findMatches(regex, text, phoneNumbers);
  if (count == ERROR_REGEX)
  {
    printf("正規表現は利用不可能です。\n");
    return 1;
  }
  for (int i = 0; i < count; i++)
  {
    printf("[%d件目] %s\n", i + 1, phoneNumbers[i]);
  }
  return 0;
}