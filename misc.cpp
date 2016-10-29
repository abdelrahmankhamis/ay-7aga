#include "misc.h"
#include <string.h>
#include <cctype>

void RemoveComma(char* str, int i, int len)
{
    for (i=0; i<len; i++) {
        if (str[i]==',') {
            str[i] = ' ';
        }
    }
}

void Trim(char *str)
{
    size_t len = 0;
    char *start = str - 1;
    char *end = NULL;

    if(str == NULL) return;
    if(str[0] == '\0') return;

    len = strlen(str);
    end = str + len;

    while(isspace(*(++start)));
    while(isspace(*(--end)) && end != start);

    if(str + len - 1 != end) *(end + 1) = '\0';
    else if( start != str &&  end == start ) *str = '\0';

    end = str;
    if(start != str)
    {
        while(*start) *end++ = *start++;
        *end = '\0';
    }
}

void Capitalize(char* str)
{
    int i, len = strlen(str);
    if ( len > 0 )
    {
        str[0] = toupper(str[0]);
        for(i = 1; i < len; i++)
        {
            if (str[i] == ' ' && i + 1 != len)
            {
                str[i+1] = toupper(str[i+1]);
                i++;
            }
            else
                str[i] = tolower(str[i]);
        }
    }
}

void StrCopy(char*dest, char*src)
{
    int i;
    for (i=0; src[i] != '\0'; i++)
        dest[i] = src[i];
    dest[i] = '\0';
}

char* RemoveExtraSpaces(char* line)
{
    int i, len = strlen(line);
    for (i=0; i<len; i++)
    {
        if (line[i] == ' ')
        {
            if (i+1 == len)
                line[i] = '\0';
            else if (i+1 != len && line[i+1] == ' ')
            {
                StrCopy(&line[i], &line[i+1]);
                i--;
                len--;
            }
        }
    }

    if (len > 0 && line[0] == ' ')
        StrCopy(&line[0], &line[1]);

    if (len > 0 && line[len -1] == ' ')
        line[len -1] = '\0';

    return line;
}
