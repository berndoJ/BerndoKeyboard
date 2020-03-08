/*******************************************************************************
* \file infinikeys_cfg.h
* \date 25.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Handles the in-operation changable configuration for the InfiniKeys
*        firmware/library.
*******************************************************************************/

#if !defined(__INFINIKEYS_CFG_H)
#define __INFINIKEYS_CFG_H

#include "infinikeys_def.h"
#include "infinikeys_hwif.h"

#if !defined(IK_CFG_DEFAULT_CFG_BUF_SIZE)
#define IK_CFG_DEFAULT_CFG_BUF_SIZE                         64U
#endif // IK_CFG_DEFAULT_CFG_BUF_SIZE

#if !defined(IK_CFG_MAX_CFG_BUF_SIZE)
#define IK_CFG_MAX_CFG_BUF_SIZE                             2048U
#endif // IK_CFG_MAX_CFG_BUF_SIZE

#if !defined(IK_CFG_DEFAULT_REALLOC_CFG_BUF_SIZE)
#define IK_CFG_DEFAULT_REALLOC_CFG_BUF_SIZE                 32U
#endif // IK_CFG_DEFAULT_REALLOC_CFG_BUF_SIZE

#if !defined(IK_CFG_DEFAULT_KEY_LOOKUP_BUF_SIZE)
#define IK_CFG_DEFAULT_KEY_LOOKUP_BUF_SIZE                  64U
#endif // IK_CFG_DEFAULT_KEY_LOOKUP_BUF_SIZE

#if !defined(IK_CFG_MAX_KEY_LOOKUP_BUF_SIZE)
#define IK_CFG_MAX_KEY_LOOKUP_BUF_SIZE                      2048U
#endif // IK_CFG_MAX_KEY_LOOKUP_BUF_SIZE

#if !defined(IK_CFG_DEFAULT_REALLOC_KEY_LOOKUP_BUF_SIZE)
#define IK_CFG_DEFAULT_REALLOC_KEY_LOOKUP_BUF_SIZE          32U
#endif // IK_CFG_DEFAULT_REALLOC_KEY_LOOKUP_BUF_SIZE

#if !defined(IK_CFG_BUFFER_FRACTURING_FC_THRES)
#define IK_CFG_BUFFER_FRACTURING_FC_THRES                   10U
#endif // IK_CFG_BUFFER_FRACTURING_FC_THRES

#if !defined(IK_CFG_MAX_CFG_VALUE_SIZE)
#define IK_CFG_MAX_CFG_VALUE_SIZE                           1024U
#endif // IK_CFG_MAX_CFG_VALUE_SIZE

/*----------------------------------------------------------------------------*/

/**
 * \brief Represents the value's type of a config entry.
 */ 
typedef enum _IK_CFG_ValueType
{
    IK_CFG_VALUE_TYPE_UINT, /**< Unsigned integer. */
    IK_CFG_VALUE_TYPE_INT, /**< Signed integer. */
    IK_CFG_VALUE_TYPE_FLOAT, /**< Float. */
    IK_CFG_VALUE_TYPE_BYTE_ARRAY, /**< Array of bytes (varaible size). */
    IK_CFG_VALUE_TYPE_STRING /**< String (variable size). */
} IK_CFG_ValueType_t;

/*----------------------------------------------------------------------------*/

/**
 * \brief Represents one configuration element.
 */
typedef struct _IK_CFG_Entry
{
    uint32_t Key; /**< The 32-bit key identifying the configuration this entry
                       sets. The key 0x00000000 indicates that the entry is
                       empty and can be repopulated. */

    uint16_t ValueSize; /**< The size of the value in number of bytes. */

    void *Value; /**< Pointer to the value. */
} IK_CFG_Entry_t;
#define IK_CFG_NULL_CFG_ENTRY { 0U, 0U, NULL }

/**
 * \brief Holds information about a certain key
 */
