#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define ERROR_FILEHANDLE -1
#define ERROR_REGEX -2

#define STR_LENGTH 1024
#define PHONENUMBER_LENGTH 16
#define LINE_LENGTH 128
#define PHONENUMBER_COUNT 8

int readTextFile(char *dest, char *fileName){
  FILE *filePointer;
  filePointer = fopen(fileName, "r");
  if(filePointer == NULL) return ERROR_FILEHANDLE;

  // destに収まることができる最小の行数分だけくりかえす
  for(int i = 0; i < (STR_LENGTH / LINE_LENGTH); i++){
    // printf("%d of %d -th reading**\n", i, (STRLEN / CHARSPERLINE));
    char currentLine[LINE_LENGTH];
    if(fgets(currentLine, LINE_LENGTH, filePointer) == NULL){
      break;
    }else{
      strcat(dest, currentLine);
    }
  }

  if(fclose(filePointer) == EOF){
    return ERROR_FILEHANDLE;
  }else{
    return 0;
  }
}



int findMatches(char *regex, char *source, char (*dest)[PHONENUMBER_LENGTH], int maxMatches){
    // REFER: https://www.boost.org/doc/libs/1_60_0/libs/regex/doc/html/boost_regex/syntax/basic_extended.html

  // const char regex[] = "0[7,8,9]0-\\d{3,4}-\\d{4,5}";
  // const char regex[] = "0[7,8,9]0-[[:digit:]]{3,4}-[[:digit:]]{4,5}";
  // const char regex[] = "0[7,8,9]0-[0-9]{3,4}-[0-9]{4,5}";
  regex_t preg; 
  regmatch_t matchResults[maxMatches];
  int matchCount = 0;

  printf("maxMatches: %d\n", maxMatches);
  if(regcomp(&preg, regex, REG_EXTENDED|REG_NEWLINE) != 0){
    return ERROR_REGEX;
  }

  // char *searchTarget = source;
  int searchOffset = 0;
  for(int i = 0; i < maxMatches; i++){
    printf("検索範囲: %d\n", searchOffset);
      // 戻り値は０で一致、それいがいで不一致
    if(regexec(&preg, source + searchOffset, maxMatches, matchResults, 0) == 0){
      printf("start: %d, end: %d\n", matchResults[i].rm_so, matchResults[i].rm_eo);
      if(matchResults[i].rm_so != -1 && matchResults[i].rm_eo != -1){
        int matchedPartLength = matchResults[i].rm_eo - matchResults[i].rm_so;
        strncpy(dest[i], &source[matchResults[i].rm_so], matchedPartLength);
        searchOffset = matchResults[i].rm_eo + 1;
        matchCount++;
      }
    }else{
      break;
    }
  }
  regfree(&preg);
  return matchCount;
}





int main(int argc, char **argv){
  char *fileName = argv[1];
  char text[STR_LENGTH] = "";
  char phoneNumbers[PHONENUMBER_COUNT][16];
  if(argc < 2){
    printf("対象のファイル名を引数に明記してください。\n");
    return 1;
  }
  printf("ファイル %s から読み込み開始: \n", fileName);
  if(readTextFile(text, fileName) == ERROR_FILEHANDLE){
    printf("読み込みに失敗しました。\n");
    return 1;
  }
  printf("抽出対象テキスト: ==========\n%s\n=================================\n", text);
  printf("結果: \n");
  // char *regex = "0[7,8,9]0-[0-9]{3,4}-[0-9]{4,5}";
  char *regex = "代表";
  int count = findMatches(regex, text, phoneNumbers, PHONENUMBER_COUNT);
  if(count == ERROR_REGEX){
    printf("正規表現は利用不可能です。\n");
    return 1;
  }
  for(int i = 0; i < count; i++){
    printf("[%d件目] %s\n", i + 1, phoneNumbers[i]);
  }
  return 0;
}