#define AM_SAFETY
#include "array_mini.h"

#include <test_mini.h>

#include <stdio.h>
#include <stdlib.h>


//#define CALL(a) a; fputs(#a "\n", stdout)

// NOTE: tests..
//       .. am_add_num_elements
//       .. am_remove_elements
int test_1()
{
	int numNumbers = 0;
	int* numbers;
	
	int numNewNumbers = 5;
	
	am_add_num_elements2(&numNumbers, &numbers, 5);
	if(numNumbers != numNewNumbers)
	{
		fprintf(stderr, "error: numNumbers != %i (numNumbers == %i)\n", numNewNumbers, numNumbers);
		return 0;
	}
	for(int i = 0; i < numNumbers; ++i)
	{
		numbers[i] = (i + 1) * (i + 1);
	}
	
	am_remove_elements2(&numNumbers, &numbers);
	if(numNumbers != 0)
	{
		fprintf(stderr, "error: numNumbers != 0 (numNumbers == %i)\n", numNumbers);
		return 0;
	}

	return 1;
}
// NOTE: tests am_add_elements
int test_2()
{
	int numNumbers;
	int* numbers;
	
	int numNewNumbers = 2;
	int newNumbers[numNewNumbers];// = { 1, 4 };
	newNumbers[0] = 1;
	newNumbers[1] = 4;
	
	am_add_elements2(&numNumbers, &numbers, numNewNumbers, newNumbers);
	if(numNumbers != numNewNumbers)
	{
		fprintf(stderr, "error: numNumbers != %i (numNumbers == %i)\n", numNewNumbers, numNumbers);
		return 0;
	}
	for(int i = 0; i < numNumbers; ++i)
	{
		if(numbers[i] != newNumbers[i])
		{
			fprintf(stderr, "error: numbers[%i] != %i (numbers[%i] == %i)\n", i, newNumbers[i], i, numbers[i]);
			return 0;
		}
	}
	
	am_remove_elements2(&numNumbers, &numbers);
	
	return 1;
}

// NOTE: tests am_append_num_elements
int test_3()
{
	int numNumbers = 5;
	//int* numbers = new int[numNumbers];
	int* numbers = (int*)malloc(sizeof(int) * numNumbers);
	for(int i = 0; i < numNumbers; ++i)
	{
		numbers[i] = (i + 1) * (i + 1);
	}
	
	int numNewNumbers = 2;
	
	int a = 5;
	am_append_num_elements2(&numNumbers, &numbers, numNewNumbers);
	if(numNumbers != a + numNewNumbers)
	{
		fprintf(stderr, "error: numNumbers != %i (numNumbers == %i)\n", a + numNewNumbers, numNumbers);
		return 0;
	}
	for(int i = 0; i < a; ++i) //< only check numbers that were already there
	{
		int b = (i + 1) * (i + 1);
		if(numbers[i] != b)
		{
			fprintf(stderr, "error: numbers[%i] != %i (numbers[%i] == %i)\n", i, b, i, numbers[i]);
			return 0;
		}
	}
	
	am_remove_elements2(&numNumbers, &numbers);
	
	return 1;
}
// NOTE: tests am_append_elements
int test_4()
{
	int numNumbers = 5;
	//int* numbers = new int[5];
	int* numbers = (int*)malloc(sizeof(int) * numNumbers);
	for(int i = 0; i < 5; ++i)
	{
		numbers[i] = (i + 1) * (i + 1);
	}
	
	int numNewNumbers = 2;
	int newNumbers[numNewNumbers];
	for(int i = 0; i < numNewNumbers; ++i)
	{
		int b = numNumbers + i;
		newNumbers[i] = (b + 1) * (b + 1);
	}
	
	int a = numNumbers;
	am_append_elements2(&numNumbers, &numbers, numNewNumbers, newNumbers);
	if(numNumbers != a + numNewNumbers)
	{
		fprintf(stderr, "error: numNumbers != %i (numNumbers == %i)\n", a + numNewNumbers, numNumbers);
		return 0;
	}
	for(int i = 0; i < numNumbers; ++i)
	{
		int b = (i + 1) * (i + 1);
		if(numbers[i] != b)
		{
			fprintf(stderr, "error: numbers[%i] != %i (numbers[%i] == %i)\n", i, b, i, numbers[i]);
			return 0;
		}
	}
	
	am_remove_elements2(&numNumbers, &numbers);

	return 1;
}

