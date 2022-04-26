#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "inc.h"

int findMatches(char *regexText, char *source, unsigned int maxMatches, char (*dest)[MAX_GROUPS][MATCHED_TEXT_LENGTH])
{
  // REFER:
  // https://www.boost.org/doc/libs/1_60_0/libs/regex/doc/html/boost_regex/syntax/basic_extended.html

  // static const unsigned int maxMatches = PHONENUMBER_COUNT;
  // static const unsigned int maxSubGroups = 3;

  regex_t preg;
  regmatch_t resultGroups[maxMatches];
  int matchCount = 0;

  // printf("maxMatches: %d\n", maxMatches);
  if (regcomp(&preg, regexText, REG_EXTENDED | REG_NEWLINE) != 0)
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
      for (unsigned int group = 0; group < MAX_GROUPS; group++)
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
          strcpy(dest[match][group], matchedText);
          if (group == 0)
          {
            // 当該部分全体を表すグループの場合
            // printf("%2u 個目の当該部分 (%2u): [%2u -> %2u] %s\n", match + 1, group, startIndex, endIndex, matchedText);
            // offset = endIndex;
            nextCursor = cursor + endIndex; // 正規表現を実行する範囲をこの当該部分以降にする
          }
          else
          {
            // 当該部分内の一部分を指すグループの場合
            // printf("| %2u 個目のグループ: [%2u -> %2u] %s\n", group, startIndex, endIndex, matchedText);
          }
        }
      }
    }
    cursor = nextCursor;
  }

  regfree(&preg);
  return matchCount;
}