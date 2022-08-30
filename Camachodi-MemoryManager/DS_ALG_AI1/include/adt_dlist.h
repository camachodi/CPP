// adt_dlist.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_DLIST_H__
#define __ADT_DLIST_H__

#include "adt_memory_node.h"

typedef struct adt_dllist {
	MemoryNode *head_;
	MemoryNode *tail_; // Corresponds to the first free place of the dlist
  u16 length_;
	u16 capacity_;
	struct dlist_ops_s *ops_;
} DLList;

struct dlist_ops_s {

  /** @brief Destroys the dlist and its data
  * 
  *   Destroy the dlist an all of it data content
  *
  *   @return KErrorCode_Vector_VectorNULL
  *   @return KErrorCode_Vector_StorageNULL
  *   @return kErrorCode_Ok
  *   @param dlist The dlist which we destroy
  */

	s16 (*destroy)(DLList *dlist);

  /** @brief Soft resets dlist data
  *
  *   Soft resets of all dlist data to 0
  *
  *   @return KErrorCode_Vector_VectorNULL
  *   @return KErrorCode_Vector_StorageNULL
  *   @return kErrorCode_Ok
  *   @param dlist The dlist which we soft reset
  */

  s16 (*softReset)(DLList *dlist);	

  /** @brief Resets dlist data
  *
  *   Reset of all the list data
  *
  *   @return KErrorCode_Vector_VectorNULL
  *   @return KErrorCode_Vector_StorageNULL
  *   @return KErrorCode_List_reset_headNULL
  *   @return kErrorCode_Ok
  *   @param dlist The dlist which we reset
  */

  s16 (*reset)(DLList *dlist);	

  /** @brief Resizes the capacity of the dlist.
  *
  *   Resizes the all capacity of the dlist, maybe some elements can be lost
  *
  *   @return NULL KErrorCode_Vector_VectorNULL
  *   @return NULL KErrorCode_Vector_StorageNULL
  *   @return NULL KErrorCode_List_resize_headNULL
  *   @return NULL kErrorCode_Ok
  *   @param dlist The dlist which we resize
  *   @param new_size The List's new size
  */

	s16 (*resize)(DLList *dlist, u16 new_size);

  /** @brief Returns the maximum number of elements to store
  *
  *   Returns the maximum number of elements to store in the dlist
  *
  *   @return KErrorCode_Vector_VectorNULL
  *   @param dlist The dlist which we need the capacity to return
  */

	u16 (*capacity)(DLList *dlist);

  /** @brief Current number of elements
  *
  *   Current number of elements in the list, this must be less than the capacity
  *
  *   @return KErrorCode_Vector_VectorNULL
  *   @param dlist The dlist which we need the length
  */

	u16 (*length)(DLList *dlist);

  /** @brief Know if dlist is empty
  *
  *   We need to know if dlist is empty
  *
  *   @return dlist->length_ == 0
  *   @param dlist The dlist which we need to dlist is full
  */

	bool (*isEmpty)(DLList *dlist);

  /** @brief Know if dlist is full
  *
  *   We need to know if dlist is full
  *
  *   @return dlist->length_ == dlist->capacity_
  *   @param dlist The dlist which we need to dlist is full
  */

	bool (*isFull)(DLList *dlist);

  /** @brief Returns a reference to the first element of the dlist
  *
  *   We need to know the first element of the dlist
  *
  *   @return NULL
  *   @return dlist->head_->data_
  *   @param dlist The dlist 
  */

	void* (*first)(DLList *dlist); 

  /** @brief Returns a reference to the last element of the dlist
  *
  *   We need to know the last element of the dlist
  *
  *   @return NULL
  *   @return dlist->tail_->data_
  *   @param dlist The dlist
  */

	void* (*last)(DLList *dlist); 

  /** @brief Returns a reference to the element at a given position
  *
  *   We need to Returns a reference to the element at a given position
  *
  *   @return NULL
  *   @return node != NULL
  *   @return node->data_ : NULL
  *   @param dlist The dlist
  *   @param position The position of the element
  */

	void* (*at)(DLList *dlist, u16 position); 