// NOTE: tests am_append_or_add_num_elements
int test_5()
{
	int numNumbers = 0;
	int* numbers;
	
	int numNewNumbers1 = 5;
	
	am_append_or_add_num_elements2(&numNumbers, &numbers, numNewNumbers1);
	if(numNumbers != numNewNumbers1)
	{
		fprintf(stderr, "error: numNumbers != %i (numNumbers == %i)\n", numNewNumbers1, numNumbers);
		return 0;
	}
	for(int i = 0; i < numNumbers; ++i)
	{
		numbers[i] = (i + 1) * (i + 1);
	}
	
	int numNewNumbers2 = 2;
	
	am_append_or_add_num_elements2(&numNumbers, &numbers, numNewNumbers2);
	if(numNumbers != numNewNumbers1 + numNewNumbers2)
	{
		fprintf(stderr, "error: numNumbers != %i (numNumbers == %i)\n", numNewNumbers1 + numNewNumbers2, numNumbers);
		return 0;
	}
	for(int i = 0; i < numNewNumbers1; ++i) //< only check numbers that were already there
	{
		int a = (i + 1) * (i + 1);
		if(numbers[i] != a)
		{
			fprintf(stderr, "error: numbers[%i] != %i (numbers[%i] == %i)\n", i, a, i, numbers[i]);
			return 0;
		}
	}
	
	am_remove_elements2(&numNumbers, &numbers);
	
	return 1;
}
// NOTE: tests am_append_or_add_elements
int test_6()
{
	int numNumbers = 0;
	int* numbers;
	
	int numNewNumbers1 = 5;
	int newNumbers1[numNewNumbers1];
	for(int i = 0; i < numNewNumbers1; ++i)
	{
		newNumbers1[i] = (i + 1) * (i + 1);
	}
	
	am_append_or_add_elements2(&numNumbers, &numbers, numNewNumbers1, newNumbers1);
	if(numNumbers != numNewNumbers1)
	{
		fprintf(stderr, "error: numNumbers != %i (numNumbers == %i)\n", numNewNumbers1, numNumbers);
		return 0;
	}
	for(int i = 0; i < numNewNumbers1; ++i)
	{
		if(numbers[i] != newNumbers1[i])
		{
			fprintf(stderr, "error: numbers[%i] != %i (numbers[%i] == %i)\n", i, newNumbers1[i], i, numbers[i]);
			return 0;
		}
	}
	
	int numNewNumbers2 = 2;
	int newNumbers2[numNewNumbers2];
	for(int i = 0; i < numNewNumbers2; ++i)
	{
		int a = numNewNumbers1 + i;
		newNumbers2[i] = (a + 1) * (a + 1);
	}
	
	am_append_or_add_elements2(&numNumbers, &numbers, numNewNumbers2, newNumbers2);
	if(numNumbers != numNewNumbers1 + numNewNumbers2)
	{
		fprintf(stderr, "error: numNumbers != %i (numNumbers == %i)\n", numNewNumbers1 + numNewNumbers2, numNumbers);
		return 0;
	}
	for(int i = 0; i < numNumbers; ++i)
	{
		int a = (i + 1) * (i + 1);
		if(numbers[i] != a)
		{
			fprintf(stderr, "error: numbers[%i] != %i (numbers[%i] == %i)\n", i, a, i, numbers[i]);
			return 0;
		}
	}
	
	am_remove_elements2(&numNumbers, &numbers);
	
	return 1;
}

