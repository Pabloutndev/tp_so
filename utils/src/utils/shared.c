#include "shared.h"

int existe_archivo(char* path)
{
	int ret = 0;
	FILE* f = fopen(path,"r");
	if (f != NULL)
	{
		ret = 1;
		fclose(f);
	}
	return ret;
}
/*
int existe_dir(char* path)
{
	DIR* dir = opendir(path);
	if (dir) {
		closedir(dir);
		return 1;
	} else if (ENOENT == errno) {
		return 0;
	} else {
		return -1;
	}
}
*/
int string_arr_size(char** a)
{
    int i = 0;
    while(a[i] != NULL)
    {
        i++;
    }
    return i;
}