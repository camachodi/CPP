// adt_dlist.c
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#include <stdio.h>
#include <stdlib.h>

#include "common_def.h"
#include "adt_dlist.h"

#include "ABGS_MemoryManager/abgs_memory_manager.h"

// DLList Declarations
static s16 DLLIST_destroy(DLList *dlist);
static s16 DLLIST_reset(DLList* dlist);
static s16 DLLIST_softReset(DLList* dlist);
static u16 DLLIST_capacity(DLList* dlist);
static u16 DLLIST_length(DLList* dlist);
static bool DLLIST_isEmpty(DLList* dlist);
static bool DLLIST_isFull(DLList* dlist);
static void* DLLIST_first(DLList* dlist);
static void* DLLIST_last(DLList* dlist);
static void* DLLIST_at(DLList* dlist, u16 position);
static void DLLIST_print(DLList* dlist);
static s16 DLLIST_insertFirst(DLList* dlist, void *data, u16 bytes);
static s16 DLLIST_insertLast(DLList* dlist, void *data, u16 bytes);
static s16 DLLIST_insertAt(DLList* dlist, void *data, u16 bytes, u16 position);
static s16 DLLIST_resize(DLList* dlist, u16 new_size);
static void* DLLIST_extractAt(DLList* dlist, u16 position);
static void* DLLIST_extractFirst(DLList* dlist);
static void* DLLIST_extractLast(DLList* dlist);
static s16 DLLIST_concat(DLList* dlist, DLList* vector_src);
static s16 DLLIST_traverse(DLList* dlist, void(*callback)(MemoryNode*));
static void DLLIST_print(DLList* dlist);


struct dlist_ops_s dlist_ops = {
  .destroy = DLLIST_destroy,
  .reset = DLLIST_reset,
  .softReset = DLLIST_softReset,
  .capacity = DLLIST_capacity,
  .length = DLLIST_length,
  .isEmpty = DLLIST_isEmpty,
  .isFull = DLLIST_isFull,
  .first = DLLIST_first,
  .last = DLLIST_last,
  .at = DLLIST_at,
  .print = DLLIST_print,
  .insertFirst = DLLIST_insertFirst,
  .insertLast = DLLIST_insertLast,
  .insertAt = DLLIST_insertAt,
  .resize = DLLIST_resize,
  .extractAt = DLLIST_extractAt,
  .extractFirst = DLLIST_extractFirst,
  .extractLast = DLLIST_extractLast,
  .concat = DLLIST_concat,
  .traverse = DLLIST_traverse,
  .print = DLLIST_print
};


DLList* DLLIST_create(u16 capacity) {
  DLList* dlist = MM->malloc(sizeof(DLList));

  if (NULL == dlist) {
    return NULL;
  }

  dlist->head_ = 0;
  dlist->tail_ = 0;
  dlist->capacity_ = capacity;
  dlist->ops_ = &dlist_ops;

  return dlist;
}

s16 DLLIST_destroy(DLList* dlist) {
  if (NULL == dlist) {
    return KErrorCode_Vector_VectorNULL;
  }
  if (0 == dlist->capacity_ || NULL == dlist->head_) {
    MM->free(dlist);
    dlist = NULL;
    return KErrorCode_Vector_StorageNULL;
  }
  MemoryNode *pointer = dlist->head_;
  MemoryNode *pointer_next = NULL;

  for (int i = 0; i < dlist->length_; ++i) {
    if (NULL != pointer) {
      pointer_next = pointer->next_;
      pointer->ops_->free(pointer);
      pointer = pointer_next;
    }
  }

  MM->free(dlist);
  dlist = NULL;
  return kErrorCode_Ok;
}

s16 DLLIST_reset(DLList* dlist) {
  if (NULL == dlist) {
    return KErrorCode_Vector_VectorNULL;
  }
  if (0 == dlist->capacity_) {
    return KErrorCode_Vector_StorageNULL;
  }
  if (NULL == dlist->head_) {
    return KErrorCode_List_reset_headNULL;
  }
  MemoryNode *pointer = dlist->head_;
  MemoryNode *pointer_next = NULL;

  for (int i = 0; i < dlist->length_; ++i) {
    if (NULL != pointer) {
      pointer_next = pointer->next_;
      pointer->ops_->free(pointer);
      pointer = pointer_next;
    }
  }

  dlist->tail_ = NULL;
  dlist->head_ = NULL;
  dlist->length_ = 0;

  return kErrorCode_Ok;
}

