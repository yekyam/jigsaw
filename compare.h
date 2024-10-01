#ifndef COMPARE_H
#define COMPARE_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool is_equal_array_void(void* arr1, void* arr2, size_t size1, size_t size2)
{
	// https://stackoverflow.com/questions/34487743/equality-of-arrays-in-c
	return size1 == size2 && !memcmp(arr1, arr2, size1);
}

bool is_equal_array_str(char** arr1, char** arr2, size_t size1, size_t size2)
{
	if (size1 != size2)
	{
		return false;
	}

	for (size_t i = 0; i < size1; i++)
	{
		if (strcmp(arr1[i], arr2[i]) != 0)
		{
			return false;
		}
	}
	return true;
}

/* Prints the given token array
 * \token_array - An array of strings
 * \size - The amount of strings in the array
 */
void print_token_array(char** token_array, size_t size)
{
	printf("token_array:\n");
	for (size_t i = 0; i < size; i++)
	{
		printf("\t%s\n", token_array[i]);
	}
}

#endif