// adt_list.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_LIST_H__
#define __ADT_LIST_H__

#include "adt_memory_node.h"

typedef struct adt_list {
	MemoryNode *head_;
	MemoryNode *tail_; // Corresponds to the first free place of the list
  u16 length_;
	u16 capacity_;
	struct list_ops_s *ops_;
} List;

struct list_ops_s {

  /** @brief Destroys the list and its data
  * 
  *   Destroy the list an all of it data content
  *
  *   @return NULL exception or ok
  *   @param list The list which we destroy
  */

	s16 (*destroy)(List *list);	

  /** @brief Soft resets list's data
  *
  *   reset to 0 or null all the data of the list
  *
  *   @return NULL exception or ok
  *   @return KErrorCode_List_destroy_listIsNULL
  *   @return KErrorCode_List_destroy_capacityheadNULL
  *   @param list The list which we soft reset
  */

  s16 (*softReset)(List *list);	

  /** @brief Resets list's data
  *
  *   Resets of all list data
  *
  *   @return NULL exception or ok
  *   @return KErrorCode_List_softReset_listIsNULL
  *   @return KErrorCode_List_softReset_headNULL
  *   @return KErrorCode_List_softReset_capacityNULL
  *   @param list The list which we reset
  */

  s16 (*reset)(List *list);	

  /** @brief Resizes the capacity of the list. Some elements can be lost
  *
  *   Resizes the all capacity of the list
  *   and probably some elements can be lost
  *
  *   @return NULL exception, error code or ok
  *   @return KErrorCode_List_reset_listIsNULL
  *   @return KErrorCode_List_reset_headNULL
  *   @return KErrorCode_List_reset_capacityNULL
  *   @param list The list which we resize
  *   @param new_size The List's new size
  */

	s16 (*resize)(List *list, u16 new_size);

  /** @brief Returns the capacity number of elements in the list
  *
  *   Returns the maximum number of elements to store
  *
  *   @return The maximum number of elements
  *   @return KErrorCode_List_resize_listNULL
  *   @return KErrorCode_List_resize_newsizeNULL
  *   @return KErrorCode_List_resize_headNULL
  *   @param list The list which we need the capacity
  */

	u16 (*capacity)(List *list);

  /** @brief Current number of elements
  *
  *   Current number of elements (<= capacity)
  *
  *   @return The number of the elements in the list
  *   @param list The list which we need the length
  */

	u16 (*length)(List *list);

  /** @brief Know if list is empty
  *
  *   We need to know if list is empty
  *
  *   @return Return true if length is empty, else equals the length to 0.
  *   @param list The list which we need to list is full
  */

	bool (*isEmpty)(List *list);

  /** @brief Know if list is full
  *
  *   We want to know if list is full
  *
  *   @return Return false if list is equal to null or 0
  *   @param list The list which we need to list is full
  */

	bool (*isFull)(List *list);

  /** @brief Returns a reference to the first element of the list
  *
  *   We want to know the first element of the list
  *
  *   @return Return a reference to the first element of the list if have data inside, other way it returns NULL
  *   @param list The list we want the first element
  */

	void* (*first)(List *list); 

  /** @brief Returns a reference to the last element of the list
  *
  *   We need to know the last element of the list
  *
  *   @return Return a reference to the last element of the list if the list isn't empty or equals NULL
  *   @param list The list we want the last element
  */

	void* (*last)(List *list); 

  /** @brief Returns a reference to the element at a given position
  *
  *   We need a reference to the element at a given position
  *
  *   @return Return a reference of the list in a given position
  *   @param list The list we want the element at a given position
  *   @param position The position of the element we want to know
  */

	void* (*at)(List *list, u16 position); 

  /** @brief Inserts an element in the first free position of the list
  *
  *   We need to insert an element on the first position of the list by moving the other elements one place on the right
  *
  *   @return Return Null exception, error code or ok
  *   @param list The list we want to insert the element
  *   @param data The data of the list we want to insert
  *   @param bytes The size of the data' s list
  */

	s16 (*insertFirst)(List *list, void *data, u16 bytes); 

