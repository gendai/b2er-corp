#include "file.h"
#include <stdlib.h>
#include <stdio.h>

char * fileReadAllText(char *path)
{
	FILE *f = fopen(path, "rb");
	if(!f)
		return 0;
	
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	rewind(f);

	char *content = malloc(size+1);
	fread(content, 1, size, f);
	fclose(f);
	return content;
}