// NOTE: tests am_replace_elements
int test_7()
{
	int numNumbers = 12;
	int* numbers = malloc(sizeof(int) * numNumbers);
	for(int i = 0; i < numNumbers; ++i)
	{
		numbers[i] = (i + 3) * (i + 3); 
	}
	
	int numNewNumbers = 5;
	int newNumbers[numNewNumbers];
	for(int i = 0; i < numNewNumbers; ++i)
	{
		newNumbers[i] = (i + 1) * (i + 1);
	}
	
	am_replace_elements2(&numNumbers, &numbers, numNewNumbers, newNumbers);
	if(numNumbers != numNewNumbers)
	{
		fprintf(stderr, "error: numNumbers != %i (numNumbers == %i)\n", numNewNumbers, numNumbers);
		return 0;
	}
	for(int i = 0; i < numNumbers; ++i)
	{
		int a = (i + 1) * (i + 1);
		if(numbers[i] != a)
		{
			fprintf(stderr, "error: numbers[%i] != %i (numbers[%i] == %i)\n", i, a, i, numbers[i]);
			return 0;
		}
	}
	
	am_remove_elements2(&numNumbers, &numbers);
	
	return 1;
}

// NOTE: tests am_add_or_replace_elements
int test_8()
{
	int numNumbers = 0;
	int* numbers;
	
	int numNewNumbers1 = 5;
	int newNumbers1[numNewNumbers1];
	for(int i = 0; i < numNewNumbers1; ++i)
	{
		newNumbers1[i] = (i + 1) * (i + 1);
	}
	
	am_add_or_replace_elements2(&numNumbers, &numbers, numNewNumbers1, newNumbers1);
	if(numNumbers != numNewNumbers1)
	{
		fprintf(stderr, "error: numNumbers != %i (numNumbers == %i)\n", numNewNumbers1, numNumbers);
		return 0;
	}
	for(int i = 0; i < numNewNumbers1; ++i)
	{
		if(numbers[i] != newNumbers1[i])
		{
			fprintf(stderr, "error: numbers[%i] != %i (numbers[%i] == %i)\n", i, newNumbers1[i], i, numbers[i]);
			return 0;
		}
	}
	
	int numNewNumbers2 = 12;
	int newNumbers2[numNewNumbers2];
	for(int i = 0; i < numNewNumbers2; ++i)
	{
		newNumbers2[i] = (i + 3) * (i + 3);
	}
	
	am_add_or_replace_elements2(&numNumbers, &numbers, numNewNumbers2, newNumbers2);
	if(numNumbers != numNewNumbers2)
	{
		fprintf(stderr, "error: numNumbers != %i (numNumbers == %i)\n", numNewNumbers2, numNumbers);
		return 0;
	}
	for(int i = 0; i < numNumbers; ++i)
	{
		if(numbers[i] != newNumbers2[i])
		{
			fprintf(stderr, "error: numbers[%i] != %i (numbers[%i] == %i)\n", i, newNumbers2[i], i, numbers[i]);
			return 0;
		}
	}
	
	am_remove_elements2(&numNumbers, &numbers);

	return 1;
}

