#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct Array {
  int capacity;  // How many elements can this array hold?
  int count;  // How many states does the array currently hold?
  char **elements;  // The string elements contained in the array
} Array;

/************************************
 *
 *   CREATE, DESTROY, RESIZE FUNCTIONS
 *
 ************************************/

/*****
 * Allocate memory for a new array
 *****/
Array *create_array (int capacity) {
  // Allocate memory for the Array struct
  Array *arr = malloc(sizeof(Array));

  // Set initial values for capacity and count
  arr->capacity = capacity;
  arr->count = 0;

  // Allocate memory for elements
  arr->elements = calloc(capacity, sizeof(char *));

  return arr;
}


/*****
 * Free memory for an array and all of its stored elements
 *****/
void destroy_array(Array *arr) {
  // Free all elements
  //Free all the string elements in a loop
  for (int i = 0; i < arr->count; i++)
  {
    free(arr->elements[i]);
  }

  free(arr->elements);
  
  // Free array
  free(arr);
}

/*****
 * Create a new elements array with double capacity and copy elements
 * from old to new
 *****/
void resize_array(Array *arr) {

  // Create a new element storage with double capacity
  int doubleCapacity = arr->capacity * 2;
  char **newElements = calloc(doubleCapacity, sizeof(char *));
  // if using realloc it doesn't need to copy elements nor freeing old elements as below
  // char **newElements = realloc(arr->elements, (doubleCapacity * sizeof(int)));

  // Copy elements into the new storage
  for (int i = 0; i < arr->count; i++)
  {
    newElements[i] = arr->elements[i];
  }
  
  // // Free the old elements array (but NOT the strings they point to)
  free(arr->elements);

  // Update the elements and capacity to new values
  arr->elements = newElements;
  arr->capacity = doubleCapacity;
}



/************************************
 *
 *   ARRAY FUNCTIONS
 *
 ************************************/

/*****
 * Return the element in the array at the given index.
 *
 * Throw an error if the index is out of range.
 *****/
char *arr_read(Array *arr, int index) {

  // Throw an error if the index is greater than the current count
  if (index >= arr->count)
  {
    fprintf(stderr, "IndexError: Index %d is out of range\n", index);
    return NULL; //exit(1); is more for bug in the code rather than input error
  }
  // Otherwise, return the element at the given index
  return arr->elements[index];
}


/*****
 * Insert an element to the array at the given index
 *****/
void arr_insert(Array *arr, char *element, int index) {

  // Throw an error if the index is greater than the current count
  if (index > arr->count)
  {
    fprintf(stderr, "IndexError: Index %d is out of range\n", index);
    exit(1);
  }

  // Resize the array if the number of elements is over capacity
  if (arr->count >= arr->capacity) // could resize before running out capacity such as (arr->count >= arr->capacity * 0.75) 
  {
    resize_array(arr);
    // fprintf(stderr, "OverCapacityError: INSERT Capacity is full and resize is not yet implemented.\n");
    // exit(2);
  }

  // Move every element after the insert index to the right one position
  for (int i = arr->count; i >= index; i--) 
  {
    arr->elements[i] = arr->elements[i - 1];
  }

  // Copy the element and add it to the array
  arr->elements[index] = strdup(element);

  // Increment count by 1
  arr->count++;
}

/*****
 * Append an element to the end of the array
 *****/
void arr_append(Array *arr, char *element) {

  // // Resize the array if the number of elements is over capacity
  // // or throw an error if resize isn't implemented yet.
  // if (arr->count >= arr->capacity)
  // {
  //   resize_array(arr);
  //   // fprintf(stderr, "OverCapacityError: APPEND Capacity is full and resize is not yet implemented.\n");
  //   // exit(2);
  // }
  // // Copy the element and add it to the end of the array
  // arr->elements[arr->count] = element;

  // // Increment count by 1
  // arr->count++;

  arr_insert(arr, element, arr->count);
}

/*****
 * Remove the first occurence of the given element from the array,
 * then shift every element after that occurence to the left one slot.
 *
 * Throw an error if the value is not found.
 *****/
void arr_remove(Array *arr, char *element) {

  int removed_index = -1;
  // Search for the first occurence of the element and remove it.
  for (int i = 0; i < arr->count; i++)
  {
    if ((strcmp(arr->elements[i], element)) == 0)
    {
      removed_index = i; //at index where desired element gets removed to be use as starting point
      // Don't forget to free its memory!
      free(arr->elements[i]);
    }
  }

  if (removed_index < 0)
  {
    fprintf(stderr, "Value %s not found\n", element);
    return;
  }

  // Shift over every element after the removed element to the left one position
  for (int i = removed_index; i < arr->count - 1; i++) //for (int i = removed_index + 1; i < arr->count; i++)
  {
    arr->elements[i] = arr->elements[i + 1]; //arr->elements[i - 1] = arr->elements[i];
  }

  // Decrement count by 1
  arr->count--;
  arr->elements[arr->count] = NULL;
}


/*****
 * Utility function to print an array.
 *****/
void arr_print(Array *arr) {
  printf("[");
  for (int i = 0 ; i < arr->count ; i++) {
    printf("%s", arr->elements[i]);
    if (i != arr->count - 1) {
      printf(",");
    }
  }
  printf("]\n");
}


#ifndef TESTING
int main(void)
{

  Array *arr = create_array(1);

  arr_insert(arr, "STRING1", 0); //[STRING1]
  arr_append(arr, "STRING4"); //[STRING1, STRING4]
  arr_insert(arr, "STRING2", 0); //[STRING2, STRING1, STRING4]
  arr_insert(arr, "STRING3", 1); //[STRING2, STRING3, STRING1, STRING4]
  arr_print(arr);
  arr_remove(arr, "STRING3"); //[STRING2, STRING1, STRING4]
  arr_print(arr);

  destroy_array(arr);

  return 0;
}
#endif
