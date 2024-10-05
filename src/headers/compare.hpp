#ifndef COMPARE_HPP
#define COMPARE_HPP

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <array>

bool is_equal_array_void(void* arr1, void* arr2, size_t size1, size_t size2)
{
	// https://stackoverflow.com/questions/34487743/equality-of-arrays-in-c
	return size1 == size2 && !memcmp(arr1, arr2, size1);
}

template <class T, std::size_t S>
bool is_equal_array(std::array<T, S> a1, std::array<T, S> a2)
{
	return a1 == a2;
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

template <typename T, std::size_t N>
void print_token_array(std::array<T, N> token_array)
{
	std::cout << "Printing " << token_array.size() << " items:\n";
	for (const auto &item : token_array)
	{
		std::cout << '\t' << item << '\n';
	}
}

#endif