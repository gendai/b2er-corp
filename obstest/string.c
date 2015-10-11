#include "string.h"
#include <stdlib.h>
#include <string.h>

char **stringSplit(char *str, char *separator, int *size)
{
	*size = 0;
	int startSub = 0;
	int sepI = 0;
	int i = 0;

	static char *TEMP_SPLIT[1000];

	while(1)
	{
		if(!str[i] || str[i] == separator[sepI])
		{
			if(!str[i] || !separator[++sepI])
			{
				int subSize = i - sepI - startSub + !!str[i];
				char *sub = malloc(subSize + 1);
				memcpy(sub, str + startSub, subSize);
				sub[subSize] = 0;
				TEMP_SPLIT[*size] = sub;
				(*size)++;
				startSub = i+1;
				sepI = 0;
			}
		}

		if(!str[i])
		{
			break;
		}

		++i;
	}

	char **result = malloc(*size * sizeof(char *));
	for(int i = 0; i < *size; ++i)
		result[i] = TEMP_SPLIT[i];
	return result;
}
