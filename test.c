#define AM_SAFETY
#include "array_mini.h"

#include <test_mini.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define min(a, b) a < b ? a : b


//#define CALL(a) a; fputs(#a "\n", stdout)

//********************************* analyzing *********************************

// NOTE: tests am_search_first_in
//       ^
//       am_is_in not tested as is implemented using am_search_first_in
int numTimesTest1Ran = 0;
int test_1()
{
	int numNumbers = 19;
	int numbers[numNumbers];// = { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37 };
	for(int i = 0; i < numNumbers; ++i)
	{
		numbers[i] = 1 + 2 * i;
	}
	
	int maxNumTimesTestCanRun = ceilf((float)numNumbers / 2.0f);
	int query = 1 + 4 * (numTimesTest1Ran % maxNumTimesTestCanRun); //< 1 -> 5 -> 9 -> 13 -> 17 -> 21 -> 25 -> 29 -> 33 -> 37
	int expectedIndex = 2 * (numTimesTest1Ran % maxNumTimesTestCanRun);
	
	int index;
	if(am_search_first_in2(NULL, numNumbers, numbers, &query, &index) == 0)
	{
		fprintf(stderr, "error: am_search_first_in2(NULL, &numNumbers, numbers, &%i, &index) == 0\n", query);
		
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

// NOTE: tests am_search_in
int numTimesTest2Ran = 0;
int test_2()
{
	int numNumbers = 18;
	int numbers[/*numNumbers*/] = { 1, 2, 3, 1, 3, 2, 2, 1, 3, 2, 3, 1, 3, 1, 2, 3, 2, 1 };
	
	int query = 1 + (numTimesTest2Ran % 3);
	
	int numExpectedIndices = numNumbers / 3;
	int expectedIndices[/*3*/][/*numExpectedIndices*/6] = {
			{ 0, 3, 7, 11, 13, 17 }, //< 1
			{ 1, 5, 6, 9, 14, 16 }, //< 2
			{ 2, 4, 8, 10, 12, 15 } //< 3
		};
	
	int numIndices;
	int indices[numNumbers];
	//if(am_search_in2(NULL, numNumbers, numbers, &query, &numIndices, &indices) == 0)
	// NOTE: see comment in am_remove_similarities (same applies here)
	int* a = indices;
	if(am_search_in2(NULL, numNumbers, numbers, &query, &numIndices, &a) == 0)
	{
		fprintf(stderr, "am_search_in2(NULL, numIndices, indices, &%i, &numIndices, indices) == 0\n", query);
		
		return 0;
	}
	
	if(numIndices != numExpectedIndices)
	{
		fprintf(stderr, "numIndices != %i (numIndices == %i)\n", numExpectedIndices, numIndices);
		
		return 0;
	}
	
	for(int i = 0; i < numIndices; ++i)
	{
		if(indices[i] != expectedIndices[query - 1][i])
		{
			fprintf(stderr, "indices[%i] != %i (indices[%i] == %i)\n", i, expectedIndices[query - 1][i], i, indices[i]);
			
			return 0;
		}
	}
	
	++numTimesTest2Ran;
	
	return 1;
}

// NOTE: tests..
//       .. am_get_uniques
//       .. am_get_duplicates
//int numTimesTest3Ran = 0;
int test_3()
{
	int numNumbers = 9;
	int numbers[/*numNumbers*/] = { 0, 1, 6, 0, 1, 9, 0, 1, 12 }; //< if numTimesTest3Ran % 3 == 0.. 0,1,6,0,1,9,0,1,12
	
	int numExpectedUniques = numNumbers / 3; //< works as numNumbers % 3 == 0
	int expectedIndexPerUnique[/*numExpectedUniques*/] = { 2, 5, 8 };
	
	int numExpectedDuplicates = numNumbers - numExpectedUniques;
	struct am_duplicate_t expectedDuplicates[numExpectedDuplicates];
	expectedDuplicates[0].numOccurrences = 3;
	expectedDuplicates[0].index = 0;
	expectedDuplicates[1].index = 3;
	expectedDuplicates[2].index = 6;
	expectedDuplicates[3].numOccurrences = 3;
	expectedDuplicates[3].index = 1;
	expectedDuplicates[4].index = 4;
	expectedDuplicates[5].index = 7;
	
	/*
	int numExpectedUniquesThusfar = 0;
	int a = numTimesTest3Ran % 3;
	for(int i = 0; i < numNumbers; ++i)
	{
		if((a + i) % 3 == 2)
		{
			++a;
			
			numbers[i] = 4 + i; //< offset by 4 to avoid worst case scenario of 4 + 0 being not unique
			indexPerExpectedUnique[numExpectedUniquesThusfar] = i;
			++numExpectedUniquesThusfar;
		}
		else
		{
			numbers[i] = (a + i) % 3;
		}
	}
	*/
	
	int numUniques;
	int indexPerUnique[numNumbers];
	//am_get_uniques(NULL, numNumbers, numbers, &numUniques, &indexPerUnique);
	int* b = indexPerUnique;
	am_get_uniques2(NULL, numNumbers, numbers, &numUniques, &b);
	
	if(numUniques != numExpectedUniques)
	{
		fprintf(stderr, "error: numUniques != %i (numUniques == %i)\n", numExpectedUniques, numUniques);
	
		return 0;
	}
	
	for(int i = 0; i < numUniques; ++i)
	{
		if(indexPerUnique[i] != expectedIndexPerUnique[i])
		{
			fprintf(stderr, "error: indexPerUnique[%i] != %i (indexPerUnique[%i] == %i)\n", i, expectedIndexPerUnique[i], i, indexPerUnique[i]);
		
			return 0;
		}
	}
	
	int numDuplicates;
	struct am_duplicate_t duplicates[numNumbers];
	//am_get_duplicates2(NULL, numNumbers, numbers, &numDuplicates, &c);
	struct am_duplicate_t* c = duplicates;
	am_get_duplicates2(NULL, numNumbers, numbers, &numDuplicates, &c);
	
	if(numDuplicates != numExpectedDuplicates)
	{
		fprintf(stderr, "error: numDuplicates != %i (numDuplicates == %i)\n", numExpectedDuplicates, numDuplicates);
	
		return 0;
	}
	
	for(int i = 0; i < numDuplicates; i += duplicates[i].numOccurrences)
	{
		if(duplicates[i].numOccurrences != expectedDuplicates[i].numOccurrences)
		{
			fprintf(stderr, "error: duplicates[%i].numOccurrences != %i (duplicates[%i].numOccurrences == %i)\n", i, expectedDuplicates[i].numOccurrences, i, duplicates[i].numOccurrences);
		
			return 0;
		}
		
		for(int j = i; j < duplicates[i].numOccurrences; ++j)
		{
			if(duplicates[j].index != expectedDuplicates[j].index)
			{
				fprintf(stderr, "error: duplicates[%i].index != %i (duplicates[%i].index == %i)\n", j, expectedDuplicates[i].index, j, duplicates[i].index); 
			
				return 0;
			}
		}
	}
	
	//++numTimesTest3Ran;
	
	return 1;
}

// NOTE: tests..
//       .. am_get_differences
//       .. am_get_similarities
int test_4()
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
	//am_get_differences2(NULL, numNumbers1, numbers1, numNumbers2, numbers2, &numDifferences1, &indexPerDifference1, &numDifferences2, &indexPerDifference2);
	// NOTE: see comment in am_remove_similarities (same applies here)
	int* a = indexPerDifference1;
	int* b = indexPerDifference2;
	am_get_differences2(NULL, numNumbers1, numbers1, numNumbers2, numbers2, &numDifferences1, &a, &numDifferences2, &b);
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
	int numSimilarities1;
	struct am_similarity_t similarities1[numMaxSimilarities];
	int numSimilarities2;
	struct am_similarity_t similarities2[numMaxSimilarities];
	
	//am_get_similarities2(NULL, numNumbers1, numbers1, numNumbers2, numbers2, &numSimilarities1, &similarities1, &numSimilarities2, &similarities2);
	// NOTE: see comment in am_remove_similarities (same applies here)
	struct am_similarity_t* c = similarities1;
	struct am_similarity_t* d = similarities2;
	am_get_similarities2(NULL, numNumbers1, numbers1, numNumbers2, numbers2, &numSimilarities1, &c, &numSimilarities2, &d);
	// ^
	// numSimilarities1 == numSimilarities2 == 3
	// similarities1 == { { 0, 0 }, { 1, 2 }, { 3, 1 } }; //< numbers 1, 2, 4
	// similarities2 == { { 0, 0 }, { 3, 1 }, { 1, 2 } }; //< numbers 1, 4, 2
	
	int numExpectedSimilarities = 3;
	struct am_similarity_t expectedSimilarities1[] = { { 0, 0 }, { 1, 2 }, { 3, 1 } };
	struct am_similarity_t expectedSimilarities2[] = { { 0, 0 }, { 3, 1 }, { 1, 2 } };
	if(numSimilarities1 != numExpectedSimilarities)
	{
		fprintf(stderr, "error: numSimilarities1 != %i (numSimilarities1 == %i)\n", numExpectedSimilarities, numSimilarities1);
		
		return 0;
	}
	if(numSimilarities2 != numExpectedSimilarities)
	{
		fprintf(stderr, "error: numSimilarities2 != %i (numSimilarities2 == %i)\n", numExpectedSimilarities, numSimilarities2);
		return 0;
	}
	int numSimilarities = numExpectedSimilarities;
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

//********************************** editing *********************************

// NOTE: tests..
//       .. am_add_num_elements
//       .. am_remove_elements
int test_5()
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
int test_6()
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
int test_7()
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
int test_8()
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

// NOTE: tests am_add_or_append_num_elements
int test_9()
{
	int numNumbers = 0;
	int* numbers;
	
	int numNewNumbers1 = 5;
	
	am_add_or_append_num_elements2(&numNumbers, &numbers, numNewNumbers1);
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
	
	am_add_or_append_num_elements2(&numNumbers, &numbers, numNewNumbers2);
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
// NOTE: tests am_add_or_append_elements
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
	
	am_add_or_append_elements2(&numNumbers, &numbers, numNewNumbers1, newNumbers1);
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
	
	am_add_or_append_elements2(&numNumbers, &numbers, numNewNumbers2, newNumbers2);
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
int test_11()
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
int test_12()
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
int test_13()
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
int test_14()
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
int test_15()
{
	int numNumbers;
	int* numbers;

	int a = 1;
	am_add_element2(&numNumbers, &numbers, &a);
	int b = 2;
	am_append_element2(&numNumbers, &numbers, &b);
	int c = 3;
	am_add_or_append_element2(&numNumbers, &numbers, &c);
	
	am_remove_elements2(&numNumbers, &numbers);
	
	am_add_one_element2(&numNumbers, &numbers);
	am_append_one_element2(&numNumbers, &numbers);
	am_add_or_append_one_element2(&numNumbers, &numbers);

	am_remove_first_element2(&numNumbers, &numbers);
	am_remove_element_at2(&numNumbers, &numbers, 1);
	am_remove_last_element2(&numNumbers, &numbers);
	
	return 1;
}

//************************ both analyzing and editing *************************

// NOTE: tests am_append_differences
int test_16()
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
int test_17()
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

// NOTE: tests am_remove
int numTimesTest18Ran = 0;
int test_18()
{
	int numNumbers = 10 + (numTimesTest18Ran % 3);
	int* numbers = (int*)malloc(sizeof(int) * numNumbers);
	for(int i = 0; i < numNumbers; ++i)
	{
		if(i % 3 == 0)
		{
			numbers[i] = -1;
		}
		else
		{
			numbers[i] = i;
		}
	}
	
	int numExpectedNumbersRemoved = ceilf(((float)numNumbers) / 3.0f); //< as "== 0" always ceil
	
	int numNumbersRemoved;
	int query = -1;
	am_remove2(NULL, &numNumbers, &numbers, &query, &numNumbersRemoved);
	// ^
	// not a problem to use &numbers here because is "int* numbers" not "int..
	// .. numbers[<..>]" unlike in test_2 where is "int <..> [<..>]" because..
	// .. won't be reallocated

	int bSuccess = 1;
	do
	{
		if(numNumbersRemoved != numExpectedNumbersRemoved)
		{
			fprintf(stderr, "error: numNumbersRemoved != %i (numNumbersRemoved == %i)\n", numExpectedNumbersRemoved, numNumbersRemoved);
		
			bSuccess = 0;
			break;
		}
		
		for(int i = 0, j = 0; i < numNumbers; ++i)
		{
			if(i % 3 == 0)
			{
				continue;
			}
			
			if(numbers[j] != i)
			{
				fprintf(stderr, "error: numbers[%i] != %i (numbers[%i] == %i)\n", j, i, j, numbers[j]);
				
				bSuccess = 0;
				break;
			}
			
			++j;
		}
		/*
		if(bSuccess == 0)
		{
			break;
		}
		*/
	} while(0);
	//delete numbers;
	free(numbers);
	if(bSuccess == 0)
	{
		return 0;
	}
	
	++numTimesTest18Ran;
	
	return 1;
}

/*
// NOTE: tests am_move_to_front
int test_19()
{
	//... //< am_move_to_front is not implemented

	return 1;
}
*/

// NOTE: tests am_move_to_back
int numTimesTest20Ran = 0;
int test_20()
{
	int numNumbers = 10 + (numTimesTest20Ran % 3);
	int numbers[numNumbers];
	for(int i = 0; i < numNumbers; ++i)
	{
		if(i % 3 == 0)
		{
			numbers[i] = -1;
		}
		else
		{
			numbers[i] = i;
		}
	}
	
	int numExpectedNumbersMovedToBack = ceilf(((float)numNumbers) / 3.0f); //< as "== 0" always ceil
	
	int numNumbersMovedToBack;
	int query = -1;
	//am_move_to_back2(NULL, numNumbers, &numbers, &numExpectedNumbersMovedToBack);
	// NOTE: see comment in am_remove_similarities (same applies here)
	int* a = numbers;
	am_move_to_back2(NULL, numNumbers, &a, &query, &numNumbersMovedToBack);
	
	if(numNumbersMovedToBack != numExpectedNumbersMovedToBack)
	{
		fprintf(stderr, "error: numNumbersMovedToBack != %i (numNumbersMovedToBack == %i)\n", numExpectedNumbersMovedToBack, numNumbersMovedToBack);
	
		return 0;
	}
	
	int numNumbersNotMovedToBack = numNumbers - numNumbersMovedToBack;
	
	//             front  back
	//             v      v
	for(int i = 0, j = 0; i < numNumbers; ++i)
	{
		if(j < numNumbersNotMovedToBack)
		{
			if(i % 3 == 0)
			{
				continue;
			}
			
			if(numbers[j] != i)
			{
				fprintf(stderr, "error: numbers[%i] != %i (numbers[%i] == %i)\n", j, i, j, numbers[j]);
				
				return 0;
			}
			
			++j;
		}
		else
		{
			if(numbers[j] != -1)
			{
				fprintf(stderr, "error: numbers[%i] != -1 (numbers[%i] == %i)\n", j, j, numbers[j]);
				
				return 0;
			}
			
			++j;
		}
	}
	
	++numTimesTest20Ran;

	return 1;
}

//*****************************************************************************

int main(int argc, char** argv)
{
	// TODO: make it such that every test uses different values every run..
	//       .. for 10 runs and then repeats?
	//       ^
	//       currently only test 1 uses 10 and test 2 uses 3

	int numRepetitions = 100;
	// ^
	// excess repetitions actually helped once to detect writing to..
	// .. unallocated pointer which resulted in "heap corruption" error in..
	// .. windows
	
	// analyzing
	TM_TEST(1, numRepetitions)
	TM_TEST(2, numRepetitions)
	TM_TEST(3, numRepetitions)
	TM_TEST(4, numRepetitions)
	// editing
	TM_TEST(5, numRepetitions)
	TM_TEST(6, numRepetitions)
	TM_TEST(7, numRepetitions)
	TM_TEST(8, numRepetitions)
	TM_TEST(9, numRepetitions)
	TM_TEST(10, numRepetitions)
	TM_TEST(11, numRepetitions)
	TM_TEST(12, numRepetitions)
	TM_TEST(13, numRepetitions)
	TM_TEST(14, numRepetitions)
	TM_TEST(15, numRepetitions)
	// both analyzing and editing
	TM_TEST(16, numRepetitions)
	TM_TEST(17, numRepetitions)
	TM_TEST(18, numRepetitions)
	//TM_TEST(19, numRepetitions)
	TM_TEST(20, numRepetitions)
	
	return 0;
}
