#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "common.h"

#define SUBSTRING_ERROR "Character not found: "

char * removeLeadingSpaces(char *buff)
{       
        char *format = buff;
        while(*format==' ' || *format=='\t')
                format++;
        
        buff = format;
        
        return buff;
}

char * getSubstring(char *buff, char c)
{       
        char *p = buff;
        int size = sizeof(buff)/sizeof(char);
        int i =0;
        
        while(*buff!=c)
        {       
                buff++;
                i++;
                if(i>size)
                        error(SUBSTRING_ERROR,&c);
        }

        char *substring=malloc(i*sizeof(char));

        if(substring==NULL)
                error(MALLOC_ERROR,NULL);

        for(int k=0;k<i;k++)
        {
                substring[k]=*p;
                p++;
        }

        return substring;

}

bool isSameString(const char *a, const char*b)
{
        if(strcmp(a,b)!=0)
                return false;

        while(*a!='\0')
                if(*a==*b)
                {
                        a++;
                        b++;
                }
                else
                        return false;
        return true;
}

bool isEmptyString(const char *s) {
  static const char *emptyline_detector = " \t\n";

  return strspn(s, emptyline_detector) == strlen(s);
}
