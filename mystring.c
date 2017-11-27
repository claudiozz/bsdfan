#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "mystring.h"

#define SUBSTRING_ERROR "Character not found: %c"

const char *
removeLeadingSpaces(const char *buff)
{
        while (*buff == ' ' || *buff == '\t')
                buff++;

        return buff;
}

int
getNumber(const char *buff, char c, const char *errString, char **pend)
{
	char	*end;
	long	 value;

	errno = 0;
	value = strtol(buff, &end, 10); /* Must it always be decimal? */

	if (errno)
		err(EX_CONFIG, errString, buff);

	if (*end != c) {
		errx(EX_CONFIG, SUBSTRING_ERROR, c);
        }

	if (value < 0)
		errx(EX_CONFIG, "Don't be negative (%ld)", value);

	if (value > INT_MAX)
		errx(EX_CONFIG, "Value %ld is entirely too large", value);

	*pend = end + 1;

        return (int)value;
}

bool
isSameString(const char *a, const char *b)
{
        return strcmp(a, b) == 0;
}

bool
isEmptyString(const char *s) {
	s += strspn(s, " \t\n");
	return *s == '\0';
}
