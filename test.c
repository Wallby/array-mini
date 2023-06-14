#define AM_SAFETY
#include "array_mini.h"

#include <test_mini.h>

#include <stdio.h>
#include <stdlib.h>

#define min(a, b) a < b ? a : b


//#define CALL(a) a; fputs(#a "\n", stdout)

// NOTE: tests am_search_in
//       ^
//       am_is_in not tested as is implemented using am_search_in
int numTimesTest1Ran = 0;
int test_1()
{
	int numNumbers = 19;
	int numbers[numNumbers];// = { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37 };
	for(int i = 0; i < numNumbers; ++i)
	{
		numbers[i] = 1 + 2 * i;
	}
	
	int a = 1 + 4 * numTimesTest1Ran; //< 1 -> 5 -> 9 -> 13 -> 17 -> 21 -> 25 -> 29 -> 33 -> 37
	int expectedIndex = 2 * numTimesTest1Ran;
	
	int index;
	if(am_search_in2(NULL, numNumbers, numbers, &a, &index) == 0)
	{
		fprintf(stderr, "error: am_search_in2(NULL, &numNumbers, numbers, %i, &index) == 0\n", a);
		
		return 0;
	}
	
	if(index != expectedIndex)
	{
		fprintf(stderr, "error: index != %i (index == %i)\n", expectedIndex, index);
		
		return 0;
	}
	
	++numTimesTest1Ran;
	
	return 1;
}

// NOTE: tests..
//       .. am_get_differences
//       .. am_get_similarities
int test_2()
{
	int numNumbers1 = 5;
	int numbers1[numNumbers1];// = { 1, 2, 3, 4, 5 };
	numbers1[0] = 1;
	numbers1[1] = 2;
	numbers1[2] = 3;
	numbers1[3] = 4;
	numbers1[4] = 5;
	int numNumbers2 = 4;
	int numbers2[numNumbers2];// = { 1, 4, 2, 6 };
	numbers2[0] = 1;
	numbers2[1] = 4;
	numbers2[2] = 2;
	numbers2[3] = 6;
	
	int numDifferences1;
	int indexPerDifference1[numNumbers1];
	int numDifferences2;
	int indexPerDifference2[numNumbers2];
	am_get_differences2(NULL, numNumbers1, numbers1, numNumbers2, numbers2, &numDifferences1, indexPerDifference1, &numDifferences2, indexPerDifference2);
	// ^
	// numDifferences1 == 2
	// indexPerDifference1 == { 2, 4 }; //< numbers 3, 5
	// numDifferences2 == 1
	// indexPerDifference2 == { 3 }; //< number 6
	
	int numExpectedDifferences1 = 2;
	int expectedIndexPerDifference1[] = { 2, 4 };
	int numExpectedDifferences2 = 1;
	int expectedIndexPerDifference2[] = { 3 };
	if(numDifferences1 != numExpectedDifferences1)
	{
		fprintf(stderr, "error: numDifferences1 != %i (numDifferences1 == %i)\n", numExpectedDifferences1, numDifferences1);
		
		return 0;
	}
	for(int i = 0; i < numDifferences1; ++i)
	{
		if(indexPerDifference1[i] != expectedIndexPerDifference1[i])
		{
			fprintf(stderr, "error: indexPerDifference1[%i] != %i (indexPerDifference1[%i] == %i)\n", i, expectedIndexPerDifference1[i], i, indexPerDifference1[i]);
			
			return 0;
		}
	}
	if(numDifferences2 != numExpectedDifferences2)
	{
		fprintf(stderr, "error: numDifferences2 != %i (numDifferences2 == %i)\n", numExpectedDifferences2, numDifferences2);

		return 1;
	}
	for(int i = 0; i < numDifferences2; ++i)
	{
		if(indexPerDifference2[i] != expectedIndexPerDifference2[i])
		{
			fprintf(stderr, "error: indexPerDifference2[%i] != %i (indexPerDifference2[%i] == %i)\n", i, expectedIndexPerDifference2[i], i, indexPerDifference2[i]);
			
			return 0;
		}
	}
	
	int numMaxSimilarities = min(numNumbers1, numNumbers2);
	int numSimilarities;
	struct am_similarity_t similarities1[numMaxSimilarities];
	struct am_similarity_t similarities2[numMaxSimilarities];
	
	am_get_similarities2(NULL, numNumbers1, numbers1, numNumbers2, numbers2, &numSimilarities, similarities1, similarities2);
	// ^
	// numSimilarities = 3
	// similarities1 == { { 0, 0 }, { 1, 2 }, { 3, 1 } }; //< numbers 1, 2, 4
	// similarities2 == { { 0, 0 }, { 3, 1 }, { 1, 2 } }; //< numbers 1, 4, 2
	
	int numExpectedSimilarities = 3;
	struct am_similarity_t expectedSimilarities1[] = { { 0, 0 }, { 1, 2 }, { 3, 1 } };
	struct am_similarity_t expectedSimilarities2[] = { { 0, 0 }, { 3, 1 }, { 1, 2 } };
	if(numSimilarities != numExpectedSimilarities)
	{
		fprintf(stderr, "error: numSimilarities != %i (numSimilarities == %i)\n", numExpectedSimilarities, numSimilarities);
		
		return 0;
	}
	for(int i = 0; i < numSimilarities; ++i)
	{
		//if(~((similarities1[i].index1 == expectedSimilarities1[i].index1) & (similarities1[i].index2 == expectedSimilarities1[i].index2)))
		if(!((similarities1[i].index1 == expectedSimilarities1[i].index1) & (similarities1[i].index2 == expectedSimilarities1[i].index2)))
		{
			fprintf(stderr, "error: similarities1[%i] != { %i, %i } (similarities1[%i] == { %i, %i })\n", i, expectedSimilarities1[i].index1, expectedSimilarities1[i].index2, i, similarities1[i].index1, similarities1[i].index2);
		
			return 0;
		}
		//if(~((similarities2[i].index1 == expectedSimilarities2[i].index1) & (similarities2[i].index2 == expectedSimilarities2[i].index2)))
		if(!((similarities2[i].index1 == expectedSimilarities2[i].index1) & (similarities2[i].index2 == expectedSimilarities2[i].index2)))
		{
			fprintf(stderr, "error: similarities2[%i] != { %i, %i } (similarities2[%i] == { %i, %i })\n", i, expectedSimilarities2[i].index1, expectedSimilarities2[i].index2, i, similarities1[i].index1, similarities1[i].index2);
		
			return 0;
		}
	}
	
	return 1;
}

