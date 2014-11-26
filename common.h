#define MALLOC_ERROR "Memory allocation error"
void error(char *err, char *detail);

struct Level
{
	int number;
	int min_max[2];
};

struct Config
{
	struct Level *levels;
	int levels_size;
};

