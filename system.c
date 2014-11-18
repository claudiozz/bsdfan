#include <unistd.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdbool.h>
#include "common.h"
#include "system.h"
#define INVALID_MODE_ERROR "Invalid fan mode"
#define PERMISSION_ERROR "Not enough permissions"

int idx_cur_level;
static int mib_set_fan_level[4];
static int mib_get_temp_level[4];
static int mib_get_temp_level_alt[5];
static bool temp_alt = false;

static void levelDown(struct Level *levels);
static void levelUp(struct Level *levels);

/*sets fan in either manual or auto mode, in manual sets the lowest defined level and calculates mib_get_fan_level and mib_get_temp_level*/ 
void setFan(int mode,struct Level *levels)
{	
	if(mode==MANUAL)
	{
		/*set fan in manual mode*/
		if( sysctlbyname("dev.acpi_ibm.0.fan",NULL,NULL,&mode,sizeof(int)) == -1)
			error(PERMISSION_ERROR,NULL);			

		/*get mib for dev.acpi_ibm.0.fan_level*/
		size_t len=4; 
		sysctlnametomib("dev.acpi_ibm.0.fan_level",mib_set_fan_level,&len);
	
		/*get mib for dev.acpi_ibm.0.thermal*/
		len = 4;
		if (sysctlnametomib("dev.acpi_ibm.0.thermal",mib_get_temp_level,&len) == -1)
		{
			len = 5;
			/*get mib for hw.acpi.thermal.tz0.temperature*/
			sysctlnametomib("hw.acpi.thermal.tz0.temperature",mib_get_temp_level_alt,&len);
			temp_alt=true;
		}

		/*set lowest available level*/
		len = 4;
		
		sysctl(mib_set_fan_level,4,NULL,NULL,&levels[0].number,sizeof(int));
		idx_cur_level = 0;
	
	}
	else if(mode==AUTO)
		sysctlbyname("dev.acpi_ibm.0.fan",NULL,NULL,&mode,sizeof(int));
	else
		error(INVALID_MODE_ERROR,NULL);
	
}

void adjustLevel(int t, struct Config *conf) /*t for temp*/
{
		if( t < conf->levels[idx_cur_level].min_max[0] )
			levelDown(conf->levels);

		else if( t > conf->levels[idx_cur_level].min_max[1])	
			levelUp(conf->levels);
}

static void levelUp(struct Level *levels)
{
	sysctl(mib_set_fan_level,4,NULL,NULL,&levels[idx_cur_level+1].number,sizeof(int));
	idx_cur_level++;
}

static void levelDown(struct Level *levels)
{
	sysctl(mib_set_fan_level,4,NULL,NULL,&levels[idx_cur_level-1].number,sizeof(int));
	idx_cur_level--;
}

int getTemp()
{
	int temp[8]={0};
	size_t len = 8*sizeof(int);
	if(temp_alt)
		sysctl(mib_get_temp_level,5,&temp,&len,NULL,0);

	else
		sysctl(mib_get_temp_level,4,&temp,&len,NULL,0);
	
	return temp[0];
	
}