  /** @brief Inserts an element in the first free position of the dlist
  *
  *   We need to insert an element on the first position of the dlist
  *
  *   @return KErrorCode_Vector_VectorNULL
  *   @return KErrorCode_Vector_DataNULL
  *   @return KErrorCode_Vector_StorageNULL
  *   @return KErrorCode_Vector_inserFirst_BytesZero
  *   @return kErrorCode_Memory
  *   @return kErrorCode_Ok
  *   @param dlist The dlist
  *   @param data The data of the dlist
  *   @param bytes The size of the data dlist
  */

	s16 (*insertFirst)(DLList *dlist, void *data, u16 bytes); 

  /** @brief Inserts an element in the last free position of the dlist
  *
  *   We need to insert an element on the last position of the dlist
  *
  *   @return KErrorCode_Vector_VectorNULL
  *   @return KErrorCode_Vector_DataNULL
  *   @return KErrorCode_Vector_inserFirst_BytesZero
  *   @return KErrorCode_Vector_StorageNULL
  *   @return kErrorCode_Memory
  *   @return kErrorCode_Ok
  *   @param dlist The dlist
  *   @param data The data of the dlist
  *   @param bytes The size of the data dlist
  */

	s16 (*insertLast)(DLList *dlist, void *data, u16 bytes); 

  /** @brief Inserts an element at the given position of the dlist
  *
  *   We need to Inserts an element at the given position of the dlist
  *
  *   @return KErrorCode_Vector_VectorNULL
  *   @return dlist->ops_->insertLast(dlist, data, bytes)
  *   @return dlist->ops_->insertFirst(dlist, data, bytes)
  *   @return KErrorCode_Vector_DataNULL
  *   @return KErrorCode_Vector_inserFirst_BytesZero
  *   @return KErrorCode_Vector_StorageNULL
  *   @return kErrorCode_Memory
  *   @return kErrorCode_Ok
  *   @param dlist The dlist
  *   @param data The data of the dlist
  *   @param bytes The size of the data dlist
  *   @param position The position of the dlist
  */

	s16 (*insertAt)(DLList *dlist, void *data, u16 bytes, u16 position); 

  /** @brief Extracts the first element of the dlist
  *
  *   We need to Extracts the first element of the dlist
  *
  *   @return NULL
  *   @return data
  *   @param dlist The dlist
  */

	void* (*extractFirst)(DLList *dlist);

  /** @brief Extracts the last element of the dlist
  *
  *   We need to Extracts the last element of the dlist
  *
  *   @return NULL
  *   @return dlist->ops_->extractFirst(dlist)
  *   @return data
  *   @param dlist The dlist
  */

	void* (*extractLast)(DLList *dlist); 

  /** @brief Extracts the element of the dlist at the given position
  *
  *   We need to Extracts the element of the dlist at the given position
  *
  *   @return NULL
  *   @return dlist->ops_->extractLast(dlist)
  *   @return dlist->ops_->extractFirst(dlist)
  *   @return data
  *   @param dlist The dlist
  *   @param position The position of the element to extract
  */

	void* (*extractAt)(DLList *dlist, u16 position); 

  /** @brief Concatenates two lists
  *
  *   We need to Concatenates two lists
  *
  *   @return KErrorCode_Vector_VectorNULL
  *   @return KErrorCode_Vector_Vectorsrc_NULL
  *   @return KErrorCode_List_concat_tailNULL
  *   @return kErrorCode_Ok
  *   @param dlist The first dlist to concat
  *   @param list_src The second dlist to concat
  */

	s16 (*concat)(DLList *dlist, DLList *list_src);


  /** @brief Calls to a function from all elements of the dlist
  *
  *   We need to Calls to a function from all elements of the dlist
  *
  *   @return KErrorCode_Vector_VectorNULL
  *   @return kErrorCode_callback
  *   @return kErrorCode_Ok
  *   @param dlist The dlist  which elements are called
  *   @param MemoryNode * callback function to applied on a node
  */

	s16 (*traverse)(DLList *dlist, void (*callback)(MemoryNode *));
  
  /** @brief Prints the features and content of the dlist
  *
  *   Prints the content of the dlist in a message in console
  *
  *   @param dlist The dlist  which information are printed
  */

	void (*print)(DLList *dlist);
};

/** @brief Creates a new dlist
*
*   @return NULL
*   @return dlist
*   @param capacity Determine the capacity of the dlist
*/

DLList* DLLIST_create(u16 capacity); // Creates a new dlist

MemoryNode* DLIST_index(DLList* dlist, u16 position);

#endif //__ADT_DLIST_H__
