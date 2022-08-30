// adt_vector.c
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#include <stdio.h>
#include <stdlib.h>

#include "common_def.h"
#include "adt_vector.h"

#include "ABGS_MemoryManager/abgs_memory_manager.h"

// Vector Declarations
Vector* VECTOR_create(u16 capacity);

static s16 VECTOR_destroy(Vector *vector);
static s16 VECTOR_reset(Vector* vector);
static s16 VECTOR_softReset(Vector* vector);
static u16 VECTOR_capacity(Vector* vector);
static u16 VECTOR_length(Vector* vector);
static bool VECTOR_isEmpty(Vector* vector);
static bool VECTOR_isFull(Vector* vector);
static void* VECTOR_first(Vector* vector);
static void* VECTOR_last(Vector* vector);
static void* VECTOR_at(Vector* vector, u16 position);
static void VECTOR_print(Vector* vector);
static s16 VECTOR_insertFirst(Vector* vector, void *data, u16 bytes);
static s16 VECTOR_insertLast(Vector* vector, void *data, u16 bytes);
static s16 VECTOR_insertAt(Vector* vector, void *data, u16 bytes, u16 position);
static s16 VECTOR_resize(Vector* vector, u16 new_size);
static void* VECTOR_extractAt(Vector* vector, u16 position);
static void* VECTOR_extractFirst(Vector* vector);
static void* VECTOR_extractLast(Vector* vector);
static s16 VECTOR_concat(Vector* vector, Vector* vector_src);
static s16 VECTOR_traverse(Vector* vector, void(*callback)(MemoryNode*));
static void VECTOR_print(Vector* vector);

struct vector_ops_s vector_ops = {
  .destroy = VECTOR_destroy,
  .reset = VECTOR_reset,
  .softReset = VECTOR_softReset,
  .capacity = VECTOR_capacity,
  .length = VECTOR_length,
  .isEmpty = VECTOR_isEmpty,
  .isFull = VECTOR_isFull,
  .first = VECTOR_first,
  .last = VECTOR_last,
  .at = VECTOR_at,
  .print = VECTOR_print,
  .insertFirst = VECTOR_insertFirst,
  .insertLast = VECTOR_insertLast,
  .insertAt = VECTOR_insertAt,
  .resize = VECTOR_resize,
  .extractAt = VECTOR_extractAt,
  .extractFirst = VECTOR_extractFirst,
  .extractLast = VECTOR_extractLast,
  .concat = VECTOR_concat,
  .traverse = VECTOR_traverse,
  .print = VECTOR_print
};


Vector* VECTOR_create(u16 capacity) {

  if (0 == capacity) {
    return NULL;
  }
  Vector* vector = MM->malloc(sizeof(Vector));

  if (NULL == vector) {
    return NULL;
  }
  vector->storage_ = MM->malloc(sizeof(MemoryNode) * capacity);
  if (NULL == vector->storage_) {

    MM->free(vector);
    return NULL;
  }
  for (int i = 0; i < capacity; i++) {
    MEMNODE_createLite(vector->storage_ + i);
  }
  vector->head_ = 0;
  vector->tail_ = 0;
  vector->capacity_ = capacity;
  vector->ops_ = &vector_ops;

  return vector;
}

s16 VECTOR_destroy(Vector* vector) {
  if (NULL == vector) {
    return KErrorCode_Vector_VectorNULL;
  }
  if (NULL == vector->storage_) {
    MM->free(vector);
    vector = NULL;
    return KErrorCode_Vector_StorageNULL;
  }
  MemoryNode *pointer = vector->storage_;

  for (int i = vector->tail_ - 1; i >= 0; --i) {
    if (pointer->data_ != NULL) {
      MM->free(pointer->data_);
    }
    pointer++;
  }
  MM->free(vector->storage_);
  vector->storage_ = NULL;
  MM->free(vector);
  vector = NULL;
  return kErrorCode_Ok;
}

s16 VECTOR_reset(Vector* vector) {
  if (NULL == vector) {
    return KErrorCode_Vector_VectorNULL;
  }
  if (NULL == vector->storage_) {
    return KErrorCode_Vector_StorageNULL;
  }
  MemoryNode *pointer = vector->storage_;

  for (int i = vector->tail_ - 1; i >= 0; --i) {
    pointer->ops_->reset(pointer);
    pointer++;
  }
  vector->tail_ = 0;
  return kErrorCode_Ok;
}

s16 VECTOR_softReset(Vector* vector) {
  if (NULL == vector) {
    return KErrorCode_Vector_VectorNULL;
  }
  if (NULL == vector->storage_) {
    return KErrorCode_Vector_StorageNULL;
  }
  if (0 == vector->capacity_) {
    return KErrorCode_Vector_CapacityZero;
  }
  MemoryNode *pointer = vector->storage_;

  for (int i = vector->tail_ - 1; i >= 0; --i) {
    pointer->ops_->softReset(pointer);
    pointer++;
  }
  vector->tail_ = 0;
  return kErrorCode_Ok;
}

