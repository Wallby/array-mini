#ifndef ARRAY_MINI_H
#define ARRAY_MINI_H


//********************************* analyzing *********************************

// NOTE: returns 1 if not different
//       returns 0 if different
#define am_predicate_t int(*)(void*, void*)

// NOTE: assumes..
//       .. numElements > 0
//       .. index != NULL
// NOTE: if predicate == NULL.. will use memcmp
// NOTE: int(*predicate)(void* element, void* query)
int am_search_in(int elementSize, int(*predicate)(void*, void*), int numElements, void* elements, void* query, int* index);
#define am_search_in2(predicate, numElements, elements, query, index) am_search_in(sizeof *elements, (am_predicate_t)predicate, numElements, (void*)elements, (void*)query, index)

// NOTE: assumes..
//       .. numElements > 0
// NOTE: if predicate == NULL.. will use memcmp
// NOTE: int(*predicate)(void* element, void* query)
int am_is_in(int elementSize, int(*predicate)(void*, void*), int numElements, void* elements, void* query);
#define am_is_in2(predicate, numElements, elements, query) am_is_in(sizeof *elements, (am_predicate_t)predicate, numElements, (void*)elements, (void*)query)

// NOTE: assumes..
//       .. numElements1 > 0
//       .. numElements2 > 0
//       .. numDifferences1 != NULL | numDifferences2 != NULL
//       .. lengthof indexPerDifference1 == numElements1
//       .. lengthof indexPerDifference2 == numElements2
// NOTE: if predicate == NULL.. will use memcmp
//       if numDifferences1 == NULL.. will ignore indexPerDifference1
//       otherwise indexPerDifference1 may == NULL
//       if numDifferences1 == NULL.. will ignore indexPerDifference2
//       otherwise indexPerDifference2 may == NULL
//       order of indexPerDifference1 assured to be order of elements1
//       order of indexPerDifference2 assured to be order of elements2
// NOTE: int(*predicate)(void* element1, void* element2)
void am_get_differences(int elementSize, int(*predicate)(void*, void*), int numElements1, void* elements1, int numElements2, void* elements2, int* numDifferences1, int* indexPerDifference1, int* numDifferences2, int* indexPerDifference2);
#define am_get_differences2(predicate, numElements1, elements1, numElements2, elements2, numDifferences1, indexPerDifference1, numDifferences2, indexPerDifference2) am_get_differences(sizeof *elements1, (am_predicate_t)predicate, numElements1, (void*)elements1, numElements2, (void*)elements2, numDifferences1, indexPerDifference1, numDifferences2, indexPerDifference2)

struct am_similarity_t
{
	int index1;
	int index2;
};
// NOTE: assumes..
//       .. numElements1 > 0
//       .. numElements2 > 0
//       .. numSimilarities != NULL
//       .. lengthof similarities1 == lengthof similarities2 ==..
//          .. min(numElements1, numElements2)
// NOTE: if predicate == NULL.. will use memcmp
//       if similarities1 may == NULL
//       if similarities2 may == NULL
//       order of similarities1 assured to be order of elements1
//       order of similarities2 assured to be order of elements2
// NOTE: int(*predicate)(void* element1, void* element2)
void am_get_similarities(int elementSize, int(*predicate)(void*, void*), int numElements1, void* elements1, int numElements2, void* elements2, int* numSimilarities, struct am_similarity_t* similarities1, struct am_similarity_t* similarities2);
#define am_get_similarities2(predicate, numElements1, elements1, numElements2, elements2, numSimilarities, similarities1, similarities2) am_get_similarities(sizeof *elements1, (am_predicate_t)predicate, numElements1, (void*)elements1, numElements2, (void*)elements2, numSimilarities, similarities1, similarities2)

//********************************* editing **********************************

// NOTE: add element(s) where there aren't any elements yet
// NOTE: assumes..
//       .. *numElements == 0
//       .. newNumElements > 0
void am_add_num_elements(int elementSize, int* numElements, void** elements, int newNumElements);
#define am_add_num_elements2(numElements, elements, newNumElements) am_add_num_elements(sizeof **elements, numElements, (void**)elements, newNumElements)
// NOTE: add element(s) where there aren't any elements yet
// NOTE: assumes..
//       .. *numElements == 0
//       .. newNumElements > 0
void am_add_elements(int elementSize, int* numElements, void** elements, int newNumElements, void* newElements);
#define am_add_elements2(numElements, elements, newNumElements, newElements) am_add_elements(sizeof **elements, numElements, (void**)elements, newNumElements, (void*)newElements)

