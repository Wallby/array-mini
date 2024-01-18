#include "array_mini.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define min(a, b) a < b ? a : b


//********************************* predicates ********************************

int am_predicate_not_memcmp(void* a, void* b)
{
	fputs("error: am_predicate_not_memcmp should not be called (see comment in array_mini.c)\n", stderr);

	// don't actually call this function as elementSize is unknown here,..
	// .. instead test if predicate == am_predicate_not_memcmp
	
	return 0;
}

static int predicate2(int elementSize, int(*predicate)(void*, void*), void* element, void* query)
{
	if(predicate == NULL)
	{
		return memcmp(element, query, elementSize) == 0 ? 1 : 0;
	}
	else if(predicate == am_predicate_not_memcmp)
	{
		return memcmp(element, query, elementSize) != 0 ? 1 : 0;
	}
	else
	{
		return predicate(element, query);
	}
}

//********************************* analyzing *********************************

//template<type A>
//void am_search_first_in(int(*predicate)(A*, A*), int numElements, A* elements, A* query, int* index)
int am_search_first_in(int elementSize, int(*predicate)(void*, void*), int numElements, void* elements, void* query, int* index)
{
	for(int i = 0; i < numElements; ++i)
	{
		void* element = elements + elementSize * i;
	
		int bAreNotDifferent = predicate2(elementSize, predicate, element, query);
		if(bAreNotDifferent == 1)
		{
			*index = i;
			return 1;
		}
	}
	
	return 0;
}

int am_search_in(int elementSize, int(*predicate)(void*, void*), int numElements, void* elements, void* query, int* numIndices, int** indices)
{
	int numIndicesThusfar = 0;
	for(int i = 0; i < numElements; ++i)
	{
		void* element = elements + elementSize * i;
	
		int bAreNotDifferent = predicate2(elementSize, predicate, element, query);
		if(bAreNotDifferent == 1)
		{
			(*indices)[numIndicesThusfar] = i;
			++numIndicesThusfar;
		}
	}
	
	*numIndices = numIndicesThusfar;
	
	return *numIndices == 0 ? 0 : 1;
}

int am_is_in(int elementSize, int(*predicate)(void*, void*), int numElements, void* elements, void* query)
{
	int a;
	return am_search_first_in(elementSize, predicate, numElements, elements, query, &a);
}

void am_get_uniques(int elementSize, int(*predicate)(void*, void*), int numElements, void* elements, int* numUniques, int** indexPerUnique)
{
	int numSimilarities;
	struct am_similarity_t similarities[numElements - 1];
	//am_get_similarities(elementSize, predicate, numElements, elements, numElements, elements, &numSimilarities, &similarities, NULL, NULL);
	// NOTE: above doesn't work because &similarities actually contains the..
	//       .. same address as similarities but only the pointer is of a..
	//       .. different type. Maybe this is to avoid an implicit pointer..
	//       .. variable to be created as similarities stored on the stack..
	//       .. means a pointer variable is not required, whereas "expecting..
	//       .. &similarities to return a pointer to such a variable" thus..
	//       .. would mean creating an implicit variable
	//       ^
	//       https://www.codeproject.com/Questions/5375895/Why-is-address-of-array-not-convertible-to-in-C
	struct am_similarity_t* a = similarities;
	am_get_similarities(elementSize, predicate, numElements, elements, numElements, elements, &numSimilarities, &a, NULL, NULL);
	
	if(numSimilarities == numElements)
	{
		return;
	}
	
	int numUniquesThusfar = 0;
	for(int i = 0; i < numElements; ++i)
	{
		//if(am_is_in2([](struct am_similarity_t* a, int* query){ return a->index1 == query ? 1 : 0; }, numSimilarities, similarities, i) == 1)
		//{
		//	continue;
		//}
		int bIsDuplicate = 0;
		for(int j = 0; j < numSimilarities; ++j)
		{
			if(similarities[j].index1 == i)
			{
				bIsDuplicate = 1;
				break;
			}
		}
		if(bIsDuplicate == 1)
		{
			continue;
		}
		
		(*indexPerUnique)[numUniquesThusfar] = i;
		++numUniquesThusfar;
	}
	*numUniques = numUniquesThusfar;
}

