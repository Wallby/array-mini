#include "array_mini.h"

#include <string.h>
#include <stdlib.h>

#define min(a, b) a < b ? a : b


//********************************* analyzing *********************************

//template<type A>
//void am_search_in(int(*predicate)(A*, A*), int numElements, A* elements, A* query, int* index)
int am_search_in(int elementSize, int(*predicate)(void*, void*), int numElements, void* elements, void* query, int* index)
{
	for(int i = 0; i < numElements; ++i)
	{
		void* element = elements + elementSize * i;
	
		int bAreNotDifferent;
		if(predicate == NULL)
		{
			bAreNotDifferent = memcmp(element, query, elementSize) == 0 ? 1 : 0;
		}
		else
		{
			bAreNotDifferent = predicate(element, query);
		}
		if(bAreNotDifferent == 1)
		{
			*index = i;
			return 1;
		}
	}
	
	return 0;
}

int am_is_in(int elementSize, int(*predicate)(void*, void*), int numElements, void* elements, void* query)
{
	int a;
	return am_search_in(elementSize, predicate, numElements, elements, query, &a);
}

void am_get_differences(int elementSize, int(*predicate)(void*, void*), int numElements1, void* elements1, int numElements2, void* elements2, int* numDifferences1, int* indexPerDifference1, int* numDifferences2, int* indexPerDifference2)
{
	if(numDifferences1 != NULL)
	{
		*numDifferences1 = 0;
		for(int i = 0; i < numElements1; ++i)
		{
			void* element1 = elements1 + elementSize * i;
			
			// if element1 is not in elements2
			// v
			if(am_is_in(elementSize, predicate, numElements2, elements2, element1) == 0)
			{
				if(indexPerDifference1 != NULL)
				{
					indexPerDifference1[*numDifferences1] = i;
				}
				++(*numDifferences1);
			}
		}
	}
	if(numDifferences2 != NULL)
	{
		*numDifferences2 = 0;
		for(int i = 0; i < numElements2; ++i)
		{
			void* element2 = elements2 + elementSize * i;
			
			// if element2 is not in elements1
			// v
			if(am_is_in(elementSize, predicate, numElements1, elements1, element2) == 0)
			{
				if(indexPerDifference2 != NULL)
				{
					indexPerDifference2[*numDifferences2] = i;
				}
				++(*numDifferences2);
			}
		}
	}
}

void am_get_similarities(int elementSize, int(*predicate)(void*, void*), int numElements1, void* elements1, int numElements2, void* elements2, int* numSimilarities, struct am_similarity_t* similarities1, struct am_similarity_t* similarities2)
{
	if(similarities1 != NULL)
	{
		*numSimilarities = 0;
		
		for(int i = 0; i < numElements1; ++i)
		{
			void* element1 = elements1 + elementSize * i;
		
			int index;
			if(am_search_in(elementSize, predicate, numElements2, elements2, element1, &index) == 1)
			{
				similarities1[*numSimilarities].index1 = i; //< i here is index into elements1
				similarities1[*numSimilarities].index2 = index;
				++(*numSimilarities);
			}
		}
	}
	if(similarities2 != NULL)
	{
		*numSimilarities = 0; //< if similarities1 != NULL.. calculates *numSimilarities twice
		
		for(int i = 0; i < numElements2; ++i)
		{
			void* element2 = elements2 + elementSize * i;
		
			int index;
			if(am_search_in(elementSize, predicate, numElements1, elements1, element2, &index) == 1)
			{
				similarities2[*numSimilarities].index1 = index;
				similarities2[*numSimilarities].index2 = i; //< i here is index into elements2
				++(*numSimilarities);
			}
		}
	}
}

//********************************** editing **********************************

void am_add_num_elements(int elementSize, int* numElements, void** elements, int newNumElements)
{
	*numElements = newNumElements;
	//*elements = new char[elementSize * (*numElements)];
	*elements = malloc(elementSize * (*numElements));
}
void am_add_elements(int elementSize, int* numElements, void** elements, int newNumElements, void* newElements)
{
	am_add_num_elements(elementSize, numElements, elements, newNumElements);
	memcpy(*elements, newElements, elementSize * newNumElements);}