typedef struct _IK_CFG_KeyLookupItem
{
    uint32_t Key; /**< The 32-bit key identifying the configuraiton. The key
                       0x00000000 indicates that the entry is empty and can be
                       repopulated. */
    
    uint16_t ValueSize; /**< If the size of the value is variable, this value
                             represents the maximum number of bytes the value
                             can have to prevent the value from allocating too
                             much memory. If the size of the value is not
                             variable, this value represents the exact number
                             of bytes the value has. */

    IK_CFG_ValueType_t ValueType; /**< The type of the configuration value. */
} IK_CFG_KeyLookupItem_t;
#define IK_CFG_NULL_KEY_LOOKUP_ITEM { 0U, 0U, 0U }

/**
 * \brief Configuration table data structure.
 */
typedef struct _IK_CFG_ConfigTable
{
    uint16_t Size; /**< The current size of the config table in entry counts. */

    IK_CFG_Entry_t *Entries; /**< Pointer to the array of entries. */
} IK_CFG_ConfigTable_t;
#define IK_CFG_NULL_CONFIG_TABLE { 0U, NULL }

/**
 * \brief Configuration key lookup table data structure.
 */
typedef struct _IK_CFG_KeyLookupTable
{
    uint16_t Size; /**< The current size of the lookup table in entry counts. */

    IK_CFG_KeyLookupItem_t *Entries; /**< Pointer to the array of entries. */
} IK_CFG_KeyLookupTable_t;
#define IK_CFG_NULL_KEY_LOOKUP_TABLE { 0U, NULL }

/**
 * \brief Initialisation struct for providing intialisation info to the
 *        InfiniKeys-Config module.
 */
typedef struct _IK_CFG_Init
{
    IK_CFG_Memory_HW_Interface_t *ConfigMem_HW_Interface;
} IK_CFG_Init_t;

/*----------------------------------------------------------------------------*/

/**
 * \brief Initialisation function for this module.
 * \param init_struct The struct providing the initialisation info to this
 *                    module.
 * \return The return status of this function.
 */
IK_Status_t IK_CFG_Init(IK_CFG_Init_t *init_struct);

/**
 * \brief Executes the code which should be executed regularly with non-constant
 *        interval.
 */
void IK_CFG_Tick(void);

/**
 * \brief De-initialises this module and frees up all buffers used by it.
 * \return The return status of this function.
 */
IK_Status_t IK_CFG_DeInit(void);

/**
 * \brief Saves the configuration to the non-volatile configuration memory. This
 *        interfaces to the memory hardware interface provided at
 *        initialisation.
 * \return The return status of this function.
 */
IK_Status_t IK_CFG_SaveToMemory(void);

/**
 * \brief Loads the configuration from the non-volatile configuration memory,
 *        clears all old configurations and loads the read config into the
 *        config buffer.
 * \return The return status of this function.
 */
IK_Status_t IK_CFG_LoadConfigFromMemory(void);

/**
 * \brief Sets the provided configuration to the provided value. This will set
 *        the corresponding configuration to the provided value regardless if
 *        the config was set previously. In that case, this function
 *        de-allocates the previous value pointer and replaces the entry with
 *        the provided new one.
 * \param entry The entry to set. (Key & Value)
 * \return The return status of this function.
 */
IK_Status_t IK_CFG_SetConfig(IK_CFG_Entry_t entry);

/**
 * \brief This function clears (all) the configuration identified with the given
 *        key from the config buffer. Afterwards it will appear as if the config
 *        was never set before.
 * \param key The key of the config to clear / remove from the config buffer.
 * \return The return status of this function.
 */
IK_Status_t IK_CFG_ClearConfig(uint32_t key);

/**
 * \brief This function retrieves the configuration for a given key. If the key
 *        is invalid or the configuration corresponding to the key is not set,
 *        this function will return an entry with key 0.
 * \param key The key of the configuration to retrieve.
 * \return The configuration that should be retrieved.
 */
IK_CFG_Entry_t IK_CFG_GetConfig(uint32_t key);