// NOTE: tests..
//       .. am_add_num_elements
//       .. am_remove_elements
int test_3()
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
int test_4()
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
int test_5()
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
int test_6()
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
int test_7()
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
int test_9()
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
int test_10()
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
int test_11()
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
int test_12()
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
int test_13()
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

// NOTE: tests am_append_differences
int test_14()
{
	int numNumbers = 5;
	//int* numbers = new int[numNumbers];
	int* numbers = (int*)malloc(sizeof(int) * numNumbers);
	for(int i = 0; i < numNumbers; ++i)
	{
		numbers[i] = (i + 1) * (i + 1);
	}
	// ^
	// 1, 4, 9, 16, 25
	
	int numNumbersToAppend = 4;
	int numbersToAppend[numNumbersToAppend];// = { 3, 9, 5, 25 };
	numbersToAppend[0] = 3;
	numbersToAppend[1] = 9;
	numbersToAppend[2] = 5;
	numbersToAppend[3] = 25;
	
	am_append_differences2(NULL, &numNumbers, &numbers, numNumbersToAppend, numbersToAppend);
	
	int numExpectedNumbers = 7;
	int expectedNumbers[numExpectedNumbers];// = { 1, 4, 9, 16, 25, 3, 5};
	expectedNumbers[0] = 1;
	expectedNumbers[1] = 4;
	expectedNumbers[2] = 9;
	expectedNumbers[3] = 16;
	expectedNumbers[4] = 25;
	expectedNumbers[5] = 3;
	expectedNumbers[6] = 5;
	if(numNumbers != numExpectedNumbers)
	{
		fprintf(stderr, "error: numNumbers != %i (numNumbers == %i)\n", numExpectedNumbers, numNumbers);
		
		return 0;
	}
	for(int i = 0; i < numNumbers; ++i)
	{
		if(numbers[i] != expectedNumbers[i])
		{
			fprintf(stderr, "error: numbers[%i] != %i (numbers[%i] == %i)\n", i, expectedNumbers[i], i, numbers[i]);
			
			return 0;
		}
	}
	
	am_remove_elements2(&numNumbers, &numbers);
	
	return 1;
}

// NOTE: tests am_remove_similarities
int test_15()
{
	int numNumbers = 5;
	//int* numbers = new int[numNumbers];
	int* numbers = (int*)malloc(sizeof(int) * numNumbers);
	for(int i = 0; i < numNumbers; ++i)
	{
		numbers[i] = (i + 1) * (i + 1);
	}
	// ^
	// 1, 4, 9, 16, 25
	
	int numNumbersToRemove = 5;
	int numbersToRemove[numNumbersToRemove];// = { 1, 2, 16, 4, 3 };
	numbersToRemove[0] = 1;
	numbersToRemove[1] = 2;
	numbersToRemove[2] = 16;
	numbersToRemove[3] = 4;
	numbersToRemove[4] = 3;
	
	am_remove_similarities2(NULL, &numNumbers, &numbers, numNumbersToRemove, numbersToRemove);
	
	int numExpectedNumbers = 2;
	int expectedNumbers[numExpectedNumbers];// = { 9, 25 };
	expectedNumbers[0] = 9;
	expectedNumbers[1] = 25;
	
	if(numNumbers != numExpectedNumbers)
	{
		fprintf(stderr, "error: numNumbers != %i (numNumbers == %i)\n", numNumbers);
		
		return 0;
	}
	for(int i = 0; i < numNumbers; ++i)
	{
		if(numbers[i] != expectedNumbers[i])
		{
			fprintf(stderr, "error: numbers[%i] != %i (numbers[%i] == %i)\n", i, expectedNumbers[i], i, numbers[i]);
			
			return 0;
		}
	}
	
	am_remove_elements2(&numNumbers, &numbers);
	
	return 1;
}

//*****************************************************************************

int main(int argc, char** argv)
{
	TM_TEST(1, 9) //< can only run max 10 times as test uses variable numTimesTest1Ran
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
	TM_TEST2(12)
	TM_TEST2(13)
	TM_TEST2(14)
	TM_TEST2(15)
	
	return 0;
}
