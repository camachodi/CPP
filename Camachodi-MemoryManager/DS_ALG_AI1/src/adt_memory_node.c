 // adt_memory_node.c
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#include <stdio.h>
#include <stdlib.h>

#include "common_def.h"
#include "adt_memory_node.h"

#include "ABGS_MemoryManager/abgs_memory_manager.h"


// Memory Node Declarations
static s16 MEMNODE_initWithoutCheck(MemoryNode *node);	
static void* MEMNODE_data(MemoryNode *node);	
static u16 MEMNODE_size(MemoryNode *node); 
static s16 MEMNODE_setData(MemoryNode* node, void* src, u16 bytes);
static s16 MEMNODE_reset(MemoryNode* node);	
static s16 MEMNODE_softReset(MemoryNode* node); 
static s16 MEMNODE_free(MemoryNode* node); 
static s16 MEMNODE_softFree(MemoryNode* node); 
static s16 MEMNODE_memSet(MemoryNode* node, u8 value); 
static s16 MEMNODE_memCopy(MemoryNode* node, void* src, u16 bytes); 
static s16 MEMNODE_memConcat(MemoryNode* node, void* src, u16 bytes); 
static s16 MEMNODE_memMask(MemoryNode* node, u8 mask); 
static void MEMNODE_print(MemoryNode* node); 


// Memory Node's API Definitions
struct memory_node_ops_s memory_node_ops = { .data = MEMNODE_data,
                                             .size = MEMNODE_size,
                                             .setData = MEMNODE_setData,
                                             .reset = MEMNODE_reset,
                                             .softReset = MEMNODE_softReset,
                                             .free = MEMNODE_free,
                                             .softFree = MEMNODE_softFree,
                                             .memSet = MEMNODE_memSet,
                                             .memCopy = MEMNODE_memCopy,
                                             .memConcat = MEMNODE_memConcat,
                                             .memMask = MEMNODE_memMask,
                                             .print = MEMNODE_print
};

// Memory Node Definitions
MemoryNode* MEMNODE_create() {
  MemoryNode *node = MM->malloc(sizeof(MemoryNode));
  if (NULL == node) {
#ifdef VERBOSE_
    printf("Error: [%s] not enough memory available\n", __FUNCTION__);
#endif
    return NULL;
  }
  MEMNODE_initWithoutCheck(node);
  return node;
}

s16 MEMNODE_createFromRef(MemoryNode **node) {
  if (NULL == node) {
    return kErrorCode_Memory;
  }
  if (*node != NULL) {
    MM->free(*node);
  }
  *node = MEMNODE_create();
  if (NULL == *node) {
#ifdef VERBOSE_
    printf("Error: [%s] not enough memory available\n", __FUNCTION__);
#endif
    return kErrorCode_Memory;
  }
  return kErrorCode_Ok;
}

s16 MEMNODE_createLite(MemoryNode* node) {

  if (node == NULL) {
    return KErrorCode_MemoryNode_createLite;
  }
  node->data_ = NULL;
  node->size_ = 0;
  node->ops_ = &memory_node_ops;
  node->next_ = NULL;
  node->prev_ = NULL;

  return kErrorCode_Ok;
}

s16 MEMNODE_initWithoutCheck(MemoryNode *node) {
  node->data_ = NULL;
  node->size_ = 0;
  node->ops_ = &memory_node_ops;
  node->next_ = NULL;
  node->prev_ = NULL;

  return kErrorCode_Ok;
}

void* MEMNODE_data(MemoryNode *node) { 

  if (node == NULL) {
#ifdef VERBOSE_
    printf("Error: [%s] pointer to node is NULL\n", __FUNCTION__);
#endif
    return NULL; 
  }
  return node->data_;
}

u16	MEMNODE_size(MemoryNode *node) { 
    
  if (node == NULL) {
    return 0;
  }
  return node->size_;
}

s16 MEMNODE_setData(MemoryNode *node, void *src, u16 bytes) { 

  if (node == NULL) {
    return KErrorCode_MemoryNode_NodeNULL;
  }
  if (src == NULL) {
    return KErrorCode_MemoryNode_NewDataIsNULL;
  }
  if (bytes == 0) {
    return KErrorCode_MemoryNode_NewDataSizeIsZero;
  }

  node->data_ = src;
  node->size_ = bytes;
  return kErrorCode_Ok;
}