void am_append_num_elements(int elementSize, int* numElements, void** elements, int numElementsToAppend)
{
	void* a = *elements;
	//*elements = (void*)new char[elementSize * ((*numElements) + numElementsToAppend)];
	*elements = malloc(elementSize * ((*numElements) + numElementsToAppend));
	memcpy(*elements, a, elementSize * (*numElements));
	//delete a;
	free(a);
	*numElements += numElementsToAppend;
}
void am_append_elements(int elementSize, int* numElements, void** elements, int numElementsToAppend, void* elementsToAppend)
{
	void* a = *elements;
	//*elements = (void*)new char[elementSize * ((*numElements) + numElementsToAppend)];
	*elements = malloc(elementSize * ((*numElements) + numElementsToAppend));
	memcpy(*elements, a, elementSize * (*numElements));
	memcpy(*elements + (elementSize * (*numElements)), elementsToAppend, elementSize * numElementsToAppend);
	// delete a;
	free(a);
	*numElements += numElementsToAppend;
}

void am_append_or_add_num_elements(int elementSize, int* numElements, void** elements, int numElementsToAppendOrAdd)
{
	void* a = *elements;
	//*elements = (void*)new char[elementSize * ((*numElements) + numElementsToAppendOrAdd)];
	*elements = malloc(elementSize * ((*numElements) + numElementsToAppendOrAdd));	
	if(*numElements > 0)
	{
		memcpy(*elements, a, elementSize * (*numElements));
		//delete a;
		free(a);		
	}
	*numElements += numElementsToAppendOrAdd;
}
void am_append_or_add_elements(int elementSize, int* numElements, void** elements, int numElementsToAppendOrAdd, void* elementsToAppendOrAdd)
{
	void* a = *elements;
	//*elements = (void*)new char[elementSize * ((*numElements) + numElementsToAppendOrAdd)];
	*elements = malloc(elementSize * ((*numElements) + numElementsToAppendOrAdd));
	if(*numElements > 0)
	{
		memcpy(*elements, a, elementSize * (*numElements));
		//delete a;
		free(a);
	}
	memcpy(*elements + (elementSize * (*numElements)), elementsToAppendOrAdd, elementSize * numElementsToAppendOrAdd);
	*numElements += numElementsToAppendOrAdd;
}

void am_replace_elements(int elementSize, int* numElements, void** elements, int newNumElements, void* newElements)
{
	//delete *elements;
	free(*elements);
	am_add_elements(elementSize, numElements, elements, newNumElements, newElements);
}

void am_add_or_replace_elements(int elementSize, int* numElements, void** elements, int newNumElements, void* newElements)
{
	if(*numElements > 0)
	{
		//delete *elements;
		free(*elements);
	}
	am_add_elements(elementSize, numElements, elements, newNumElements, newElements);
}

void am_remove_elements(int elementSize, int* numElements, void** elements)
{
	//delete *elements;
	free(*elements);
	*numElements = 0;
}

void am_remove_first_num_elements(int elementSize, int* numElements, void** elements, int firstNumElementsToRemove)
{
	if(*numElements == firstNumElementsToRemove)
	{
		//delete *elements;
		free(*elements);
	}
	else
	{
		void* a = *elements;
		//*elements = new char[elementSize * (*numElements - firstNumElementsToRemove)];
		*elements = malloc(elementSize * (*numElements - firstNumElementsToRemove));
		memcpy(*elements, a + (elementSize * firstNumElementsToRemove), elementSize * (*numElements - firstNumElementsToRemove));
		//delete a;
		free(a);
	}
	*numElements -= firstNumElementsToRemove;
}
void am_remove_num_elements_at(int elementSize, int* numElements, void** elements, int index, int numElementsToRemove)
{
	if(*numElements == numElementsToRemove)
	{		
		//delete *elements;
		free(*elements);
	}
	else //< elseif(*numElements > numElementsToRemove)
	{
		void* a = *elements;
		//*elements = new char[elementSize * (*numElements - numElementsToRemove)];
		*elements = malloc(elementSize * (*numElements - numElementsToRemove));
		int numElementsBefore = (index - 1) + 1;
		int numElementsAfter = *numElements - (index + numElementsToRemove);
		if(numElementsBefore > 0)
		{
			memcpy(*elements, a, elementSize * numElementsBefore);
		}
		if(numElementsAfter > 0)
		{
			memcpy(*elements + (elementSize * index), a + (elementSize * (index + numElementsToRemove)), elementSize * numElementsAfter);
		}
		//delete a;
		free(a);
	}
	*numElements -= numElementsToRemove;
}
void am_remove_last_num_elements(int elementSize, int* numElements, void** elements, int lastNumElementsToRemove)
{
	if(*numElements == lastNumElementsToRemove)
	{
		//delete *elements;
		free(*elements);
	}
	else
	{
		void* a = *elements;
		//*elements = (void*)new char[elementSize * ((*numElements) - lastNumElementsToRemove)];
		*elements = malloc(elementSize * ((*numElements) - lastNumElementsToRemove));
		memcpy(*elements, a, elementSize * ((*numElements) - lastNumElementsToRemove));
		//delete a;
		free(a);
	}
	*numElements -= lastNumElementsToRemove;
}

