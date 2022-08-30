// adt_memory_node.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_MEMORY_NODE_H__
#define __ADT_MEMORY_NODE_H__

#include "ABGS_MemoryManager/abgs_platform_types.h"

// Memory Node type
typedef struct memory_node_s {
  void *data_;
  u16 size_;
  struct memory_node_ops_s *ops_;
  struct memory_node_s* next_;
  struct memory_node_s* prev_;
} MemoryNode;

// Memory Node's API Declarations
struct memory_node_ops_s {

  /** @brief Get the data of the pointer
  *   
  *   Getter the datas of struct which are pointed by the pointer 
  *
  *   @return data pointer
  *   @param *node The node which pointed to memory adress
  */
  void*(*data) (MemoryNode *node); 

  /** @brief Setter of data pointer content
  *
  *   Setter the data and his data size to a memory node
  *
  *   @return NULL exception if src or data is NULL
  *   @param *node The node which pointed to memory adress
  *   @param *src The void pointer which is point to the 
  *      adress with a new data for node
  *   @param bytes The size of the new data of the memory content
  */

  s16(*setData) (MemoryNode *node, void *src, u16 bytes);
  
  /** @brief Return the size of the data
  *   
  *   Return the size of the data pointed and then we know 
  *      his size
  *
  *   @return The size of the data
  *   @return KErrorCode_MemoryNode_NodeNULL 
  *   @return KErrorCode_MemoryNode_NewDataIsNULL
  *   @return KErrorCode_MemoryNode_NewDataSizeIsZero
  *   @param *node The node which pointed to memory adress
  */

  u16(*size) (MemoryNode *node); 

  /** @brief Reset content of memory node
  *
  *   Return the data of memory node 
  *
  *   @return NULL exception if node is NULL
  *   @param *node The node which pointed to memory adress
  */

  s16(*reset) (MemoryNode *node);		

  /** @brief Reset content of memory node without a free
  *
  *   Return the data of memory node and put his size at 0
  *      without a free. So we only put it the data to NULL and
  *      size to 0
  *
  *   @return NULL exception if node is NULL
  *   @return KErrorCode_MemoryNode_Reset
  *   @param *node The node which pointed to memory adress
  */

  s16(*softReset) (MemoryNode *node);	

  /** @brief Free memory, both data and node
  *   
  *   Freeying the memory node content with a free of the node and data
  *
  *   @return NULL exception if node is NULL
  *   @return KErrorCode_MemoryNode_softReset
  *   @param *node The node which pointed to memory adress
  */

  s16(*free) (MemoryNode *node);	

  /** @brief Free only the node 
  *
  *   Freeying the memory node but doenst free his data 
  *      because the data is in another part of the memory and we need to 
  *      keep it
  *
  *   @return NULL exception if node is NULL
  *   @return KErrorCode_MemoryNode_free
  *   @param *node The node which pointed to memory adress
  */

  s16(*softFree) (MemoryNode *node);	// free only the node (its mmory, not its data)

  /** @brief Puts another dates to the node
  *
  *   We put another dates on the node, data´s node and size of the data using
  *      the memset function
  *
  *   @return NULL exception if node is NULL
  *   @return KErrorCode_MemoryNode_softFree
  *   @param *node The node which pointed to memory adress
  *   @param value The new data of the node
  */

  s16(*memSet) (MemoryNode *node, u8 value); 

  /** @brief Copy the dates to the node
  *
  *   We freeyind the node data and put the new data and his size on the node 
  *      with the memcpy function
  *
  *   @return NULL exception if node is NULL, data of the node and new data is NULL
  *   @return KErrorCode_MemoryNode_memSetNodeIsNull
  *   @return KErrorCode_MemoryNode_memSetDataIsNull
  *   @return KErrorCode_MemoryNode_memSetDataIsNull
  *   @param *node The node which pointed to memory adress
  *   @param *src The new data node 
  *   @param bytes The new data size of the node
  */

  s16(*memCopy) (MemoryNode *node, void *src, u16 bytes); 

  /** @brief Concat the memory
  *
  *   Concat the memory with the dates of two memory nodes and their sizes
  *
  *   @return NULL exception if node is NULL, data of the node and new data is NULL
  *   @return KErrorCode_MemoryNode_memCopy
  *   @return KErrorCode_MemoryNode_memCopy_SrcIsNull
  *   @param *node The node which pointed to memory adress
  *   @param *src The new data node
  *   @param bytes The new data size of the node
  */

  s16(*memConcat) (MemoryNode *node, void *src, u16 bytes); 

  /** @brief Mask the size of the data
  *
  *   Mask the every byte of the data size with a binary mask
  *
  *   @return NULL exception if node is NULL, data of the node and new data is NULL
  *   @return KErrorCode_MemoryNode_memConcat_NodeIsNull
  *   @return KErrorCode_MemoryNode_memConcat_DataIsNull
  *   @return KErrorCode_MemoryNode_memConcat_SrcIsNull
  *   @return KErrorCode_MemoryNode_memConcat_BytesIsNull
  *   @param *node The node which pointed to memory adress
  *   @param mask The mask for the data size
  */

  s16(*memMask) (MemoryNode *node, u8 mask); // mask the bytes of the data with &

  /** @brief Print the information of the node
  *
  *   Print in the console the information of the memory node
  *
  *   @return nothing
  *   @return KErrorCode_MemoryNode_memMask_NodeIsNull
  *   @return KErrorCode_MemoryNode_memMask_NodeDataIsNull
  *   @param *node The node which pointed to memory adress
  */

  void(*print) (MemoryNode *node);
};
  
  /** @brief Initialize the node
  *
  *   Initialization of the node
  *
  *   @return the node which we create
  *   @param *node The node which pointed to memory adress
  */

  MemoryNode* MEMNODE_create();

  /** @brief Create a new node with other the reference of the other node
  *
  *   Initialization of the node
  *
  *   @return NULL exception if node is NULL and NULL exception if new node is NULL
  *   @param **node The new node 
  */

  s16 MEMNODE_createFromRef(MemoryNode **node);

  /** @brief Creates a memory node without memory allocation
  *
  *   Initialization of the node
  *
  *   @return the note which we create
  *   @param *node The node which pointed to memory adress
  */

  s16 MEMNODE_createLite(MemoryNode *node); 

#endif // __ADT_MEMORY_NODE_H__