void am_get_duplicates(int elementSize, int(*predicate)(void*, void*), int numElements, void* elements, int* numDuplicates, struct am_duplicate_t** duplicates)
{
	int numSimilarities;
	struct am_similarity_t similarities[numElements - 1];
	//am_get_similarities(elementSize, predicate, numElements, elements, numElements, elements, &numSimilarities, &similarities, NULL, NULL);
	// NOTE: see comment in am_get_uniques (same applies here)
	struct am_similarity_t* a = similarities;
	am_get_similarities(elementSize, predicate, numElements, elements, numElements, elements, &numSimilarities, &a, NULL, NULL);
	
	if(numSimilarities == 0)
	{
		return;
	}
	
	int numDuplicatesThusfar = 0;
	
	int numIndicesThusfar = 0;
	int indicesThusfar[numSimilarities];
	// ^
	// numIndices for which there is now an element in duplicates
	
	for(int i = 0; i < numElements; ++i)
	{
		//if(am_is_in2(NULL, numIndicesThusfar, indicesThusfar, i) == 1)
		int bIsIndex = 0;
		for(int j = 0; j < numIndicesThusfar; ++j)
		{
			if(indicesThusfar[j] == i)
			{
				bIsIndex = 1;
				break;
			}
		}
		if(bIsIndex == 1)
		{
			continue;
		}
		
		int numIndices;
		int indices[numSimilarities]; //< # max indices == # similarities
		
		// actually stores indices each to similarity, index to duplicate is..
		// .. similarities[indices[..]].index2
		// v
		//if(am_search_in2([](struct am_similarity_t* a, int* query){ return a->index1 == query ? 1 : 0; }, numSimilarities, similarities, i, &numIndices, &indices) == 0)
		// doesn't store indices each to similarity unlike above
		// v
		for(int j = 0; j < numSimilarities; ++j)
		{
			if(similarities[j].index1 == i)
			{
				indices[numIndices] = similarities[j].index2;
				++numIndices;
			}
		}
		if(numIndices == 0)
		{
			continue;
		}
		
		(*duplicates)[numDuplicatesThusfar].index = indices[0];
		++numDuplicatesThusfar;
		indicesThusfar[0] = indices[0];
		++numIndicesThusfar;
		for(int j = 1; j < numIndices; ++j)
		{
			(*duplicates)[numDuplicatesThusfar - 1].nextDuplicate = &(*duplicates)[numDuplicatesThusfar];
			(*duplicates)[numDuplicatesThusfar].index = indices[j];
			++numDuplicatesThusfar;
			indicesThusfar[numIndicesThusfar] = indices[j];
			++numIndicesThusfar;
		}
		(*duplicates)[numDuplicatesThusfar - 1].nextDuplicate = NULL;
	}
	*numDuplicates = numDuplicatesThusfar;
}

void am_get_differences(int elementSize, int(*predicate)(void*, void*), int numElements1, void* elements1, int numElements2, void* elements2, int* numDifferences1, int** indexPerDifference1, int* numDifferences2, int** indexPerDifference2)
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
					(*indexPerDifference1)[*numDifferences1] = i;
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
					(*indexPerDifference2)[*numDifferences2] = i;
				}
				++(*numDifferences2);
			}
		}
	}
}