u16 VECTOR_capacity(Vector* vector) {
  if (NULL == vector) {
    return 0;
  }
  return vector->capacity_;
}

u16 VECTOR_length(Vector* vector) {
  if (NULL == vector) {
    return 0;
  }
  return vector->tail_ - vector->head_;
}

bool VECTOR_isEmpty(Vector* vector) {
  if (NULL == vector) {
    return true;
  }
  if (NULL == vector->storage_) {
    return true;
  }

  return vector->tail_ == vector->head_;
}

bool VECTOR_isFull(Vector* vector) {
  if (NULL == vector) {
    return false;
  }
  if (NULL == vector->storage_) {
    return false;
  }
  return vector->tail_ == vector->capacity_;
}

void* VECTOR_first(Vector* vector) {

  if (NULL == vector) {
    return NULL;
  }
  if (vector->tail_ == 0) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }
  return (vector->storage_ + vector->head_)->data_;
}

void* VECTOR_last(Vector* vector) {

  if (NULL == vector) {
    return NULL;
  }
  if (vector->tail_ == 0) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }
  return (vector->storage_ + vector->tail_ - 1)->data_;
}

void* VECTOR_at(Vector* vector, u16 position) {

  if (NULL == vector) {
    return NULL;
  }
  if (vector->tail_ == 0) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }
  if (position >= vector->tail_) {
    return NULL;
  }
  return (vector->storage_ + position)->data_;
}

s16 VECTOR_insertFirst(Vector* vector, void* data, u16 bytes) {

  if (NULL == vector) {
    return KErrorCode_Vector_VectorNULL;
  }
  if (NULL == data) {
    return KErrorCode_Vector_DataNULL;
  }
  if (NULL == vector->storage_) {
    return KErrorCode_Vector_StorageNULL;
  }
  if (bytes == 0) {
    return KErrorCode_Vector_inserFirst_BytesZero;
  }
  if (vector->capacity_ <= vector->tail_) {
    return KErrorCode_Vector_inserFirst_NoMem;
  }
  MemoryNode *pointer = (vector->storage_ + vector->tail_);
  for (int i = vector->tail_; i > vector->head_; --i) {
    pointer->data_ = (pointer - 1)->data_;
    pointer->size_ = (pointer - 1)->size_;
    pointer--;
  }
  pointer->data_ = data;
  pointer->size_ = bytes;
  vector->tail_ += 1;

  return kErrorCode_Ok;
}

s16 VECTOR_insertLast(Vector* vector, void* data, u16 bytes) {

  if (NULL == vector) {
    return KErrorCode_Vector_VectorNULL;
  }
  if (NULL == data) {
    return KErrorCode_Vector_DataNULL;
  }
  if (NULL == vector->storage_) {
    return KErrorCode_Vector_StorageNULL;
  }
  if (bytes == 0) {
    return KErrorCode_Vector_inserFirst_BytesZero;
  }
  if (vector->capacity_ <= vector->tail_) {
    return KErrorCode_Vector_inserFirst_NoMem;
  }
  (vector->storage_ + vector->tail_)->data_ = data;
  (vector->storage_ + vector->tail_)->size_ = bytes;

  vector->tail_ += 1;

  return kErrorCode_Ok;
}

s16 VECTOR_insertAt(Vector* vector, void* data, u16 bytes, u16 position) {
  if (NULL == vector) {
    return KErrorCode_Vector_VectorNULL;
  }
  if (NULL == data) {
    return KErrorCode_Vector_DataNULL;
  }
  if (NULL == vector->storage_) {
    return KErrorCode_Vector_StorageNULL;
  }
  if (bytes == 0) {
    return KErrorCode_Vector_inserFirst_BytesZero;
  }
  if (vector->capacity_ <= vector->tail_) {
    return KErrorCode_Vector_inserFirst_NoMem;
  }

  if (position > vector->tail_) {
    position = vector->tail_;
  }
  MemoryNode *pointer = (vector->storage_ + vector->tail_);
  for (int i = vector->tail_; i > position; --i) {
    pointer->data_ = (pointer - 1)->data_;
    pointer->size_ = (pointer - 1)->size_;
    pointer--;
  }
  pointer->data_ = data;
  pointer->size_ = bytes;
  vector->tail_ += 1;

  return kErrorCode_Ok;
}

void* VECTOR_extractAt(Vector* vector, u16 position) {
  if (NULL == vector) {
    return NULL;
  }
  if (position >= vector->tail_) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }
  if (VECTOR_isEmpty(vector)) {
    return NULL;
  }
  void* aux = (vector->storage_ + position)->data_;
  (vector->storage_ + position)->ops_->softReset((vector->storage_ + position));
  for (int i = position; i < vector->tail_; ++i) {
    vector->storage_->ops_->setData(vector->storage_ + i,
      (vector->storage_ + i + 1)->data_,
      (vector->storage_ + i + 1)->size_);
  }
  vector->tail_ -= 1;

  return aux;
}