s16 DLLIST_softReset(DLList* dlist) {
  if (NULL == dlist) {
    return KErrorCode_Vector_VectorNULL;
  }
  if (NULL == dlist->head_) {
    return KErrorCode_Vector_StorageNULL;
  }
  MemoryNode *pointer = dlist->head_;
  MemoryNode *pointer_next = NULL;

  for (int i = 0; i < dlist->length_; ++i) {
    if (NULL != pointer) {
      pointer_next = pointer->next_;
      pointer->ops_->softFree(pointer);
      pointer = pointer_next;
    }
  }

  dlist->tail_ = NULL;
  dlist->head_ = NULL;
  dlist->length_ = 0;
  return kErrorCode_Ok;
}

u16 DLLIST_capacity(DLList* dlist) {
  if (NULL == dlist) {
    return KErrorCode_Vector_VectorNULL;
  }
  return dlist->capacity_;
}

u16 DLLIST_length(DLList* dlist) {
  if (NULL == dlist) {
    return KErrorCode_Vector_VectorNULL;
  }
  return dlist->length_;
}

bool DLLIST_isEmpty(DLList* dlist) {
  if (NULL == dlist) {
    return true;
  }

  return dlist->length_ == 0;
}

bool DLLIST_isFull(DLList* dlist) {
  if (NULL == dlist) {
    return false;
  }
  return dlist->length_ == dlist->capacity_;
}

void* DLLIST_first(DLList* dlist) {

  if (NULL == dlist) {
    return NULL;
  }
  if (dlist->length_ == 0) {
    return NULL;
  }
  if (NULL == dlist->head_) {
    return NULL;
  }
  return dlist->head_->data_;
}

void* DLLIST_last(DLList* dlist) {

  if (NULL == dlist) {
    return NULL;
  }
  if (dlist->length_ == 0) {
    return NULL;
  }
  if (NULL == dlist->tail_) {
    return NULL;
  }
  return dlist->tail_->data_;
}

void* DLLIST_at(DLList* dlist, u16 position) {

  if (NULL == dlist) {
    return NULL;
  }
  if (dlist->length_ == 0) {
    return NULL;
  }
  if (NULL == dlist->head_) {
    return NULL;
  }
  if (position >= dlist->length_) {
    return NULL;
  }

	MemoryNode* node = DLIST_index(dlist, position);

  return node != NULL ? node->data_ : NULL;
}

s16 DLLIST_insertFirst(DLList* dlist, void* data, u16 bytes) {

  if (NULL == dlist) {
    return KErrorCode_Vector_VectorNULL;
  }
  if (NULL == data) {
    return KErrorCode_Vector_DataNULL;
  }
  if (dlist->length_ >= dlist->capacity_) {
    return KErrorCode_Vector_StorageNULL;
  }
  if (bytes == 0) {
    return KErrorCode_Vector_inserFirst_BytesZero;
  }
  
  MemoryNode* node = MEMNODE_create();

  if (NULL == node) {
    return kErrorCode_Memory;
  }

  node->ops_->setData(node, data, bytes);
  node->next_ = dlist->head_;
  if (dlist->head_ != NULL) {
    dlist->head_->prev_ = node;
  }
  dlist->head_ = node;
  dlist->length_++;

  if (NULL == dlist->tail_) {
    dlist->tail_ = node;
  }

  return kErrorCode_Ok;
}

