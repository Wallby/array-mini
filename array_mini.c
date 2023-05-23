#include "array_mini.h"

#include <string.h>
#include <stdlib.h>


//template<type A>
//void am_add_num_elements(int* numElements, A** elements, int newNumElements)
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
	//*elements = (void*)new char[elementSize * ((*numElements) + 1)];
	*elements = malloc(elementSize * ((*numElements) + 1));	
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
