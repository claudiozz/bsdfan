#include <stdbool.h>
#define MANUAL 0
#define AUTO 1


void setFan(int mode,struct Level *levels);
void adjustLevel(int t, struct Config *conf);
int getTemp();
