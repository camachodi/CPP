// adt_vector.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_VECTOR_H__
#define __ADT_VECTOR_H__

#include "adt_memory_node.h"

typedef struct adt_vector_s {
	u16 head_;
	u16 tail_; // Corresponds to the first free place of the vector
	u16 capacity_;
	MemoryNode *storage_;
	struct vector_ops_s *ops_;
} Vector;

struct vector_ops_s {

  /** @brief Destroys the vector and its data
  * 
  *   Destroy the vector an all of it data content
  *
  *   @return NULL exception or ok
  *   @param vector The vector which we destroy
  */

	s16 (*destroy)(Vector *vector);	

  /** @brief Soft resets vector's data
  *
  *   Soft resets of all vector data
  *
  *   @return NULL exception or ok
  *   @return KErrorCode_Vector_VectorNULL
  *   @return KErrorCode_Vector_StorageNULL
  *   @param vector The vector which we soft reset
  */

  s16 (*softReset)(Vector *vector);	

  /** @brief Resets vector's data
  *
  *   Resets of all vector data
  *
  *   @return NULL exception or ok
  *   @return KErrorCode_Vector_CapacityZero
  *   @return KErrorCode_Vector_StorageNULL
  *   @return KErrorCode_Vector_VectorNULL
  *   @param vector The vector which we reset
  */

  s16 (*reset)(Vector *vector);	

  /** @brief Resizes the capacity of the vector. Some elements can be lost
  *
  *   Resizes the all capacity of the vector
  *   and probably some elements can be lost
  *
  *   @return NULL exception or ok
  *   @return KErrorCode_Vector_StorageNULL
  *   @param vector The vector which we resize
  *   @param new_size The Vector's new size
  */

	s16 (*resize)(Vector *vector, u16 new_size);

  /** @brief Returns the maximum number of elemets to store
  *
  *   Returns the maximum number of elemets to store
  *
  *   @return The maximun number of elements
  *   @param vector The vector which we need the capacity
  */

	u16 (*capacity)(Vector *vector);

  /** @brief Current number of elements
  *
  *   Current number of elements (<= capacity)
  *
  *   @return The current number of elements 
  *   @param vector The vector which we need the leght
  */

	u16 (*length)(Vector *vector);

  /** @brief Know if vector is empty
  *
  *   We need to know if vector is empty
  *
  *   @return Return true if lenght = 0
  *   @param vector The vector which we need to vector is full
  */

	bool (*isEmpty)(Vector *vector);

  /** @brief Know if vector is full
  *
  *   We need to know if vector is full
  *
  *   @return Return true if lenght == capacity
  *   @param vector The vector which we need to vector is full
  */

	bool (*isFull)(Vector *vector);

  /** @brief Returns a reference to the first element of the vector
  *
  *   We need to know the first element of the vector
  *
  *   @return Return a reference to the first element of the vector
  *   @param vector The vector 
  */

	void* (*first)(Vector *vector); 

  /** @brief Returns a reference to the last element of the vector
  *
  *   We need to know the lastelement of the vector
  *
  *   @return Return a reference to the last element of the vector
  *   @param vector The vector
  */

	void* (*last)(Vector *vector); 

  /** @brief Returns a reference to the element at a given position
  *
  *   We need to Returns a reference to the element at a given position
  *
  *   @return Return a reference of the vector in a given position
  *   @param vector The vector
  *   @param position The position of the element
  */

	void* (*at)(Vector *vector, u16 position); 

  /** @brief Inserts an element in the first free position of the vector
  *
  *   We need to insert an element on the first position of the vector
  *
  *   @return Return Null exception or ok
  *   @param vector The vector
  *   @param data The data of the vector
  *   @param bytes The size of the data´s vector
  */

	s16 (*insertFirst)(Vector *vector, void *data, u16 bytes); 

