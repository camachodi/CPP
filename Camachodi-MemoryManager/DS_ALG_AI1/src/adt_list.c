// adt_list.c
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#include <stdio.h>
#include <stdlib.h>

#include "common_def.h"
#include "adt_list.h"

#include "ABGS_MemoryManager/abgs_memory_manager.h"

// List Declarations
static s16 LIST_destroy(List *list);
static s16 LIST_reset(List* list);
static s16 LIST_softReset(List* list);
static u16 LIST_capacity(List* list);
static u16 LIST_length(List* list);
static bool LIST_isEmpty(List* list);
static bool LIST_isFull(List* list);
static void* LIST_first(List* list);
static void* LIST_last(List* list);
static void* LIST_at(List* list, u16 position);
static void LIST_print(List* list);
static s16 LIST_insertFirst(List* list, void *data, u16 bytes);
static s16 LIST_insertLast(List* list, void *data, u16 bytes);
static s16 LIST_insertAt(List* list, void *data, u16 bytes, u16 position);
static s16 LIST_resize(List* list, u16 new_size);
static void* LIST_extractAt(List* list, u16 position);
static void* LIST_extractFirst(List* list);
static void* LIST_extractLast(List* list);
static s16 LIST_concat(List* list, List* vector_src);
static s16 LIST_traverse(List* list, void(*callback)(MemoryNode*));
static void LIST_print(List* list);

struct list_ops_s list_ops = {
  .destroy = LIST_destroy,
  .reset = LIST_reset,
  .softReset = LIST_softReset,
  .capacity = LIST_capacity,
  .length = LIST_length,
  .isEmpty = LIST_isEmpty,
  .isFull = LIST_isFull,
  .first = LIST_first,
  .last = LIST_last,
  .at = LIST_at,
  .print = LIST_print,
  .insertFirst = LIST_insertFirst,
  .insertLast = LIST_insertLast,
  .insertAt = LIST_insertAt,
  .resize = LIST_resize,
  .extractAt = LIST_extractAt,
  .extractFirst = LIST_extractFirst,
  .extractLast = LIST_extractLast,
  .concat = LIST_concat,
  .traverse = LIST_traverse,
  .print = LIST_print
};


List* LIST_create(u16 capacity) {
  List* list = MM->malloc(sizeof(List));

  if (NULL == list) {
    return NULL;
  }
  if (0 == capacity) {
    return KErrorCode_List_create_capacityNULL;
  }

  list->head_ = 0;
  list->tail_ = 0;
  list->capacity_ = capacity;
  list->ops_ = &list_ops;

  return list;
}

s16 LIST_destroy(List* list) {
  if (NULL == list) {
    return KErrorCode_List_destroy_listIsNULL;
  }
  if (0 == list->capacity_ || NULL == list->head_) {
    MM->free(list);
    list = NULL;
    return KErrorCode_List_destroy_capacityheadNULL;
  }
  MemoryNode *pointer = list->head_;
  MemoryNode *pointer_next = NULL;

  for (int i = 0; i < list->length_; ++i) {
    if (NULL != pointer) {
      pointer_next = pointer->next_;
      pointer->ops_->free(pointer);
      pointer = pointer_next;
    }
  }

  MM->free(list);
  list = NULL;
  return kErrorCode_Ok;
}

s16 LIST_reset(List* list) {
  if (NULL == list) {
    return KErrorCode_List_reset_listIsNULL;
  }
  if (NULL == list->head_) {
    return KErrorCode_List_reset_headNULL;
  }
  if (0 == list->capacity_) {
    return KErrorCode_List_reset_capacityNULL;
  }
  MemoryNode *pointer = list->head_;
  MemoryNode *pointer_next = NULL;

  for (int i = 0; i < list->length_; ++i) {
    if (NULL != pointer) {
      pointer_next = pointer->next_;
      pointer->ops_->free(pointer);
      pointer = pointer_next;
    }
  }

  list->tail_ = NULL;
  list->head_ = NULL;
  list->length_ = 0;

  return kErrorCode_Ok;
}

s16 LIST_softReset(List* list) {
  if (NULL == list) {
    return KErrorCode_List_softReset_listIsNULL;
  }
  if (NULL == list->head_) {
    return KErrorCode_List_softReset_headNULL;
  }
  if (0 == list->capacity_) {
    return KErrorCode_List_softReset_capacityNULL;
  }
  MemoryNode *pointer = list->head_;
  MemoryNode *pointer_next = NULL;

  for (int i = 0; i < list->length_; ++i) {
    if (NULL != pointer) {
      pointer_next = pointer->next_;
      pointer->ops_->softFree(pointer);
      pointer = pointer_next;
    }
  }

  list->tail_ = NULL;
  list->head_ = NULL;
  list->length_ = 0;

  return kErrorCode_Ok;
}

u16 LIST_capacity(List* list) {
  if (NULL == list) {
    return 0;
  }
  return list->capacity_;
}

u16 LIST_length(List* list) {
  if (NULL == list) {
    return 0;
  }
  return list->length_;
}

