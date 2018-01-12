#include <sys/param.h>
#include <sys/linker.h>

#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "parser.h"
#include "system.h"

#define INVALID_ARGUMENT_ERROR "Arugment not recognised"
#define MODULE_LOAD_ERROR "Module acpi_ibm.ko load failed"
#define DEFAULT_CONF_PATH LOCALBASE "/etc/bsdfan.conf"

#define VERSION "0.1"

static void
autofan(int sig __unused)
{
	setFan(AUTO,NULL);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	const char *confpath= DEFAULT_CONF_PATH;
	char op;
	bool daemonize = false;
	const struct Config *conf;

	if (kldfind("acpi_ibm")== -1)
		if(kldload("acpi_ibm")==-1)
			err(EXIT_FAILURE, MODULE_LOAD_ERROR);

	while( (op = getopt(argc, argv, "vdc:")) != -1)
	{
		switch(op)
		{
			case 'v':
				printf("Version: %s\n",VERSION);
				exit(0);
				break;
			case 'd':
				daemonize = true;
				break;
			case 'c':
				confpath=optarg;
				break;
			case '?':
				errx(EXIT_FAILURE, INVALID_ARGUMENT_ERROR);
				break;
			default:
				break;
		}
	}

	conf = readConfig(confpath);

	/* when exit you must reset the fan to AUTO */
	(void) signal (SIGINT, autofan);
	(void) signal (SIGTERM, autofan);
	(void) signal (SIGHUP, autofan);
	(void) signal (SIGSEGV, autofan);

	setFan(MANUAL,conf->levels);

	int oldtemp=0;

	if(daemonize)
		daemon(0,0);

	while(1)
	{
		int time =2;

		int cur_temp = getTemp();

		if(oldtemp-cur_temp>2)
			time =1;

		oldtemp = cur_temp;

		adjustLevel(cur_temp,conf);

		sleep(time);
	}

}
