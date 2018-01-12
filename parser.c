#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sysexits.h>

#include "common.h"
#include "mystring.h"
#include "parser.h"

#define CONFIG_ERROR "Could not find config file"
#define PARSING_ERROR "Error parsing conifg, the following argument was not understood: %s"
#define LEVEL_ERROR "Could not find '('"
#define LEVEL_VAL_ERROR "Could not parse level value: %s"
#define INVALID_LEVEL_ERROR "Level out of range: %d"
#define LEVEL_RANGE_ERROR "This parsed range is not admissible: %s"
#define SANITY_CHECK_ERROR "The levels defined in the config are not in an ascending order by level_number "
#define SANITY_CHECK_TEMP_RANGE_ERROR "First level min temperature >0 or last level max temperature <150, dangerous"
#define SANITY_CHECK_TEMP_ERROR "There's a gap between the temperatures defined (level n.max<n+1.min)"
/*PARSER TO READ THE CONFIGURATION FILE*/

static const struct Level * getLevel(const char *buff);
static void sanityCheck(const struct Level *levels, int levels_size);

const struct Config *
readConfig(const char *path)
{
	struct Config * config;
	FILE *fp;
	char buff[255];

	fp = fopen(path, "r");
	if (fp == NULL)
		err(EX_CONFIG, CONFIG_ERROR);

	config = malloc(sizeof(struct Config));
	if (config == NULL)
		err(EX_SOFTWARE, MALLOC_ERROR);

	config->levels = NULL;
	config->levels_size = 0;

	while (fgets(buff, sizeof(buff), fp) != NULL)
	{
		 if (buff[0]=='#' || isEmptyString(buff))
			continue;

		const char *format = removeLeadingSpaces(buff);

		if (format[0]=='#')
			continue;

		char read[6];
		strncpy(read, format, sizeof(read)-1);
		read[5]='\0';

		if (isSameString(read, "level")) {
			config->levels = realloc(config->levels,sizeof(config->levels)+sizeof(struct Level));

			if(config->levels==NULL)
				err(EX_SOFTWARE, MALLOC_ERROR);

			config->levels[config->levels_size] = *getLevel(format);
			config->levels_size++;
		} else
			errx(EX_CONFIG, PARSING_ERROR, read);
	}

	fclose(fp);
	sanityCheck(config->levels,config->levels_size);
	return config;
}

static void
sanityCheck(const struct Level *levels, int levels_size)
{
	if(levels[0].min_max[0]!=0 || levels[levels_size-1].min_max[1]<150)
		errx(EX_CONFIG, SANITY_CHECK_TEMP_RANGE_ERROR);

	for(int i=0;i<levels_size-1;i++)
		if(levels[i].number>=levels[i+1].number)
			errx(EX_CONFIG, SANITY_CHECK_ERROR);
		else if (levels[i].min_max[1]<levels[i+1].min_max[0])
			errx(EX_CONFIG, SANITY_CHECK_TEMP_ERROR);
}

static const struct
Level * getLevel(const char *buff)
{
        char *p;
        struct Level *level;

        p = strchr(buff, '(');

        if(!p)
                errx(EX_CONFIG, LEVEL_ERROR);
        p++;

        level = malloc(sizeof(struct Level));

	if (level == NULL)
		err(EX_SOFTWARE, MALLOC_ERROR);

        level->number = getNumber(p, ',', LEVEL_VAL_ERROR, &p);
        if (level->number > 7)
                errx(EX_CONFIG, INVALID_LEVEL_ERROR, level->number);

        level->min_max[0] = getNumber(p, ',', LEVEL_VAL_ERROR, &p);
        level->min_max[1] = getNumber(p, ')', LEVEL_VAL_ERROR, &p);

	if (level->min_max[0] >= level->min_max[1])
		errx(EX_CONFIG, LEVEL_RANGE_ERROR, buff);

	return level;
}