bool LIST_isEmpty(List* list) {
  if (NULL == list) {
    return true;
  }

  return list->length_ == 0;
}

bool LIST_isFull(List* list) {
  if (NULL == list) {
    return false;
  }
  return list->length_ == list->capacity_;
}

void* LIST_first(List* list) {

  if (NULL == list) {
    return NULL;
  }
  if (list->length_ == 0) {
    return NULL;
  }
  if (NULL == list->head_) {
    return NULL;
  }
  return list->head_->data_;
}

void* LIST_last(List* list) {

  if (NULL == list) {
    return NULL;
  }
  if (list->length_ == 0) {
    return NULL;
  }
  if (NULL == list->tail_) {
    return NULL;
  }
  return list->tail_->data_;
}

void* LIST_at(List* list, u16 position) {

  if (NULL == list) {
    return NULL;
  }
  if (list->length_ == 0) {
    return NULL;
  }
  if (NULL == list->head_) {
    return NULL;
  }
  if (position >= list->length_) {
    return NULL;
  }

  MemoryNode* node = list->head_;

  for (int i = 0; i < position; ++i) {
    if (node != NULL) {
      node = node->next_;
    }
  }

  return node != NULL ? node->data_ : NULL;
}

s16 LIST_insertFirst(List* list, void* data, u16 bytes) {

  if (NULL == list) {
    return KErrorCode_List_inserFirst_listIsNULL;
  }
  if (NULL == data) {
    return KErrorCode_List_inserFirst_dataNULL;
  }
  if (list->ops_->isFull(list)) {
    return KErrorCode_List_inserFirst_listFull;
  }
  if (bytes == 0) {
    return KErrorCode_List_inserFirst_BytesZero;
  }
  
  MemoryNode* node = MEMNODE_create();

  if (NULL == node) {
    return   KErrorCode_List_inserFirst_nodeNULL;
  }

  node->ops_->setData(node, data, bytes);
  node->next_ = list->head_;
  list->head_ = node;
  list->length_++;

  if (NULL == list->tail_) {
    list->tail_ = node;
  }

  return kErrorCode_Ok;
}

s16 LIST_insertLast(List* list, void* data, u16 bytes) {

  if (NULL == list) {
    return KErrorCode_List_inserLast_listIsNULL;
  }
  if (NULL == data) {
    return KErrorCode_List_inserLast_dataNULL;
  }
  if (bytes == 0) {
    return KErrorCode_List_inserLast_BytesZero;
  }
  if (list->length_ >= list->capacity_) {
    return KErrorCode_List_inserLast_lengthCapacity;
  }

  MemoryNode* node = MEMNODE_create();

  if (NULL == node) {
    return KErrorCode_List_inserLast_nodeNULL;
  }

  node->ops_->setData(node, data, bytes);
  if (list->tail_ != NULL) {
    list->tail_->next_ = node;
  }
  list->tail_ = node;
  list->length_++;
  if (list->head_ == NULL) {
    list->head_ = node;
  }

  return kErrorCode_Ok;
}

s16 LIST_insertAt(List* list, void* data, u16 bytes, u16 position) {
  if (NULL == list) {
    return KErrorCode_List_insterAt_listNULL;
  }
  if (NULL == data) {
    return KErrorCode_List_insterAt_dataNULL;
  }
  if (bytes == 0) {
    return KErrorCode_List_insterAt_BytesZero;
  }
  if (list->length_ >= list->capacity_) {
    return KErrorCode_List_insterAt_LenghtCapacity;
  }

  if (position > list->length_) {
    position = list->length_;
  }

  MemoryNode* node = MEMNODE_create();

  if (NULL == node) {
    return kErrorCode_Memory;
  }

  node->ops_->setData(node, data, bytes);

  if (position == 0 && list->length_ == 0) {
    list->head_ = node;
    list->tail_ = node;
  }
  else {
    MemoryNode* node_position = list->head_;

    for (int i = 0; i < position - 1; ++i) {
      if (node_position != NULL) {
        node_position = node_position->next_;
      }
    }

    if (node_position == NULL) {
      return KErrorCode_List_insterAt_nodePosition;
    }
    else if (position > 0 ) {
      node->next_ = node_position->next_;
      node_position->next_ = node;
    }
    else {
      node->next_ = node_position;
      list->head_ = node;
    }
  }
  list->length_++;

  return kErrorCode_Ok;
}

void* LIST_extractAt(List* list, u16 position) {
	if (NULL == list) {
    return NULL;
  }
  if (NULL == list->head_) {
    return NULL;
  }
	if (position <= 0) {
		return list->ops_->extractFirst(list);
	}
	if (list->length_ <= position) {
		return list->ops_->extractLast(list);
	}

	MemoryNode* aux = LIST_index(list, position - 1);
	
	if (NULL != aux) {
		if(NULL != aux->next_) {
			MemoryNode* auxNext = aux->next_->next_;

			void* auxData = aux->next_->data_;

			aux->next_->ops_->softFree(aux->next_);
			aux->next_ = auxNext;
			list->length_--;

			return auxData;
		}
	}
	else {
		return NULL;
	}
}