// NOTE: tests..
//       .. am_remove_first_num_elements
//       .. am_remove_last_num_elements
int test_9()
{
	int numNumbers = 12;
	//int* numbers = new int[numNumbers];
	int* numbers = (int*)malloc(sizeof(int) * numNumbers);
	for(int i = 0; i < numNumbers; ++i)
	{
		numbers[i] = (i + 3) * (i + 3);
	}
	
	int numNumbersToRemove1 = 3;
	
	int a = numNumbers;
	am_remove_first_num_elements2(&numNumbers, &numbers, numNumbersToRemove1);
	if(numNumbers != a - numNumbersToRemove1)
	{
		fprintf(stderr, "error: numNumbers != %i (numNumbers == %i)\n", a - numNumbersToRemove1, numNumbers);
		return 0;
	}
	for(int i = 0; i < numNumbers; ++i)
	{
		int b = numNumbersToRemove1 + i;
		int c = (b + 3) * (b + 3);
		if(numbers[i] != c)
		{
			fprintf(stderr, "error: numbers[%i] != %i (numbers[%i] == %i)\n", i, c, i, numbers[i]);
			return 0;
		}
	}
	
	int numNumbersToRemove2 = 4;
	
	am_remove_last_num_elements2(&numNumbers, &numbers, numNumbersToRemove2);
	if(numNumbers != a - numNumbersToRemove1 - numNumbersToRemove2)
	{
		fprintf(stderr, "error: numNumbers != %i (numNumbers == %i)\n", a - numNumbersToRemove1 - numNumbersToRemove2, numNumbers);
		return 0;
	}
	for(int i = 0; i < numNumbers; ++i)
	{
		int b = numNumbersToRemove1 + i;
		int c = (b + 3) * (b + 3);
		if(numbers[i] != c)
		{
			fprintf(stderr, "error: numbers[%i] != %i (numbers[%i] == %i)\n", i, c, i, numbers[i]);
			return 0;
		}
	}
	
	am_remove_elements2(&numNumbers, &numbers);
	
	return 1;
}

// NOTE: tests am_remove_num_elements_at
int test_10()
{
	int numNumbers = 12;
	//int* numbers = new int[numNumbers];
	int* numbers = (int*)malloc(sizeof(int) * numNumbers);
	for(int i = 0; i < numNumbers; ++i)
	{
		numbers[i] = (i + 3) * (i + 3);
	}
	
	int indexToRemoveNumbersAt = 6;
	int numNumbersToRemove = 3;
	
	int a = numNumbers;
	am_remove_num_elements_at2(&numNumbers, &numbers, indexToRemoveNumbersAt, numNumbersToRemove);
	if(numNumbers != a - numNumbersToRemove)
	{
		fprintf(stderr, "error: numNumbers != %i (numNumbers == %i)\n", a - numNumbersToRemove, numNumbers);
		return 0;
	}
	for(int i = 0; i < indexToRemoveNumbersAt; ++i)
	{
		int b = (i + 3) * (i + 3);
		if(numbers[i] != b)
		{
			fprintf(stderr, "error: numbers[%i] != %i (numbers[%i] == %i)\n", i, b, i, numbers[i]);
			return 0;
		}
	}
	for(int i = indexToRemoveNumbersAt; i < numNumbers; ++i)
	{
		int b = i + numNumbersToRemove;
		int c = (b + 3) * (b + 3);
		if(numbers[i] != c)
		{
			fprintf(stderr, "error: numbers[%i] != %i (numbers[%i] == %i)\n", i, c, i, numbers[i]);
			return 0;
		}
	}
	
	am_remove_elements2(&numNumbers, &numbers);
	
	return 1;
}

// NOTE: test whether am_*_(one_)element (i.e. macros) compile
int test_11()
{
	int numNumbers;
	int* numbers;

	int a = 1;
	am_add_element2(&numNumbers, &numbers, &a);
	int b = 2;
	am_append_element2(&numNumbers, &numbers, &b);
	int c = 3;
	am_append_or_add_element2(&numNumbers, &numbers, &c);
	
	am_remove_elements2(&numNumbers, &numbers);
	
	am_add_one_element2(&numNumbers, &numbers);
	am_append_one_element2(&numNumbers, &numbers);
	am_append_or_add_one_element2(&numNumbers, &numbers);

	am_remove_first_element2(&numNumbers, &numbers);
	am_remove_element_at2(&numNumbers, &numbers, 1);
	am_remove_last_element2(&numNumbers, &numbers);
	
	return 1;
}

//*****************************************************************************

int main(int argc, char** argv)
{
	TM_TEST2(1)
	TM_TEST2(2)
	TM_TEST2(3)
	TM_TEST2(4)
	TM_TEST2(5)
	TM_TEST2(6)
	TM_TEST2(7)
	TM_TEST2(8)
	TM_TEST2(9)
	TM_TEST2(10)
	TM_TEST2(11)
	
	return 0;
}
