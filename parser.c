#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "common.h"
#include "mystring.h"

#define CONFIG_ERROR "Could not find config file"
#define PARSING_ERROR "Error parsing conifg, the following argument was not understood: "
#define LEVEL_ERROR "Could not find '('"
#define LEVEL_VAL_ERROR "Could not parse level value: "
#define INVALID_LEVEL_ERROR "Level out of range: " 
#define LEVEL_RANGE_ERROR "This parsed range is not admissible"
#define SANITY_CHECK_ERROR "The levels defined in the config are not in an ascending order by level_number "
#define SANITY_CHECK_TEMP_RANGE_ERROR "First level min temperature >0 or last level max temperature <150, dangerous"
#define SANITY_CHECK_TEMP_ERROR "There's a gap between the temperatures defined (level n.max<n+1.min)"
/*PARSER TO READ THE CONFIGURATION FILE*/

static struct Level * getLevel(char *buff);
static void sanityCheck(struct Level *levels, int levels_size);

struct Config * readConfig(char *path)
{
	
	FILE *fp=NULL;
	fp = fopen(path, "r");
	
	if(fp==NULL)
		error(CONFIG_ERROR,NULL);	
	
	char buff[255];
	
	struct Config * config=malloc(sizeof(struct Config));	
	if(config==NULL)
		error(MALLOC_ERROR,NULL);	
	
	config->levels=NULL;
	config->levels_size=0;
	
	while (fgets(buff,255,(FILE *)fp) != NULL)
	{	
		 if( buff[0]=='#' || isEmptyString(buff) )
			continue;
		
		char *format=removeLeadingSpaces(buff);
		
		if(format[0]=='#')
			continue;	
		
		char read[6];
		strncpy(read,format,sizeof(read)-1);
		read[5]='\0';
		
		if( isSameString(read,"level") )
		{
			config->levels = realloc(config->levels,sizeof(config->levels)+sizeof(struct Level));
			
			if(config->levels==NULL)
				error(MALLOC_ERROR,NULL);
			
			config->levels[config->levels_size] = *getLevel(format);
			config->levels_size++;
		}
		else
			error(PARSING_ERROR,read);	
	}
	
	fclose(fp);
	sanityCheck(config->levels,config->levels_size);	
	return config;	
}

static void sanityCheck(struct Level *levels, int levels_size)
{
	if(levels[0].min_max[0]!=0 || levels[levels_size-1].min_max[1]<150)
		error(SANITY_CHECK_TEMP_RANGE_ERROR,NULL);


	for(int i=0;i<levels_size-1;i++)
		if(levels[i].number>=levels[i+1].number)
			error(SANITY_CHECK_ERROR,NULL);
		else if (levels[i].min_max[1]<levels[i+1].min_max[0])
			error(SANITY_CHECK_TEMP_ERROR,NULL);
}

static struct Level * getLevel(char *buff)
{
        char *p = strchr(buff,'(');
        if(!p)
                error(LEVEL_ERROR,NULL);
        p++;
    
        struct Level *level=malloc(sizeof(struct Level));
		
	if(level==NULL)
		error(MALLOC_ERROR,NULL);
    
        char *check;
    
        level->number = (int) strtol(getSubstring(p,','),&check,10);
        if(check==p)
                error(LEVEL_VAL_ERROR,p);
        else if (level->number<0 || level->number >7) 
                error(INVALID_LEVEL_ERROR,p);   
    
        p = strchr(p,',');    
        p++;
    
        level->min_max[0] = (int) strtol(getSubstring(p,','),&check,10);
    
        if(check==p)
                error(LEVEL_VAL_ERROR,p);

        p = strchr(p,',');    
        p++;

        level->min_max[1] = (int) strtol(getSubstring(p,')'),&check,10);

        if(check==p)
                error(LEVEL_VAL_ERROR,p);
	
	if(level->min_max[0]<0 || level->min_max[1]<0 || level->min_max[0]>=level->min_max[1])
		error(LEVEL_RANGE_ERROR,buff);
        
	return level;
}