void* LIST_extractFirst(List* list) {

  if (NULL == list) {
    return NULL;
  }
  if (0 == list->length_) {
    return NULL;
  }
  if (NULL == list->head_) {
    return NULL;
  }
  MemoryNode* next_node = list->head_->next_;
  void* data = list->head_->data_;
  list->head_->ops_->softFree(list->head_);
  list->head_ = next_node;
  list->length_--;

  if (NULL == list->head_) {
    list->tail_ = NULL;
  }

  return data;
}

void* LIST_extractLast(List* list) {
  if (NULL == list) {
    return NULL;
  }
  if (NULL == list->tail_) {
    return NULL;
  }
  if (0 == list->length_) {
    return NULL;
  }

  MemoryNode* prev_node = list->head_;

  for (int i = 0; i < list->length_ - 2; ++i) {
    if (prev_node != NULL) {
      prev_node = prev_node->next_;
    }
  }
  
  void* data = prev_node->data_;
  MemoryNode* last_node = prev_node;
  
  if (list->length_ > 1) {
    data = prev_node->next_->data_;
    last_node = prev_node->next_;
    prev_node->next_ = NULL;
  }

  last_node->ops_->softFree(last_node);
  if (list->length_ > 1) {
    list->tail_ = prev_node;
  }
  else {
    list->tail_ = NULL;
    list->head_ = NULL;
  }
  list->length_--;

  return data;
}

s16 LIST_concat(List* list, List* list_src) {

	
	if (NULL == list) {
		return KErrorCode_List_concat_listNULL;
	}
	if (NULL == list_src) {
		return KErrorCode_List_concat_listSrcNULL;
	}
	if (NULL == list_src->head_) {
		return KErrorCode_List_concat_headNULL;
	}

	MemoryNode* node = NULL;
	MemoryNode* newdata = NULL;

	u16 length = list_src->length_;

	for (int i = 0; i < length; ++i) {
		
		node = LIST_index(list_src, i);
		newdata = MEMNODE_create();
		node->ops_->memCopy(newdata, node->data_, node->size_);
		list->ops_->insertLast(list, newdata->data_, newdata->size_);
		newdata->ops_->softFree(newdata);

	}

	list->capacity_ += list_src->capacity_;

	return kErrorCode_Ok;

}

s16 LIST_traverse(List* list, void(*callback)(MemoryNode*)) {

  if (NULL == list) {
    return KErrorCode_List_traverse_listNULL;
  }
  if (0 == list->length_) {
    return KErrorCode_List_traverse_lengthNULL;    
  }
  if (NULL == list->head_) {
    return KErrorCode_List_traverse_headNULL;
  }
  MemoryNode* node = list->head_;
  for (int i = 0; i < list->length_; ++i) {
    if (node != NULL) {
      callback(node);
      node = node->next_;
    }
  }

  return kErrorCode_Ok;
}

void LIST_print(List *list) {

  if (list == NULL) {
    printf("    [List Info] Address: NULL\n");
  }
  else {
    printf("    [List Info] Address: %p\n", list);
    printf("    [List Info] Length: %d\n", list->length_);
    printf("    [List Info] Capacity: %d\n", list->capacity_);

    printf("    [List Info] First address: %p\n", list->head_);
    printf("    [List Info] Last address: %p\n", list->tail_);

    if (list->head_ != NULL) {
      MemoryNode* node = list->head_;
      for (int i = 0; i < list->length_; ++i) {
        printf("        [List Info] Storage #%d:\n", i);
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

s16 LIST_resize(List *list, u16 new_size) {

  if (NULL == list) {
    return KErrorCode_List_resize_listNULL;
  }
  if (0 >= new_size) {
    return KErrorCode_List_resize_newsizeNULL;
  }
  if (NULL == list->head_) {
    return KErrorCode_List_resize_headNULL;
  }

  if (new_size < list->length_) {
    MemoryNode *pointer = list->head_;
    for (int i = 0; i < new_size; ++i) {
      if (NULL != pointer) {
        pointer = pointer->next_;
      }
    }
    MemoryNode *pointer_next = NULL;

    for (int i = new_size; i < list->length_; ++i) {
      if (NULL != pointer) {
        pointer_next = pointer->next_;
        pointer->ops_->free(pointer);
        pointer = pointer_next;
      }
    }

    list->tail_ = pointer_next;
    list->length_ = new_size;
  }

  list->capacity_ = new_size;

  return kErrorCode_Ok;
}

MemoryNode* LIST_index(List* list, u16 position) {
	if (NULL == list) {
		return NULL;
	}
	if (list->length_ == 0) {
		return NULL;
	}
	if (NULL == list->head_) {
		return NULL;
	}
	if (position >= list->length_) {
		return NULL;
	}

	MemoryNode* node = list->head_;

	for (int i = 0; i < position && node != NULL; i++) {
		node = node->next_;
	}

	return node;
}