  /** @brief Inserts an element in the last free position of the vector
  *
  *   We need to insert an element on the last position of the vector
  *
  *   @return Return Null exception or ok
  *   @return KErrorCode_Vector_VectorNULL
  *   @return KErrorCode_Vector_DataNULL
  *   @return KErrorCode_Vector_StorageNULL
  *   @return KErrorCode_Vector_inserFirst_BytesZero
  *   @return KErrorCode_Vector_inserFirst_NoMem
  *   @param vector The vector
  *   @param data The data of the vector
  *   @param bytes The size of the data´s vector
  */

	s16 (*insertLast)(Vector *vector, void *data, u16 bytes); 

  /** @brief Inserts an element at the given position of the vector
  *
  *   We need to Inserts an element at the given position of the vector
  *
  *   @return Return Null exception or ok
  *   @return KErrorCode_Vector_VectorNULL
  *   @return KErrorCode_Vector_DataNULL
  *   @return KErrorCode_Vector_StorageNULL
  *   @return KErrorCode_Vector_inserFirst_BytesZero
  *   @return KErrorCode_Vector_inserFirst_NoMem
  *   @param vector The vector
  *   @param data The data of the vector
  *   @param bytes The size of the data´s vector
  *   @param position The position of the vector
  */

	s16 (*insertAt)(Vector *vector, void *data, u16 bytes, u16 position); 

  /** @brief Extracts the first element of the vector
  *
  *   We need to Extracts the first element of the vector
  *
  *   @return Return a reference to the removed data
  *   @return KErrorCode_Vector_VectorNULL
  *   @return KErrorCode_Vector_DataNULL
  *   @return KErrorCode_Vector_StorageNULL
  *   @return KErrorCode_Vector_inserFirst_BytesZero
  *   @return KErrorCode_Vector_inserFirst_NoMem
  *   @param vector The vector
  */

	void* (*extractFirst)(Vector *vector); // Extracts the first element of the vector

  /** @brief Extracts the last element of the vector
  *
  *   We need to Extracts the last element of the vector
  *
  *   @return Return a reference to the removed data
  *   @param vector The vector
  */

	void* (*extractLast)(Vector *vector); 

  /** @brief Extracts the element of the vector at the given position
  *
  *   We need to Extracts the element of the vector at the given position
  *
  *   @return Return a reference to the removed data
  *   @param vector The vector
  *   @param position The position of the element to extract
  */

	void* (*extractAt)(Vector *vector, u16 position); 

  /** @brief Concatenates two vectors
  *
  *   We need to Concatenates two vectors
  *
  *   @return Return the concatenated vector
  *   @param vector The fisrt vector to concat
  *   @param vector_src The second vector to concat
  */

	s16 (*concat)(Vector *vector, Vector *vector_src); // Concatenates two vectors

  /** @brief Calls to a function from all elements of the vector
  *
  *   We need to Calls to a function from all elements of the vector
  *
  *   @return Return Null exception or ok
  *   @return KErrorCode_Vector_VectorNULL
  *   @return KErrorCode_Vector_StorageNULL
  *   @return KErrorCode_Vector_Vectorsrc_NULL
  *   @return KErrorCode_Vector_Vectorsrc_storage_NULL
  *   @param vector The vector  which elements are called
  *   @param vector_src callback function to aplied on a node
  */

	s16 (*traverse)(Vector *vector, void (*callback)(MemoryNode *));
  
  /** @brief Prints the features and content of the vector
  *
  *   Prints the features and content of the vector
  *
  *   @return KErrorCode_Vector_VectorNULL
  *   @return kErrorCode_callback
  *   @param vector The vector  which information are printed
  */

	void (*print)(Vector *vector); // Prints the features and content of the vector
};

/** @brief Creates a new vector
*
*   @return return the own vector
*   @param capacity Determine the capacity of the vector
*/

Vector* VECTOR_create(u16 capacity); // Creates a new vector
#endif //__ADT_VECTOR_H__
