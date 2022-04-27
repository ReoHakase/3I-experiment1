#define ERROR_FILEHANDLE -1
#define ERROR_REGEX -2

#define STR_LENGTH 1024
#define MATCHED_TEXT_LENGTH 32
#define MAX_MATCHES 4
#define MAX_GROUPS 4
#define LINE_LENGTH 128

int readTextFile(char *dest, char *fileName);
int findMatches(char *regexText, char *source, unsigned int maxMatches, char (*dest)[MAX_GROUPS][MATCHED_TEXT_LENGTH]);
void listMatches(char *regexText, char *source);