s16 DLLIST_insertLast(DLList* dlist, void* data, u16 bytes) {

  if (NULL == dlist) {
    return KErrorCode_Vector_VectorNULL;
  }
  if (NULL == data) {
    return KErrorCode_Vector_DataNULL;
  }
  if (bytes <= 0) {
    return KErrorCode_Vector_inserFirst_BytesZero;
  }
  if (dlist->length_ >= dlist->capacity_) {
    return KErrorCode_Vector_StorageNULL;
  }

  MemoryNode* node = MEMNODE_create();

  if (NULL == node) {
    return kErrorCode_Memory;
  }

  node->ops_->setData(node, data, bytes);
  node->prev_ = dlist->tail_;
  if (dlist->tail_ != NULL) {
    dlist->tail_->next_ = node;
  }
  dlist->tail_ = node;
  dlist->length_++;
  if (dlist->head_ == NULL) {
    dlist->head_ = node;
  }

  return kErrorCode_Ok;
}

s16 DLLIST_insertAt(DLList* dlist, void* data, u16 bytes, u16 position) {

  if (NULL == dlist) {
    return KErrorCode_Vector_VectorNULL;
  }

	if (position >= dlist->length_) {
		return dlist->ops_->insertLast(dlist, data, bytes);
	}

	if (position == 0 || dlist->length_ < 1) {
		return dlist->ops_->insertFirst(dlist, data, bytes);
	}

  if (NULL == data) {
    return KErrorCode_Vector_DataNULL;
  }
  if (bytes == 0) {
    return KErrorCode_Vector_inserFirst_BytesZero;
  }
  if (dlist->length_ >= dlist->capacity_) {
    return KErrorCode_Vector_StorageNULL;
  }

  MemoryNode* node = MEMNODE_create();

  if (NULL == node) {
    return kErrorCode_Memory;
  }

  node->ops_->setData(node, data, bytes);
	
	MemoryNode* node_position = DLIST_index(dlist, position);

	node_position->prev_->next_ = node;
	node->next_ = node_position;
	node->prev_ = node_position->prev_;
	node->next_->prev_ = node;
  dlist->length_++;

  return kErrorCode_Ok;
}

void* DLLIST_extractAt(DLList* dlist, u16 position) {

  if (NULL == dlist) {
    return NULL;
  }

	if (position >= dlist->length_) {
		return dlist->ops_->extractLast(dlist);
	}

	if (position == 0 || dlist->length_ <= 1) {
		return dlist->ops_->extractFirst(dlist);
	}

  if (NULL == dlist->head_) {
    return NULL;
  }
  if (dlist->ops_->isEmpty(dlist)) {
    return NULL;
  }

	MemoryNode* node = DLIST_index(dlist, position);

  node->next_->prev_ = node->prev_;
  node->prev_->next_ = node->next_;

	void* data = node->data_;
	node->ops_->softFree(node);

	dlist->length_--;
  return data;

}

void* DLLIST_extractFirst(DLList* dlist) {

  if (NULL == dlist) {
    return NULL;
  }
  if (0 == dlist->length_) {
    return NULL;
  }
  if (NULL == dlist->head_) {
    return NULL;
  }
  MemoryNode* next_node = dlist->head_->next_;
  void* data = dlist->head_->data_;
  dlist->head_->ops_->softFree(dlist->head_);
  dlist->head_ = next_node;
  dlist->length_--;

  if (NULL == dlist->head_) {
    dlist->tail_ = NULL;
  }
  else {
    dlist->head_->prev_ = NULL;
  }

  return data;
}

void* DLLIST_extractLast(DLList* dlist) {

	if (NULL == dlist) {
		return NULL;
	}

  if (dlist->ops_->length(dlist) <= 1) {
    return dlist->ops_->extractFirst(dlist);
  }

  if (NULL == dlist->tail_) {
    return NULL;
  }
  if (0 == dlist->length_) {
    return NULL;
  }

  MemoryNode* prev_node = dlist->tail_->prev_;
  
  void* data = dlist->tail_->data_;

  prev_node->ops_->softFree(dlist->tail_);
  prev_node->next_ = NULL;
  dlist->tail_ = prev_node;
  dlist->length_--;

  return data;
}