void am_append_differences(int elementSize, int(*predicate)(void*, void*), int* numElements, void** elements, int numElementsToAppend, void* elementsToAppend)
{
	int maxNumDifferences = min(*numElements, numElementsToAppend);
	
	int numDifferences;
	int indexPerDifference[maxNumDifferences];
	am_get_differences(elementSize, predicate, *numElements, *elements, numElementsToAppend, elementsToAppend, NULL, NULL, &numDifferences, indexPerDifference);
	
	int oldNumElements = *numElements;
	
	am_append_num_elements(elementSize, numElements, elements, numDifferences);
	
	for(int i = 0; i < numDifferences; ++i)
	{
		//(*elements)[oldNumElements + i] = elementsToAppend[indexPerDifference[i]];
		//void* element = &elements[oldNumElements + i];
		void* element = (*elements) + elementSize * (oldNumElements + i);
		//void* elementToAppend = elementsToAppend[indexPerDifference[i]];
		void* elementToAppend = elementsToAppend + elementSize * indexPerDifference[i];
		memcpy(element, elementToAppend, elementSize);
	}
}

void am_remove_similarities(int elementSize, int(*predicate)(void*, void*), int* numElements, void** elements, int numElementsToRemove, void* elementsToRemove)
{
	int maxNumSimilarities = min(*numElements, numElementsToRemove);
	
	int numSimilarities;
	struct am_similarity_t similarities[maxNumSimilarities];
	am_get_similarities(elementSize, predicate, *numElements, *elements, numElementsToRemove, elementsToRemove, &numSimilarities, similarities, NULL);
	
	// remove different elements shift element(s) if any after to the front
	// v
	int newNumElements = 0;
	for(int i = 0; i < *numElements;)
	{
		int numElementsToRemove = 0;
		for(int j = i; j < *numElements; ++j)
		{
			// if j is different.. break
			// otherwise.. ++numElementsToRemove
			// v
			//if(am_is_in([](struct similarity_t* a, int* b){ return a->index1 == *b ? 1 : 0; }, numSimilarities, similarities, j) == 1)
			int bIsDifferent = 1;
			for(int ii = 0; ii < numSimilarities; ++ii)
			{
				if(j == similarities[ii].index1)
				{
					bIsDifferent = 0;
					break;
				}
			}
			if(bIsDifferent == 1)
			{
				break;
			}
			
			++numElementsToRemove;
		}
		
		int indexToNextDifferentElement = i + numElementsToRemove;
		if(numElementsToRemove > 0)
		{
			if(indexToNextDifferentElement < (*numElements))
			{
				//void* nextDifferentElement = (*elements)[indexToNextDifferentElement];
				void* nextDifferentElement = (*elements) + elementSize * indexToNextDifferentElement;
				int numElementsRemaining = (*numElements) - indexToNextDifferentElement;
				memcpy((*elements) + elementSize * newNumElements, nextDifferentElement, elementSize * numElementsRemaining);
			}
			++newNumElements; //< the current element was removed thus += 1 for only the next different element
		}
		else
		{
			newNumElements += 2; //< the current element wasn't removed thus += 2 for both the current element and the next different element
		}
		i = indexToNextDifferentElement + 1; //< + 1 again as no reason to check a different element again
	}
	
	void* a = *elements;
	if(newNumElements > 0)
	{
		//*elements = (void*)new char[elementSize * newNumElements];
		*elements = malloc(elementSize * newNumElements);
		memcpy(*elements, a, elementSize * newNumElements);
	}
	//delete a;
	free(a);
	
	*numElements = newNumElements;
}