void* VECTOR_extractFirst(Vector* vector) {

  if (NULL == vector) {
    return NULL;
  }
  if (0 == vector->tail_) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }
  void* aux = vector->storage_->data_;
  (vector->storage_)->ops_->softReset((vector->storage_));
  for (int i = 0; i < vector->tail_; i++) {

    (vector->storage_ + i)->data_ = (vector->storage_ + i + 1)->data_;
    (vector->storage_ + i)->size_ = (vector->storage_ + i + 1)->size_;
  }
  vector->tail_ -= 1;

  return aux;
}

void* VECTOR_extractLast(Vector* vector) {
  if (NULL == vector) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }
  if (vector->tail_ == 0) {
    return NULL;
  }
  void* aux = (vector->storage_ + vector->tail_ - 1)->data_;
  (vector->storage_ + vector->tail_)->data_ = NULL;
  (vector->storage_ + vector->tail_)->size_ = 0;
  vector->tail_ -= 1;

  return aux;
}

s16 VECTOR_concat(Vector* vector, Vector* vector_src) {

  if (NULL == vector) {
    return KErrorCode_Vector_VectorNULL;
  }
  if (NULL == vector->storage_) {
    return KErrorCode_Vector_StorageNULL;
  }
  if (NULL == vector_src) {
    return KErrorCode_Vector_Vectorsrc_NULL;
  }
  if (NULL == vector_src->storage_) {
    return KErrorCode_Vector_Vectorsrc_storage_NULL;
  }
  VECTOR_resize(vector, vector->capacity_ + vector_src->capacity_);
  u16 tail = 0;
  MemoryNode* memory_node = vector_src->storage_;
  MemoryNode* memory_node2 = vector->storage_ + vector->tail_;
  for (tail = 0; tail < vector_src->tail_; tail++) {

    (vector->storage_)->ops_->memCopy(
      memory_node2, memory_node->data_, memory_node->size_
    );
    memory_node++;
    memory_node2++;
  }
  vector->tail_ += tail;

  return kErrorCode_Ok;
}

s16 VECTOR_traverse(Vector* vector, void(*callback)(MemoryNode*)) {

  if (NULL == vector) {
    return KErrorCode_Vector_VectorNULL;
  }
  if (NULL == callback) {
    return kErrorCode_callback;
  }
  for (int i = vector->head_; i < vector->tail_; ++i) {
    callback(vector->storage_ + i);
  }

  return kErrorCode_Ok;
}

void VECTOR_print(Vector *vector) {

  printf("\n\n");

  if (vector == NULL) {
    printf("     [Vector Info] Address: NULL\n");
  }
  else {
    printf("     [Vector Info] Address: %p\n", &vector);
    printf("     [Vector Info] Head: %d\n", vector->head_);
    printf("     [Vector Info] Tail: %d\n", vector->tail_);
    printf("     [Vector Info] Length: %d\n", VECTOR_length(vector));
    printf("     [Vector Info] Capacity: %d\n", vector->capacity_);

    MemoryNode* aux = vector->storage_;

    if (vector->storage_ == NULL) {
      printf("     [Vector Info] Address: NULL\n");
    }
    else {
      for (int i = vector->head_; i < vector->tail_; ++i) {
        printf("     [Vector Info] Storage: #%d\n", i);
        vector->storage_->ops_->print(aux);
        aux++;
      }
    }
  }
}

s16 VECTOR_resize(Vector *vector, u16 new_size) {

  if (NULL == vector) {
    return KErrorCode_Vector_VectorNULL;
  }

  if (NULL == vector->storage_) {
    return KErrorCode_Vector_StorageNULL;
  }

  u16 resize_size = vector->capacity_;
  if (vector->capacity_ > new_size) {
    resize_size = new_size;
  }

  MemoryNode *aux = MM->malloc(new_size * sizeof(MemoryNode));

  if (aux == NULL) {
    return KErrorCode_Vector_auxNull;
  }

  memcpy(aux, vector->storage_, resize_size * sizeof(MemoryNode));

  for (int i = vector->capacity_; i < new_size; i++) {
    MEMNODE_createLite(aux + i);
  }

  if (resize_size != vector->capacity_) {

    MemoryNode *aux2 = vector->storage_ + new_size;

    for (int i = 0; i < vector->capacity_ - new_size; i++) {

      vector->storage_->ops_->reset(aux2);
      aux2++;

    }
    vector->tail_ = resize_size;
  }
  MM->free(vector->storage_);
  vector->storage_ = aux;
  vector->capacity_ = new_size;

  return kErrorCode_Ok;
}