s16 DLLIST_concat(DLList* dlist, DLList* list_src) {

  if (NULL == dlist) {
    return KErrorCode_Vector_VectorNULL;
  }
  if (NULL == list_src) {
    return KErrorCode_Vector_Vectorsrc_NULL;
  }
	
  MemoryNode* node = list_src->head_;

  for (int i = 0; i < list_src->length_; ++i) {
    if (node != NULL) {
      if (dlist->length_ > 0) {

        dlist->tail_->next_ = MEMNODE_create();
        node->ops_->memCopy(dlist->tail_->next_, node->data_, node->size_);
        dlist->tail_->prev_ = dlist->tail_;
        dlist->tail_ = dlist->tail_->next_;
      }
      else {
        dlist->tail_ = MEMNODE_create();
				if (NULL == dlist->tail_) {
					return KErrorCode_List_concat_tailNULL;
				}
        node->ops_->memCopy(dlist->tail_, node->data_, node->size_);
        dlist->head_ = dlist->tail_;
      }
      node = node->next_;
      dlist->length_++;
    }
  }

  dlist->capacity_ += list_src->capacity_;

  return kErrorCode_Ok;
}

s16 DLLIST_traverse(DLList* dlist, void(*callback)(MemoryNode*)) {

  if (NULL == dlist) {
    return KErrorCode_Vector_VectorNULL;
  }
  if (0 == dlist->length_) {
    return kErrorCode_callback;
  }
  if (NULL == dlist->head_) {
    return kErrorCode_callback;
  }
  MemoryNode* node = dlist->head_;
  for (int i = 0; i < dlist->length_; ++i) {
    if (node != NULL) {
      callback(node);
      node = node->next_;
    }
  }

  return kErrorCode_Ok;
}

void DLLIST_print(DLList *dlist) {

  if (dlist == NULL) {
    printf("    [DLList Info] Address: NULL\n");
  }
  else {
    printf("    [DLList Info] Address: %p\n", dlist);
    printf("    [DLList Info] Length: %d\n", dlist->length_);
    printf("    [DLList Info] Capacity: %d\n", dlist->capacity_);

    printf("    [DLList Info] First address: %p\n", dlist->head_);
    printf("    [DLList Info] Last address: %p\n", dlist->tail_);

    if (dlist->head_ != NULL) {
      MemoryNode* node = dlist->head_;
      for (int i = 0; i < dlist->length_; ++i) {
        printf("        [DLList Info] Storage #%d:\n", i);
        if (node != NULL) {
          node->ops_->print(node);
          node = node->next_;
        }
        else {
          printf("        [Node info] Data: NULL\n");
        }
      }
    }
  }
}

s16 DLLIST_resize(DLList *dlist, u16 new_size) {

  if (NULL == dlist) {
    return KErrorCode_Vector_VectorNULL;
  }

  if (0 >= new_size) {
    return KErrorCode_Vector_StorageNULL;
  }

  if (NULL == dlist->head_) {
    return KErrorCode_List_resize_headNULL;
  }

  if (new_size < dlist->length_) {
    MemoryNode *pointer = dlist->head_;
    for (int i = 0; i < new_size; ++i) {
      if (NULL != pointer) {
        pointer = pointer->next_;
      }
    }
    MemoryNode *pointer_next = NULL;

    for (int i = new_size; i < dlist->length_; ++i) {
      if (NULL != pointer) {
        pointer_next = pointer->next_;
        pointer->ops_->free(pointer);
        pointer = pointer_next;
      }
    }

    dlist->tail_ = pointer_next;
    dlist->length_ = new_size;
  }

  dlist->capacity_ = new_size;

  return kErrorCode_Ok;
}

MemoryNode* DLIST_index(DLList* dlist, u16 position) {

	if (NULL == dlist) {
		return NULL;
	}
	if (dlist->length_ == 0) {
		return NULL;
	}
	if (NULL == dlist->head_) {
		return NULL;
	}

	MemoryNode* node = NULL;
  u16 length = dlist->ops_->length(dlist);
	bool go_back = false;

	if (position > length / 2) {

		node = dlist->tail_;
		go_back = true;

	} else {
		node = dlist->head_;
	}

	if (go_back) {

		for (int i = length; i > position; --i) {
			node = node->prev_;
		}

	} else {

		for (int i = 0; i < position; ++i) {
			node = node->next_;
		}
	}

	return node;
}