void am_get_similarities(int elementSize, int(*predicate)(void*, void*), int numElements1, void* elements1, int numElements2, void* elements2, int* numSimilarities1, struct am_similarity_t** similarities1, int* numSimilarities2, struct am_similarity_t** similarities2)
{
	if(numSimilarities1 != NULL)
	{
		*numSimilarities1 = 0;
		
		for(int i = 0; i < numElements1; ++i)
		{
			void* element1 = elements1 + elementSize * i;
		
			int index;
			if(am_search_first_in(elementSize, predicate, numElements2, elements2, element1, &index) == 1)
			{
				(*similarities1)[*numSimilarities1].index1 = i; //< i here is index into elements1
				(*similarities1)[*numSimilarities1].index2 = index;
				++(*numSimilarities1);
			}
		}
	}
	if(numSimilarities2 != NULL)
	{
		*numSimilarities2 = 0;
		
		for(int i = 0; i < numElements2; ++i)
		{
			void* element2 = elements2 + elementSize * i;
		
			int index;
			if(am_search_first_in(elementSize, predicate, numElements1, elements1, element2, &index) == 1)
			{
				(*similarities2)[*numSimilarities2].index1 = index;
				(*similarities2)[*numSimilarities2].index2 = i; //< i here is index into elements2
				++(*numSimilarities2);
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

void am_add_or_append_num_elements(int elementSize, int* numElements, void** elements, int numElementsToAppendOrAdd)
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
void am_add_or_append_elements(int elementSize, int* numElements, void** elements, int numElementsToAppendOrAdd, void* elementsToAppendOrAdd)
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

//************************ both analyzing and editing *************************

void am_append_differences(int elementSize, int(*predicate)(void*, void*), int* numElements, void** elements, int numElementsToAppend, void* elementsToAppend)
{
	int maxNumDifferences = min(*numElements, numElementsToAppend);
	
	int numDifferences;
	int indexPerDifference[maxNumDifferences];
	//am_get_differences(elementSize, predicate, *numElements, *elements, numElementsToAppend, elementsToAppend, NULL, NULL, &numDifferences, &indexPerDifference);
	// NOTE: for some reason not clear to me above is not allowed but below is
	//       ^
	//       see..
	//       .. https://www.codeproject.com/Questions/5375895/Why-is-address-of-array-not-convertible-to-in-C
	int* a = indexPerDifference;
	am_get_differences(elementSize, predicate, *numElements, *elements, numElementsToAppend, elementsToAppend, NULL, NULL, &numDifferences, &a);
	
	if(numDifferences == 0)
	{
		return;
	}
	
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
	int maxNumSimilarities = min(*numElements, numElementsToRemove) - 1;
	
	int numSimilarities;
	struct am_similarity_t similarities[maxNumSimilarities];
	//am_get_similarities(elementSize, predicate, *numElements, *elements, numElementsToRemove, elementsToRemove, &numSimilarities, &similarities, NULL, NULL);
	// NOTE: see comment in am_get_uniques (same applies here)
	struct am_similarity_t* a = similarities;
	am_get_similarities(elementSize, predicate, *numElements, *elements, numElementsToRemove, elementsToRemove, &numSimilarities, &a, NULL, NULL);
	// ^
	// use similarities1 here not similarities2 as duplicates in elements is..
	// .. allowed, duplicates in elementsToRemove if any would be ignored
	
	if(numSimilarities == 0)
	{
		return;
	}
	
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
	
	void* b = *elements;
	if(newNumElements > 0)
	{
		//*elements = (void*)new char[elementSize * newNumElements];
		*elements = malloc(elementSize * newNumElements);
		memcpy(*elements, b, elementSize * newNumElements);
	}
	//delete b;
	free(b);
	
	*numElements = newNumElements;
}

void am_remove(int elementSize, int(*predicate)(void*, void*), int* numElements, void** elements, void* query, int* numElementsRemoved)
{
	*numElementsRemoved = 0;
	for(int i = 0; i < *numElements - *numElementsRemoved;)
	{
		void* element1 = *elements + elementSize * i;
		
		int bRemove1 = predicate2(elementSize, predicate, element1, query);
		if(bRemove1 == 1)
		{
			int indexToNextElementToNotRemove = -1;
			for(int j = i + 1; j < *numElements - *numElementsRemoved; ++j)
			{
				void* element2 = *elements + elementSize * j;
			
				int bRemove2 = predicate2(elementSize, predicate, element2, query);
				if(bRemove2 == 0)
				{
					indexToNextElementToNotRemove = j;
					break;
				}
			}
			if(indexToNextElementToNotRemove != -1)
			{
				int numElementsToRemove = indexToNextElementToNotRemove - i;
				
				int lastNumElementsToMoveToFront = *numElements - indexToNextElementToNotRemove;
				
				// move back elements to front
				memcpy(*elements + elementSize * i, *elements + elementSize * indexToNextElementToNotRemove, elementSize * lastNumElementsToMoveToFront);
				
				*numElementsRemoved += numElementsToRemove;
			}
			else
			{
				int numElementsToRemove = (*numElements - *numElementsRemoved) - i;
			
				*numElementsRemoved += numElementsToRemove;
			}
		}
		else
		{
			++i;
		}
	}
	if(*numElementsRemoved > 0)
	{
		int newNumElements = *numElements - *numElementsRemoved;
	
		void* a = *elements;
		if(newNumElements > 0)
		{
			//*elements = new char[elementSize * newNumElements];
			*elements = malloc(elementSize * newNumElements);
			memcpy(*elements, a, elementSize * newNumElements);
		}
		//delete a;
		free(a);
		
		*numElements = newNumElements;
	}
}

void am_move_to_front(int elementSize, int(*predicate)(void*, void*), int numElements, void** elements, void* query, int* numElementsMovedToFront)
{
	//...
}

void am_move_to_back(int elementSize, int(*predicate)(void*, void*), int numElements, void** elements, void* query, int* numElementsMovedToBack)
{
	*numElementsMovedToBack = 0;
	for(int i = 0; i < numElements - *numElementsMovedToBack;)
	{
		void* element1 = *elements + elementSize * i;
	
		int bMoveToBack1 = predicate2(elementSize, predicate, element1, query);
		if(bMoveToBack1 == 1)
		{
			int indexToNextElementToNotMoveToBack = -1;
			for(int j = i + 1; j < numElements - *numElementsMovedToBack; ++j)
			{
				void* element2 = *elements + elementSize * j;
			
				int bMoveToBack2 = predicate2(elementSize, predicate, element2, query);
				if(bMoveToBack2 == 0)
				{
					indexToNextElementToNotMoveToBack = j;
					break;
				}
			}
			if(indexToNextElementToNotMoveToBack != -1)
			{
				int numElementsToMoveToBack = indexToNextElementToNotMoveToBack - i;
				
				char elementsToMoveToBack[elementSize * numElementsToMoveToBack]; //< 0 length array is not officially allowed in C, but guaranteed that numElementsToMoveToBack is >= 1 here
				/// backup front elements
				memcpy(elementsToMoveToBack, *elements + elementSize * i, elementSize * numElementsToMoveToBack);
				
				int lastNumElementsToMoveToFront = numElements - indexToNextElementToNotMoveToBack;

				// move back elements to front
				memcpy(*elements + elementSize * i, *elements + elementSize * indexToNextElementToNotMoveToBack, elementSize * lastNumElementsToMoveToFront);
				// move front elements from backup to back
				memcpy(*elements + elementSize * (i + lastNumElementsToMoveToFront), elementsToMoveToBack, elementSize * numElementsToMoveToBack);
				
				*numElementsMovedToBack += numElementsToMoveToBack;
			}
			else
			{
				int numElementsToMoveBack = (numElements - *numElementsMovedToBack) - i;
				// ^
				// because if i == 0.. numElementsToMoveToBack should ==..
				// .. numElements - *numElementsMovedToBack
				
				// ~don't actually move these elements as already at back~
				// ^
				// do move these back (but not to back as already at back)..
				// .. to assure order is preserved
				
				char elementsToMoveBack[elementSize * numElementsToMoveBack]; //< 0 length array is not officially allowed in C, but guaranteed that numElementsToMoveToBack is >= 1 here
				memcpy(elementsToMoveBack, *elements + elementSize * i, elementSize * numElementsToMoveBack);
				
				int lastNumElementsToMoveFront = *numElementsMovedToBack;
				
				memcpy(*elements + elementSize * i, *elements + elementSize * (i + numElementsToMoveBack), elementSize * lastNumElementsToMoveFront);
				memcpy(*elements + elementSize * (i + lastNumElementsToMoveFront), elementsToMoveBack, elementSize * numElementsToMoveBack);
				
				*numElementsMovedToBack += numElementsToMoveBack;
			}
		}
		else
		{
			++i;
		}
	}
}