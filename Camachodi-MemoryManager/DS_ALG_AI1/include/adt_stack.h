// adt_stack.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_STACK_H__
#define __ADT_STACK_H__

#include "adt_vector.h"

typedef struct adt_stack_s {

  Vector *storage_;
  struct stack_ops_s *ops_;

} Stack;

struct stack_ops_s {

  /** @brief Destroys the stack and its data
*
*   Destroy the stack an all of it data content
*
*   @return NULL exception or ok
*   @param stack The stack which we destroy
*/

	s16 (*destroy)(Stack *stack);	

  /** @brief Soft resets stack's data
 *
 *   Soft resets of all stack data
 *
 *   @return NULL exception or ok
 *   @return KErrorCode_Stack_Destroy_stackNULL
 *   @return KErrorCode_Stack_Destroy_storageNULL
 *   @param stack The stack which we soft reset
 */

  s16 (*softReset)(Stack *stack);

  /** @brief Resets stack's data
*
*   Resets of all stack data
*
*   @return NULL exception or ok
*   @return KErrorCode_Stack_SoftReset_stackNULL
*   @return KErrorCode_Stack_oftReset_storageNULL
*   @param stack The vector which we reset
*/

  s16 (*reset)(Stack *stack);

  /** @brief Resizes the capacity of the stack. Some elements can be lost
*
*   Resizes the all capacity of the stack
*   and probably some elements can be lost
*
*   @return NULL exception or ok
*   @return KErrorCode_Stack_Reset_stackNULL
*   @return KErrorCode_Stack_Reset_storageNULL
*   @param vector The stack which we resize
*   @param new_size The stack's new size
*/

	s16 (*resize)(Stack *stack, u16 new_size);

  /** @brief Returns the maximum number of elemets to store
*
*   Returns the maximum number of elemets to store
*
*   @return The maximun number of elements
*   @return KErrorCode_Stack_Resize_stackNULL
*   @return KErrorCode_Stack_Rseize_storageNULL
*   @param stack The stack which we need the capacity
*/

	s16 (*capacity)(Stack *stack);

  /** @brief Current number of elements
*
*   Current number of elements (<= capacity)
*
*   @return The current number of elements
*   @param stack The stack which we need the leght
*/
 
	s16 (*length)(Stack *stack);

  /** @brief call insterlast of vector
*
*   We need to call insertlat function of vector
*
*   @return Return the insertLast
*   @param stack
*/

  s16(*push)(Stack *stack);

  /** @brief call last of vector
*
*   We need to call last function of vector
*
*   @return Return the last
*   @return KErrorCode_Stack_Push_stackNULL
*   @return KErrorCode_Stack_Push_storageNULL
*   @param stack
*/

  void*(*top)(Stack *stack);

  /** @brief call extractlast of vector
*
*   We need to call extractlast function of vector
*
*   @return Return the extractlast
*   @param stack
*/

  void*(*pop)(Stack *stack);

  /** @brief call isempty of vector
*
*   We need to call isempty function of vector
*
*   @return Return the isempty
*   @param stack
*/

	bool (*isEmpty)(Stack *stack);

  /** @brief Prove is stack is full
*
*   We need to prove if stack is full
*
*   @return Return bool true or false
*   @param stack The second stack to concat
*/

	bool (*isFull)(Stack *stack);

  /** @brief Concatenates two stacks
*
*   We need to Concatenates two stacks
*
*   @return Return the concatenated vector
*   @param stacks The fisrt vector to concat
*   @param stack_src The second stack to concat
*/

	s16 (*concat)(Stack *stack, Stack *stack_1); 

  /** @brief Prints the features and content of the stack
*
*   Prints the features and content of the stack
*   @return KErrorCode_Stack_Concat_stackNULL
*   @return KErrorCode_Stack_Concat_storageNULL
*   @param vector The stack  which information are printed
*/

	void (*print)(Stack *stack); 

};


Stack* STACK_create(u16 capacity);
#endif //__ADT_STACK_H__