// NOTE: assumes..
//       .. *numElements == 0
#define am_add_one_element(elementSize, numElements, elements) am_add_num_elements(elementSize, numElements, elements, 1)
#define am_add_one_element2(numElements, elements) am_add_one_element(sizeof **elements, numElements, (void**)elements)
#define am_add_element(elementSize, numElements, elements, elementToAdd) am_add_elements(elementSize, numElements, elements, 1, elementToAdd)
#define am_add_element2(numElements, elements, elementToAdd) am_add_element(sizeof **elements, numElements, (void**)elements, (void*)elementToAdd)

// NOTE: append element(s) where there is/are element(s)
// NOTE: assumes..
//       .. *numElements > 0
//       .. numElementsToAppend > 0
void am_append_num_elements(int elementSize, int* numElements, void** elements, int numElementsToAppend);
#define am_append_num_elements2(numElements, elements, numElementsToAppend) am_append_num_elements(sizeof **elements, numElements, (void**)elements, numElementsToAppend)
// NOTE: append element(s) where there is/are element(s)
// NOTE: assumes..
//       .. *numElements > 0
//       .. numElementsToAppend > 0
void am_append_elements(int elementSize, int* numElements, void** elements, int numElementsToAppend, void* elementsToAppend);
#define am_append_elements2(numElements, elements, numElementsToAppend, elementsToAppend) am_append_elements(sizeof **elements, numElements, (void**)elements, numElementsToAppend, elementsToAppend)

// NOTE: assumes..
//       .. *numElements > 0
#define am_append_one_element(elementSize, numElements, elements) am_append_num_elements(elementSize, numElements, elements, 1)
#define am_append_one_element2(numElements, elements) am_append_one_element(sizeof **elements, numElements, (void**)elements)
#define am_append_element(elementSize, numElements, elements, elementToAppend) am_append_elements(elementSize, numElements, elements, 1, elementToAppend)
#define am_append_element2(numElements, elements, elementToAppend) am_append_element(sizeof **elements, numElements, (void**)elements, (void*)elementToAppend)

// NOTE: see am_add_(num_)elements and am_append_(num_)elements
// NOTE: assumes..
//       .. numElementsToAppend > 0
void am_append_or_add_num_elements(int elementSize, int* numElements, void** elements, int numElementsToAppendOrAdd);
#define am_append_or_add_num_elements2(numElements, elements, numElementsToAppendOrAdd) am_append_or_add_num_elements(sizeof **elements, numElements, (void**)elements, numElementsToAppendOrAdd)
void am_append_or_add_elements(int elementSize, int* numElements, void** elements, int numElementsToAppendOrAdd, void* elementsToAppendOrAdd);
#define am_append_or_add_elements2(numElements, elements, numElementsToAppendOrAdd, elementsToAppendOrAdd) am_append_or_add_elements(sizeof **elements, numElements, (void**)elements, numElementsToAppendOrAdd, (void*)elementsToAppendOrAdd)

#define am_append_or_add_one_element(elementSize, numElements, elements) am_append_or_add_num_elements(elementSize, numElements, elements, 1)
#define am_append_or_add_one_element2(numElements, elements) am_append_or_add_one_element(sizeof **elements, numElements, (void**)elements)
#define am_append_or_add_element(elementSize, numElements, elements, elementToAppendOrAdd) am_append_or_add_elements(elementSize, numElements, elements, 1, elementToAppendOrAdd)
#define am_append_or_add_element2(numElements, elements, elementToAppendOrAdd) am_append_or_add_element(sizeof **elements, numElements, (void**)elements, (void*)elementToAppendOrAdd)

// NOTE: assumes..
//       .. *numElements >= 0
//       .. *newNumElements >= 0
void am_replace_elements(int elementSize, int* numElements, void** elements, int newNumElements, void* newElements);
#define am_replace_elements2(numElements, elements, newNumElements, newElements) am_replace_elements(sizeof **elements, numElements, (void**)elements, newNumElements, (void*)newElements)

