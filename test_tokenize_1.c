#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../compare.h"


char** tokenize(char* line, size_t line_length, size_t* out_num_tokens) 
{
	char *field = NULL;
	char *line_cpy = line;
	const char *delimiters = " \t\n";
	size_t count = 0;
	char** tokens_array = NULL;


	//Tokenize line inspired from https://stackoverflow.com/questions/50310178/split-string-on-any-number-of-white-spaces-in-c
	tokens_array = calloc(line_length+1, sizeof(char*));
	if (tokens_array == NULL)
	{
		perror("couldn't calloc? fix your system");
	}
	while ((field = strsep(&line_cpy, delimiters)) != NULL)
	{
		if (*field == '\0') {
			continue;
		}
		tokens_array[count] = strdup(field);
		count++;
	}
	*out_num_tokens = count;

	return tokens_array;	
}


int main() {
	char *line = strdup("ls\t\t    \t\t     \t \t        \t-l \t\t\t\t\t\t\t .");

	size_t size;
	char **tokens = tokenize(line, strlen(line), &size);

	char *res[] = {"ls", "-l", "."};
	free(line);
	if (is_equal_array_str(tokens, res, size, 3))
		return 0;
	return -1;
}