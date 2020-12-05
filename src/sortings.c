#include <stdlib.h>
#include "sortings.h"

#define swap_strings(a, b) char *temp = a; a = b; b = temp

void bubble(strings_array_t array, array_size_t array_size, comparator_func_t cmp)
{
	for (array_size_t i = 0; i < array_size; i++)
		for (array_size_t j = i + 1; j < array_size; j++)
			if (!cmp(array[i], array[j]))
			{
				swap_strings(array[i], array[j]);
			}
}

void insertion(strings_array_t array, array_size_t array_size, comparator_func_t cmp)
{
	for (array_size_t i = 1; i < array_size; i++)
	{
		array_size_t j = i;
		
		while (j != 0 && !cmp(array[j - 1], array[j]))
		{
			swap_strings(array[j - 1], array[j]);
			j--;
		}
	}
}

void merge(strings_array_t array, array_size_t array_size, comparator_func_t cmp)
{
	if (array_size <= 1)
		return;
	
	if (array_size == 2)
	{
		if (!cmp(array[0], array[1]))
		{
			swap_strings(array[0], array[1]);
		}
		
		return;
	}
	
	array_size_t first_array_size = array_size / 2 + array_size % 2, second_array_size = array_size / 2;
	merge(array, first_array_size, cmp);
	merge(array + first_array_size, second_array_size, cmp);
	
	strings_array_t sorted_array = (strings_array_t)malloc(sizeof(char *) * array_size);
	
	array_size_t first_array_index = 0, second_array_index = first_array_size;
	
	for (array_size_t i = 0; i < array_size; i++)
	{
		if (second_array_index == array_size)
		{
			sorted_array[i] = array[first_array_index];
			first_array_index++;
		}
		else
		if (first_array_index == first_array_size)
		{
			sorted_array[i] = array[second_array_index];
			second_array_index++;
		}
		else
		if (cmp(array[first_array_index], array[second_array_index]))
		{
			sorted_array[i] = array[first_array_index];
			first_array_index++;
		}
		else
		{
			sorted_array[i] = array[second_array_index];
			second_array_index++;
		}
	}
	
	for (array_size_t i = 0; i < array_size; i++)
		array[i] = sorted_array[i];
	
	free(sorted_array);
}

void quick(strings_array_t array, array_size_t array_size, comparator_func_t cmp)
{
	if (array_size <= 1)
		return;
	
	if (array_size == 2)
	{
		if (!cmp(array[0], array[1]))
		{
			swap_strings(array[0], array[1]);
		}
		
		return;
	}
	
	array_size_t pivot_index = array_size - 1;
	
	for (array_size_t i = 0; i < pivot_index; i++)
	{
		while (pivot_index != 0 && !cmp(array[i], array[pivot_index]))
		{
			if (pivot_index - 1 != i)
			{
				swap_strings(array[pivot_index - 1], array[pivot_index]);
			}
			
			swap_strings(array[i], array[pivot_index]);
			pivot_index--;
		}
	}
	
	array_size_t first_array_size = pivot_index, second_array_size = array_size - pivot_index;
	quick(array, first_array_size, cmp);
	quick(array + first_array_size, second_array_size, cmp);
}

void radix(strings_array_t array, array_size_t array_size, comparator_func_t cmp)
{
	strings_array_t buffer = (strings_array_t)malloc(sizeof(char *) * array_size);
	unsigned int *string_sizes = malloc(sizeof(unsigned int) * array_size), *string_sizes_buf = malloc(sizeof(unsigned int) * array_size);
	unsigned int max_length = 0;
	
	for (array_size_t i = 0; i < array_size; i++)
	{
		string_sizes[i] = strlen(array[i]);
		
		if (string_sizes[i] > max_length)
			max_length = string_sizes[i];
	}
	
	for (unsigned int current_radix = max_length; current_radix > 0; current_radix--)
	{
		unsigned int buffer_next_element = 0;
		
		for (array_size_t i = 0; i < array_size; i++)
		{
			if (string_sizes[i] < current_radix)
			{
				string_sizes_buf[buffer_next_element] = string_sizes[i];
				buffer[buffer_next_element] = array[i];
				buffer_next_element++;
			}
		}
		
		for (int current_char = 0; current_char <= 256; current_char++)
		for (array_size_t i = 0; i < array_size; i++)
		{
			if (string_sizes[i] < current_radix)
				continue;
			
			if (array[i][current_radix - 1] == current_char)
			{
				string_sizes_buf[buffer_next_element] = string_sizes[i];
				buffer[buffer_next_element] = array[i];
				buffer_next_element++;
			}
		}
		
		for (array_size_t i = 0; i < array_size; i++)
			array[i] = buffer[i];
		
		for (array_size_t i = 0; i < array_size; i++)
			string_sizes[i] = string_sizes_buf[i];
	}
	
	if (!cmp(array[0], array[1]))
	{
		for (array_size_t i = 0; i < array_size; i++)
			array[i] = buffer[array_size - i - 1];
	}
	
	free(buffer);
	free(string_sizes);
	free(string_sizes_buf);
}

