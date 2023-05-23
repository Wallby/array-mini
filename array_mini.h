#ifndef ARRAY_MINI_H
#define ARRAY_MINI_H


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

// NOTE: see am_add_(num_)elements and am_append_(num_)elements
// NOTE: assumes..
//       .. numElementsToAppend > 0
void am_append_or_add_num_elements(int elementSize, int* numElements, void** elements, int numElementsToAppendOrAdd);
#define am_append_or_add_num_elements2(numElements, elements, numElementsToAppendOrAdd) am_append_or_add_num_elements(sizeof **elements, numElements, (void**)elements, numElementsToAppendOrAdd)
void am_append_or_add_elements(int elementSize, int* numElements, void** elements, int numElementsToAppendOrAdd, void* elementsToAppendOrAdd);
#define am_append_or_add_elements2(numElements, elements, numElementsToAppendOrAdd, elementsToAppendOrAdd) am_append_or_add_elements(sizeof **elements, numElements, (void**)elements, numElementsToAppendOrAdd, (void*)elementsToAppendOrAdd)

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

#endif