s16 MEMNODE_reset(MemoryNode* node) { 

  if (node == NULL) {
    return KErrorCode_MemoryNode_Reset;
  }
  if (node->data_ != NULL) {
    MM->free((u8*)(node->data_));
    node->data_ = NULL;
  }
  node->size_ = 0;
  return kErrorCode_Ok;
}

s16 MEMNODE_softReset(MemoryNode* node) { 

  if (node == NULL) {
    return KErrorCode_MemoryNode_softReset;
  }
  node->data_ = NULL;
  node->size_ = 0;

  return kErrorCode_Ok;
}

s16 MEMNODE_free(MemoryNode* node) { 

  if (node == NULL) {
    return KErrorCode_MemoryNode_free;
  }
  if (node->data_ != NULL) {
    MM->free(node->data_);
  }
  MM->free(node);
  return kErrorCode_Ok;
}

s16 MEMNODE_softFree(MemoryNode* node) { 

  if (node == NULL) {
    return KErrorCode_MemoryNode_softFree;
  }
  MM->free(node);
  return kErrorCode_Ok;
}

s16 MEMNODE_memSet(MemoryNode* node, u8 value) { 

  if (NULL == node) {
    return KErrorCode_MemoryNode_memSetNodeIsNull;
  }
  if (NULL == node->data_) {
    return KErrorCode_MemoryNode_memSetDataIsNull;
  }
  if (!memset(node->data_, value, node->size_)) {
    return KErrorCode_MemoryNode_memSetDataIsNull;
  }
  return kErrorCode_Ok;
}

s16 MEMNODE_memCopy(MemoryNode* node, void* src, u16 bytes) { 

  if (node == NULL) {
    return KErrorCode_MemoryNode_memCopy;
  }
  if (src == NULL) {
    return KErrorCode_MemoryNode_memCopy_SrcIsNull;
  }
  if (node->data_ != NULL) {
    //return KErrorCode_MemoryNode_memCopy_dataIsNull;
		MM->free(node->data_);
  }

  node->data_ = MM->malloc(bytes);
  memcpy(node->data_, src, bytes);
  node->size_ = bytes;
  return kErrorCode_Ok;
}

s16 MEMNODE_memConcat(MemoryNode* node, void* src, u16 bytes) { 

  if (node == NULL) {
    return KErrorCode_MemoryNode_memConcat_NodeIsNull;
  }
  if (node->data_ == NULL) {
    return KErrorCode_MemoryNode_memConcat_DataIsNull;
  }
  if (src == NULL) {
    return KErrorCode_MemoryNode_memConcat_SrcIsNull;
  }
  if (0 == bytes) {
    return KErrorCode_MemoryNode_memConcat_BytesIsNull;
  }

  void* aux = MM->malloc(node->size_ + bytes);
  memcpy(aux, node->data_, node->size_);
  memcpy((u8*)aux + node->size_, src, bytes);
  MM->free(node->data_);
  node->data_ = aux;
  node->size_ = node->size_ + bytes;
  return kErrorCode_Ok;
}

s16 MEMNODE_memMask(MemoryNode* node, u8 mask) { 

  if (node == NULL) {
    return KErrorCode_MemoryNode_memMask_NodeIsNull;
  }
  if (node->data_ == NULL) {
    return KErrorCode_MemoryNode_memMask_NodeDataIsNull;
  }
  u8* auxiliar = (u8*)node->data_;
  for (int i = 0; i < node->size_; i++){
    *auxiliar = *auxiliar & mask;
    auxiliar++;
  }

  return kErrorCode_Ok;
}

void MEMNODE_print(MemoryNode* node) { 

  if (node == NULL) {
    printf("            [Node Info] Address: NULL\n");
  } else {
    printf("            [Node Info] Address: %p\n",node);
    printf("            [Node Info] Size: %d\n", node->size_);
    if (node->data_ == NULL) {
      printf("            Data Address: = NULL\n");
    } else {
      printf("            [Node Info] Data address: %p\n", node->data_);
      printf("            [Node Info] Data content: %s\n", (char*)node->data_);
    }
    printf("            [Node Info] Next address: %p\n", node->next_);
    printf("            [Node Info] Prev address: %p\n", node->prev_);
  }
}