// NOTE: assumes..
//       .. *newNumElements >= 0
void am_add_or_replace_elements(int elementSize, int* numElements, void** elements, int newNumElements, void* newElements);
#define am_add_or_replace_elements2(numElements, elements, newNumElements, newElements) am_add_or_replace_elements(sizeof **elements, numElements, (void**)elements, newNumElements, (void*)newElements)

// NOTE: remove element(s) where there is/are element(s)
// NOTE: assumes..
//       .. numElements > 0
void am_remove_elements(int elementSize, int* numElements, void** elements);
#define am_remove_elements2(numElements, elements) am_remove_elements(sizeof **elements, numElements, (void**)elements)

// NOTE: assumes..
//       .. *numElements >= firstNumElementsToRemove
//       .. firstNumElementsToRemove > 0
void am_remove_first_num_elements(int elementSize, int* numElements, void** elements, int firstNumElementsToRemove);
#define am_remove_first_num_elements2(numElements, elements, firstNumElementsToRemove) am_remove_first_num_elements(sizeof **elements, numElements, (void**)elements, firstNumElementsToRemove)
// NOTE: assumes..
//       .. *numElements >= numElementsToRemove
//       .. numElementsToRemove > 0
//       .. 0 <= index <= *numElements - numElementsToRemove
void am_remove_num_elements_at(int elementSize, int* numElements, void** elements, int index, int numElementsToRemove);
#define am_remove_num_elements_at2(numElements, elements, index, numElementsToRemove) am_remove_num_elements_at(sizeof **elements, numElements, (void**)elements, index, numElementsToRemove)
// NOTE: assumes..
//       .. *numElements >= lastNumElementsToRemove
//       .. lastNumElementsToRemove > 0
void am_remove_last_num_elements(int elementSize, int* numElements, void** elements, int lastNumElementsToRemove);
#define am_remove_last_num_elements2(numElements, elements, lastNumElementsToRemove) am_remove_last_num_elements(sizeof **elements, numElements, (void**)elements, lastNumElementsToRemove)

// NOTE: assumes..
//       .. *numElements > 0
#define am_remove_first_element(elementSize, numElements, elements) am_remove_first_num_elements(elementSize, numElements, elements, 1)
#define am_remove_first_element2(numElements, elements) am_remove_first_element(sizeof **elements, numElements, (void**)elements)
// NOTE: assumes..
//       .. *numElements > 0
//       .. 0 <= index < *numElements
#define am_remove_element_at(elementSize, numElements, elements, index) am_remove_num_elements_at(elementSize, numElements, elements, index, 1)
#define am_remove_element_at2(numElements, elements, index) am_remove_element_at(sizeof **elements, numElements, (void**)elements, index)
// NOTE: assumes..
//       .. *numElements > 0
#define am_remove_last_element(elementSize, numElements, elements) am_remove_last_num_elements(elementSize, numElements, elements, 1)
#define am_remove_last_element2(numElements, elements) am_remove_last_element(sizeof **elements, numElements, (void**)elements)

// NOTE: assumes..
//       .. numElements > 0
//       .. numElementsToAppend > 0
//       .. elements and elementsToAppend have atleast one difference
// NOTE: if predicate == NULL.. will use memcmp
// NOTE: int(*predicate)(void* element, void* elementToAppend)
void am_append_differences(int elementSize, int(*predicate)(void*, void*), int* numElements, void** elements, int numElementsToAppend, void* elementsToAppend);
#define am_append_differences2(predicate, numElements, elements, numElementsToAppend, elementsToAppend) am_append_differences(sizeof **elements, (am_predicate_t)predicate, numElements, (void**)elements, numElementsToAppend, (void*)elementsToAppend)

// NOTE: assumes..
//       .. numElements > 0
//       .. numElementsToRemove > 0
//       .. elements and elementsToRemove have atleast one similarity
// NOTE: if predicate == NULL.. will use memcmp
// NOTE: int(*predicate)(void* element, void* elementToRemove)
void am_remove_similarities(int elementSize, int(*predicate)(void*, void*), int* numElements, void** elements, int numElementsToRemove, void* elementsToRemove);
#define am_remove_similarities2(predicate, numElements, elements, numElementsToRemove, elementsToRemove) am_remove_similarities(sizeof **elements, (am_predicate_t)predicate, numElements, (void**)elements, numElementsToRemove, (void*)elementsToRemove)

#endif