/**
 * \brief This function checks if the configuration for the given key is set.
 * \param key The key of the configuration to check.
 * \return Boolean value indicating if the configuration corresponding to the
 *         key is set or not.
 */
IK_Bool_t IK_CFG_IsConfigSet(uint32_t key);

/**
 * \brief This function checks if the given size is valid for an entry with the
 *        given key. This function evaluates this according to the key lookup
 *        table.
 * \param key The key of the configuration to check the valid size of.
 * \param value_size The size to check.
 * \return Boolean value indicating if value_size is a valid value for an entry
 *         with the key key.
 */
IK_Bool_t IK_CFG_IsValueSizeValid(uint32_t key, uint16_t value_size);

/**
 * \brief Forcefully initialises a compactification of the configuration
 *        buffer's element buffer. The compactification process moves
 *        all elements to the beginning of the buffer and shrinks its size to
 *        the size needed by the elements plus an additional 10 elements to
 *        ensure that adding a few elements will not necessarily trigger a new
 *        reallocation of the memory, which would be inefficient.
 * \return The return status of this function.
 */
IK_Status_t IK_CFG_ForceCompactification(void);

/**
 * \brief Adds the given item to the configuration key lookup table. This
 *        function will search for the first available spot in the key lookup
 *        table and insert the element at that position. If the KLT is full,
 *        this function will reallocate the KLT buffer, so more elements can be
 *        added. It adds the item afterwards to the newly allocated buffer. If
 *        the buffer cannot be reallocated, this function will return IK_ERROR.
 *        This function will also return IK_ERROR if either the key given with
 *        the item parameter is 0 or already exists in the KLT or if the value
 *        size given by the item parameter is 0.
 * \param item The item to add to the key lookup table. This item has to be
 *             valid and the key has to be not already present in the key
 *             lookup table.
 * \return The return status of this function.
 */
IK_Status_t IK_CFG_KLT_AddItem(IK_CFG_KeyLookupItem_t item);

/**
 * \brief This function removes the key lookup item with the given key from the
 *        configuration key lookup table. (It actually removes all lookup items
 *        with the given key, but under normal operation, such a case should
 *        never happen.) If the key is 0, this function will return IK_ERROR. If
 *        the key is not present in the key lookup table, this function will
 *        return IK_OK as usual, so no key checking is required beforehand.
 * \param key The key of the item that should be removed from the KLT.
 * \return The return status of this function.
 */
IK_Status_t IK_CFG_KLT_RemoveItem(uint32_t key);

/**
 * \brief Looks up the item with the given key from the key lookup table. If the
 *        given key is 0 or if the item doesn't exist in the KLT, this function
 *        will return a key lookup item with key 0, value size 0 and value type
 *        0.
 * \param key The key of the item that should be looked up in the KLT.
 * \return The item that should be looked up. (Or 0-item, if key is invalid.)
 */
IK_CFG_KeyLookupItem_t IK_CFG_KLT_LookupKey(uint32_t key);

/**
 * \brief Checks if the given key is present in the configuration key lookup 
 *        table. If the key is 0 or the key is not present in the KLT, this 
 *        function will return FALSE, if the key is present, it will return
 *        TRUE.
 * \param key The key to check the presence of.
 * \return Boolean value indicating if the given key corresponds to a valid
 *         item in the configuration key lookup table.
 */
IK_Bool_t IK_CFG_KLT_HasKey(uint32_t key);

/**
 * \brief Forcefully initialises a compactification of the configuration key
 *        lookup table's element buffer. The compactification process moves
 *        all elements to the beginning of the buffer and shrinks its size to
 *        the size needed by the elements plus an additional 10 elements to
 *        ensure that adding a few elements will not necessarily trigger a new
 *        reallocation of the memory, which would be inefficient.
 * \return The return status of this function.
 */
IK_Status_t IK_CFG_KLT_ForceCompactification(void);


#endif // __INFINIKEYS_CFG_H