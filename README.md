NAME
       bsdfan - a simple utility to manage thinkpads fan on FreeBSD

SYNOPSIS
       bsdfan [-d] [-c config-file ]

DESCRIPTION
       bsdfan automaticallty regulates the fan speed by the sensor temperature
       and the user configuration.


OPTIONS
       -d     Start bsdfan as a daemon

       -c config-file
              Use  the  alternate   system   wide   config-file   instead   of
              /usr/local/etc/bsdfan.conf.

FILES
       /usr/local/etc/bsdfan.conf
              The  system  wide  configuration  file.   Overridden  by  the -c
              option.

              Here the user can specifies the levels the fan will be  set  to.
              They  range from 0 (fan idle) to 7 (full speed), it's not neces-
              sary to use all levels.  Levels must be  declared  in  ascending
              order by level_number and are declared as follows:

              level(level_number,level_min_temperature,level_max_temperature)

              where level_number is the level number as described above from 0
              to 7,

              level_min_temperature is the temperature below which  the  level
              will be decreased.

              level_max_temperature  is  the  maximum temperature beyond which
              the level will be increased.

              level_min_temperature for the first level must be  equal  to  0.
              levl_max_temperature  for  the  last  level must be greater than
              150.

              An example is provided with the software.

