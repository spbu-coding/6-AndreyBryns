#include <stdio.h>
#include <stdlib.h>
#include "sortings.c"

#define NUMBER_OF_ARGUMENTS 6

void free_array(strings_array_t array, array_size_t array_size)
{
	for (array_size_t i = 0; i < array_size; i++)
		free(array[i]);
	
	free(array);
}

typedef enum
{
	BUBBLE_SORT = 0,
	INSERTION_SORT,
	MERGE_SORT,
	QUICK_SORT,
	RADIX_SORT
} sort_t;

int asc(const char *a, const char *b)
{
	unsigned int len; 
	int result_of_comparing;
	
	if (strlen(a) > strlen(b))
		len = strlen(b);
	else
		len = strlen(a);
	
	result_of_comparing = strncmp(a, b, len - 1);

	if (result_of_comparing == 0)
	{
		return strlen(a) < strlen(b);
	}
	return result_of_comparing > 0;
}

int des(const char *a, const char *b)
{
	unsigned int len;
	int result_of_comparing;
	
	if (strlen(a) > strlen(b))
		len = strlen(b);
	else
		len = strlen(a);
		
	result_of_comparing = strncmp(a, b, len - 1);
	
	if (result_of_comparing == 0)
		return strlen(a) > strlen(b);
	
	return result_of_comparing < 0;
}

int is_number(char *str)
{
	unsigned int len = strlen(str);
	
	for (unsigned int i = 0; i < len; i++)
		if (str[i] < '0' && str[i] > '9')
			return 0;
	
	return 1;
}

strings_array_t allocate_memory_for_strings_array(array_size_t array_size)
{
	strings_array_t array = (strings_array_t)malloc(sizeof(char *) * array_size);
	
	if (array == NULL)
	{
		return NULL;
	}
	
	for (array_size_t i = 0; i < array_size; i++)
	{
		array[i] = malloc(sizeof(char) * (MAX_INPUT_STRING_SIZE + 2));
		
		if (array[i] == NULL)
		{
			free_array(array, i);
			
			return NULL;
		}
	}
	
	return array;
}

int read_array(strings_array_t array, array_size_t array_size, FILE *input)
{
	char symbol;
	
	unsigned int symbol_index;
	
	for (array_size_t i = 0; i < array_size; i++)
	{
		if (feof(input))
			return -1;
		
		symbol_index = 0;
		
		while (symbol_index < MAX_INPUT_STRING_SIZE)
		{
			if ((symbol = fgetc(input)) == EOF)
				break;
			
			if (symbol == '\n')
				break;
			
			array[i][symbol_index] = symbol;
			
			symbol_index++; 
		}
		
		if (i == array_size - 1)
		{
			array[i][symbol_index] = '\n';
			array[i][symbol_index + 1] = '\0';
		}
		else
			array[i][symbol_index] = '\0';
		
		//printf("%d\n", strlen(array[i]));
	}
	
	return 0;
}

int write_array(strings_array_t array, array_size_t array_size, FILE *output)
{
	for (array_size_t i = 0; i < array_size; i++)
		if (fputs(array[i], output))
			return -1;
			
	return 0;
}

int main(int argc, char **argv)
{
	if (argc != NUMBER_OF_ARGUMENTS)
	{
		fprintf(stderr, "Invalid number of arguments\n");
		
		return -1;
	}
	
	array_size_t number_of_strings;
	FILE *input, *output;
	sort_t sort_algorithm;
	comparator_func_t cmp;
	
	if (!is_number(argv[1]))
	{
		fprintf(stderr, "First parameter not a number\n");
		
		return -1;
	}
	
	number_of_strings = atoi(argv[1]);
	
	input = fopen(argv[2], "rb");
	output = fopen(argv[3], "wb");
	
	if (input == NULL || output == NULL)
	{
		fprintf(stderr, "Input/output error\n");
		
		if (output != NULL)
			fclose(output);
		if (input != NULL)
			fclose(input);
		
		return -1;
	}
	
	if (!strcmp(argv[4], "bubble"))
		sort_algorithm = BUBBLE_SORT;
	else
	if (!strcmp(argv[4], "insertion"))
		sort_algorithm = INSERTION_SORT;
	else
	if (!strcmp(argv[4], "quick"))
		sort_algorithm = QUICK_SORT;
	else
	if (!strcmp(argv[4], "merge"))
		sort_algorithm = MERGE_SORT;
	else
	if (!strcmp(argv[4], "radix"))
		sort_algorithm = RADIX_SORT;
	else
	{
		fprintf(stderr, "Invalid sort algorithm\n");
		
		fclose(output);
		fclose(input);
		
		return -1;
	}
	
	if (!strcmp(argv[5], "asc"))
		cmp = *asc;
	else
	if (!strcmp(argv[5], "des"))
		cmp = *des;
	else
	{
		fprintf(stderr, "Invalid comparer\n");
		
		fclose(output);
		fclose(input);
		
		return -1;
	}
	
	if (number_of_strings == 0)
	{
		fprintf(output, "\n");
		
		fclose(output);
		fclose(input);
		
		return 0;
	}
	
	strings_array_t array = allocate_memory_for_strings_array(number_of_strings);
	
	if (array == NULL)
	{
		fprintf(stderr, "Memory allocation error\n");

		fclose(output);
		fclose(input);
		
		return -1;
	}
	
	if (read_array(array, number_of_strings, input) != 0)
	{
		fprintf(stderr, "Too few lines\n");
		
		free_array(array, number_of_strings);
		fclose(output);
		fclose(input);
		
		return -1;
	}
	
	switch(sort_algorithm)
	{
		case BUBBLE_SORT:
		{
			bubble(array, number_of_strings, cmp);
			
			break;
		}
		
		case INSERTION_SORT:
		{
			insertion(array, number_of_strings, cmp);
			
			break;
		}
		
		case MERGE_SORT:
		{
			merge(array, number_of_strings, cmp);
			
			break;
		}
		
		case QUICK_SORT:
		{
			quick(array, number_of_strings, cmp);
			
			break;
		}
		
		case RADIX_SORT:
		{
			radix(array, number_of_strings, cmp);
			
			break;
		}
	}
	
	if (write_array(array, number_of_strings, output) != 0)
	{
		fprintf(stderr, "Cannot write on file\n");
		
		free_array(array, number_of_strings);
		fclose(output);
		fclose(input);
		
		return -1;
	}
	
	free_array(array, number_of_strings);
	fclose(output);
	fclose(input);
	return 0;
}