  /** @brief Inserts an element in the last free position of the list
  *
  *   We need to insert an element on the last position of the list by adding one node at the last position of the list
  *
  *   @return Return Null exception, error code or ok
  *   @return KErrorCode_List_inserFirst_listIsNULL
  *   @return KErrorCode_List_inserFirst_dataNULL
  *   @return KErrorCode_List_inserFirst_listFull
  *   @return KErrorCode_List_inserFirst_BytesZero
  *   @return KErrorCode_List_inserFirst_nodeNULL
  *   @param list The list we want to insert the element
  *   @param data The data of the list
  *   @param bytes The size of the data to insert in the list
  */

	s16 (*insertLast)(List *list, void *data, u16 bytes); 

  /** @brief Inserts an element at the given position of the list
  *
  *   We need to Inserts an element at the given position of the list
  *
  *   @return Return Null exception, error code or ok
  *   @return KErrorCode_List_inserLast_listIsNULL
  *   @return KErrorCode_List_inserLast_dataNULL
  *   @return KErrorCode_List_inserLast_BytesZero
  *   @return KErrorCode_List_inserLast_lengthCapacity
  *   @return KErrorCode_List_inserFirst_nodeNULL
  *   @param list The list we want to insert the data
  *   @param data The data of the list
  *   @param bytes The size of the data' s list
  *   @param position The position of the list we want to insert at
  */

	s16 (*insertAt)(List *list, void *data, u16 bytes, u16 position); 

  /** @brief Extracts the first element of the list
  *
  *   We need to Extracts the first element of the list
  *
  *   @return Return a reference to the removed that is going to be removed at first position
  *   @return KErrorCode_List_insterAt_listNULL
  *   @return KErrorCode_List_insterAt_dataNULL
  *   @return KErrorCode_List_insterAt_BytesZero
  *   @return KErrorCode_List_insterAt_LenghtCapacity
  *   @return kErrorCode_Memory
  *   @return KErrorCode_List_insterAt_nodePosition
  *   @param list The list where the we are going to extract the data
  */

	void* (*extractFirst)(List *list); // Extracts the first element of the list

  /** @brief Extracts the last element of the list
  *
  *   We need to Extracts the last element of the list
  *
  *   @return Return a reference to the removed data
  *   @param list The list
  */

	void* (*extractLast)(List *list); 

  /** @brief Extracts the element of the list at the given position
  *
  *   We need to Extracts the element of the list at the given position
  *
  *   @return Return a reference to the removed data
  *   @param list The list
  *   @param position The position of the element to extract
  */

	void* (*extractAt)(List *list, u16 position); 

  /** @brief Concatenates two lists
  *
  *   We need to Concatenates two lists
  *
  *   @return Return the concatenated list
  *   @param list The first list to concat
  *   @param list_src The second list to concat
  */

	s16 (*concat)(List *list, List *list_src);

  /** @brief Calls to a function from all elements of the list
  *
  *   We need to Calls to a function from node of all elements of the list
  *
  *   @return Return Null exception, error code or ok
  *   @return KErrorCode_List_concat_listNULL
  *   @return KErrorCode_List_concat_listSrcNULL
  *   @return KErrorCode_List_concat_headNULL
  *   @param list The list  which elements are called
  *   @param MemoryNode* callback function to apply on a node
  */

	s16 (*traverse)(List *list, void (*callback)(MemoryNode *));
  
  /** @brief Prints the features and content of the list
  *
  *   Prints the content of the list that we want to know in the console
  *   @return KErrorCode_List_traverse_listNULL
  *   @return KErrorCode_List_traverse_lengthNULL
  *   @return KErrorCode_List_traverse_headNULL
  *   @param list The list  which information are printed
  */

	void (*print)(List *list); // Prints the features and content of the list
};

/** @brief Creates a new list
*
*   @return return the list
*   @param capacity Determine the capacity of the list
*/

List* LIST_create(u16 capacity); // Creates a new list

MemoryNode* LIST_index(List* list, u16 position);

#endif //__ADT_LIST_H__
