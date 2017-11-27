#include <stdbool.h>
#define MANUAL 0
#define AUTO 1

void setFan(int mode, const struct Level *levels);
void adjustLevel(int t, const struct Config *conf);
int getTemp(void);
