#include "myshell.h"

/**
 * _getenv - getenv function
 * @name: character name
 * Return: return NULL
 */

char *_getenv(const char *name)
{
	size_t nvmelen = strlen(name);
	char **var;

	for (var = environ; *var != NULL; var++)
	{
		if (strncmp(name, *var, nvmelen) == 0 && (*var)[nvmelen] == '=')
		{
			return (&((*var)[nvmelen + 1]));
		}
	}
	return (NULL);
}
