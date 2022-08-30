// adt_vector.c
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#include <stdio.h>
#include <stdlib.h>

#include "common_def.h"
#include "adt_stack.h"
#include "adt_memory_node.h"

#include "ABGS_MemoryManager/abgs_memory_manager.h"


static s16 STACK_destroy(Stack* stack);
static s16 STACK_softReset(Stack* stack);
static s16 STACK_reset(Stack* stack);
static s16 STACK_resize(Stack* stack, u16 new_size);
static u16 STACK_capacity(Stack* stack);
static u16 STACK_length(Stack* stack);
static bool STACK_isEmpty(Stack* stack);
static bool STACK_isFull(Stack* stack);
static void* STACK_top(Stack* stack);
static s16 STACK_push(Stack* stack, void* data, u16 bytes);
static void* STACK_pop(Stack* stack);
static s16 STACK_concat(Stack* stack, Stack* stack_1);
static void STACK_print(Stack* stack);

struct stack_ops_s stack_ops_s = {
  .reset = STACK_reset,
  .push = STACK_push,
  .isEmpty = STACK_isEmpty,
  .isFull = STACK_isFull,
  .print = STACK_print,
  .capacity = STACK_capacity,
  .length = STACK_length,
  .pop = STACK_pop,
  .resize = STACK_resize,
  .top = STACK_top,
  .destroy = STACK_destroy,
  .concat = STACK_concat
};


Stack* STACK_create(u16 capacity) {
  if (0 == capacity) {
    return NULL;
  }

  Stack* stack = MM->malloc(sizeof(Stack));

  if (NULL == stack) {
    return NULL;
  }

  stack->storage_ = stack->storage_ = VECTOR_create(capacity);
  stack->ops_ = &stack_ops_s;

  return stack;
}

s16 STACK_destroy(Stack* stack) {

  if (NULL == stack) {
    return KErrorCode_Stack_Destroy_stackNULL;
  }
  if (NULL == stack->storage_) {
    return KErrorCode_Stack_Destroy_storageNULL;
  }

  stack->storage_->ops_->destroy(stack->storage_);

  MM->free(stack);
  return kErrorCode_Ok;
}

s16 STACK_reset(Stack* stack) {

  if (NULL == stack) {
    return KErrorCode_Stack_Reset_stackNULL;
  }
  if (NULL == stack->storage_) {
    return KErrorCode_Stack_Reset_storageNULL;
  }

  return stack->storage_->ops_->reset(stack->storage_);
}

s16 STACK_softReset(Stack* stack) {

  if (NULL == stack) {
    return KErrorCode_Stack_SoftReset_stackNULL;
  }
  if (NULL == stack->storage_) {
    return KErrorCode_Stack_oftReset_storageNULL;
  }

  return stack->storage_->ops_->softReset(stack->storage_);
}

u16 STACK_capacity(Stack* stack) {

  if (NULL == stack || NULL == stack->storage_) {
    return 0;
  }

  return stack->storage_->ops_->capacity(stack->storage_);
}

u16 STACK_length(Stack* stack) {

  if (NULL == stack || NULL == stack->storage_) {
    return 0;
  }

  return stack->storage_->ops_->length(stack->storage_);
}

bool STACK_isEmpty(Stack* stack) {

  if (NULL == stack || NULL == stack->storage_) {
    return false;
  }

  return stack->storage_->ops_->isEmpty(stack->storage_);
}

bool STACK_isFull(Stack* stack) {

  if (NULL == stack || NULL == stack->storage_) {
    return false;
  }

  return stack->storage_->ops_->isFull(stack->storage_);
}

s16 STACK_concat(Stack* stack, Stack* stack_1) {

  if (NULL == stack || NULL == stack_1) {
    return KErrorCode_Stack_Concat_stackNULL;
  }
  if (NULL == stack->storage_ || NULL == stack_1->storage_) {
    return KErrorCode_Stack_Concat_storageNULL;
  }

  return stack->storage_->ops_->concat(stack->storage_, stack_1->storage_);
}

void* STACK_top(Stack *stack) {

  if (NULL == stack || NULL == stack->storage_) {
    return NULL;
  }

  return stack->storage_->ops_->last(stack->storage_);
}

void* STACK_pop(Stack *stack) {

  if (NULL == stack || NULL == stack->storage_) {
    return NULL;
  }

  return stack->storage_->ops_->extractLast(stack->storage_);
}

s16 STACK_push(Stack* stack, void* data, u16 bytes) {

  if (NULL == stack) {
    return KErrorCode_Stack_Push_stackNULL;
  }

  if (NULL == stack->storage_) {
    return KErrorCode_Stack_Push_storageNULL;
  }

  return stack->storage_->ops_->insertLast(stack->storage_, data, bytes);
}

s16 STACK_resize(Stack *stack, u16 new_size) {

  if (NULL == stack) {
    return KErrorCode_Stack_Resize_stackNULL;
  }

  if (NULL == stack->storage_) {
    return KErrorCode_Stack_Rseize_storageNULL;
  }

  return stack->storage_->ops_->resize(stack->storage_, new_size);
}


void STACK_print(Stack *stack) {

  printf("\n\n");

  if (NULL == stack) {
    printf("     [Stack Info] Address: NULL\n");
  }
  else {
    if (NULL == stack->storage_) {

      printf("     [Stack Info] Address: NULL\n");

    }
    else {

      printf("     [Stack Info] Address: %p\n", stack);
      printf("     [Stack Info] Storage address: %p\n", stack->storage_);
      stack->storage_->ops_->print(stack->storage_);
    }
  }
}



