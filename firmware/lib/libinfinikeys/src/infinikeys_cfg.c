// infinikeys_cfg.c
// Copyright (c) 2020 Johannes Berndorfer (berndoJ)

#include "infinikeys_cfg.h"

#include <stdlib.h>
#include <string.h>

static IK_CFG_Init_t *_cfg_init = NULL;

static IK_CFG_ConfigTable_t _cfg = IK_CFG_NULL_CONFIG_TABLE;
static uint8_t _cfg_fract_val = 0;

static IK_CFG_KeyLookupTable_t _cfg_key_lookup = IK_CFG_NULL_KEY_LOOKUP_TABLE;
static uint8_t _cfg_key_lookup_fract_val = 0;

static IK_Status_t _IK_CFG_AllocCfgBuf(uint16_t initial_size);
static IK_Status_t _IK_CFG_ReallocCfgBuf(uint16_t new_size);
static IK_Status_t _IK_CFG_CompactifyCfgBuf(void);
static IK_Status_t _IK_CFG_FreeCfgBuf(void);
static IK_Status_t _IK_CFG_AllocCfgKeyLookupBuf(uint16_t initial_size);
static IK_Status_t _IK_CFG_ReallocCfgKeyLookupBuf(uint16_t new_size);
static IK_Status_t _IK_CFG_CompactifyCfgKeyLookupBuf(void);
static IK_Status_t _IK_CFG_FreeCfgKeyLookupBuf(void);

IK_Status_t IK_CFG_Init(IK_CFG_Init_t *init_struct)
{
    if (init_struct == NULL)
        return IK_ERROR;

    if (init_struct->ConfigMem_HW_Interface == NULL)
        return IK_ERROR;

    if (_cfg_init != NULL)
        return IK_ERROR;
    
    _cfg_init = init_struct;

    // Alloc the configuration table element buffer.
    if (_IK_CFG_AllocCfgBuf(IK_CFG_DEFAULT_CFG_BUF_SIZE) != IK_OK)
        return IK_ERROR;

    // Alloc the configuration key lookup table element buffer.
    if (_IK_CFG_AllocCfgKeyLookupBuf(IK_CFG_DEFAULT_KEY_LOOKUP_BUF_SIZE) != IK_OK)
        return IK_ERROR;

    return IK_OK;
}

void IK_CFG_Tick(void)
{
    // Check if the fracturing value is over the threshold value for compactification.
    if (_cfg_key_lookup_fract_val >= IK_CFG_BUFFER_FRACTURING_FC_THRES)
    {
        IK_CFG_KLT_ForceCompactification();
    }
    if (_cfg_fract_val >= IK_CFG_BUFFER_FRACTURING_FC_THRES)
    {
        IK_CFG_ForceCompactification();
    }
}

IK_Status_t IK_CFG_DeInit(void)
{
    // Free the configuration table element buffer.
    if (_IK_CFG_FreeCfgBuf() != IK_OK)
        return IK_ERROR;

    // Free the configuration key lookup table element buffer.
    if (_IK_CFG_FreeCfgKeyLookupBuf() != IK_OK)
        return IK_ERROR;

    _cfg_init = NULL;

    return IK_OK;
}

IK_Status_t IK_CFG_SaveToMemory(void)
{
    IK_Buffer_t save_buf;
    uint16_t i, j, cfg_memsize, cfg_ptr, val_ptr;

    // Check if the config is initialised and if the hw interface is present.
    if (_cfg.Size == 0 || _cfg.Entries == NULL ||
        _cfg_init == NULL|| _cfg_init->ConfigMem_HW_Interface == NULL)
        return IK_ERROR;

    // Check if size of pointers is 32bit.
    if (sizeof(void*) != 4) 
        return IK_ERROR;

    // Create a buffer for the configuration to save the memory in.
    // Calculate the needed size of the buffer.
    cfg_memsize = _cfg.Size * (sizeof(IK_CFG_Entry_t) - 2); // -2 because of 2 byte ptr instead of 4 bytes.
    save_buf.Size = 16 + cfg_memsize;
    for (i = 0; i < _cfg.Size; i++)
    {
        save_buf.Size += _cfg.Entries[i].ValueSize;
    }
    // Check if size fits in memory.
    if (save_buf.Size > _cfg_init->ConfigMem_HW_Interface->MemorySize)
        return IK_ERROR;
    // Allocate the buffer.
    save_buf.DataPtr = (uint8_t *) malloc(save_buf.Size * sizeof(uint8_t));
    if (save_buf.DataPtr == NULL)
        return IK_ERROR;
    
    // Set the first initialisation vectors.
    save_buf.DataPtr[0] = IK_LOWBYTE(save_buf.Size); // [0][1] : uint16 : Total Size
    save_buf.DataPtr[1] = IK_HIGHBYTE(save_buf.Size);
    save_buf.DataPtr[2] = IK_LOWBYTE(_cfg.Size); // [0][1] : uint16 : Config Table element count.
    save_buf.DataPtr[3] = IK_HIGHBYTE(_cfg.Size);

    // Initialise the pointers.
    cfg_ptr = 4;
    val_ptr = cfg_memsize + cfg_ptr;

    // Insert all config elements and value elements.
    for (i = 0; i < _cfg.Size; i++)
    {
        IK_CFG_Entry_t e = _cfg.Entries[i];

        save_buf.DataPtr[cfg_ptr++] = IK_BYTE0(e.Key);
        save_buf.DataPtr[cfg_ptr++] = IK_BYTE1(e.Key);
        save_buf.DataPtr[cfg_ptr++] = IK_BYTE2(e.Key);
        save_buf.DataPtr[cfg_ptr++] = IK_BYTE3(e.Key);

        save_buf.DataPtr[cfg_ptr++] = IK_BYTE0(e.ValueSize);
        save_buf.DataPtr[cfg_ptr++] = IK_BYTE1(e.ValueSize);

        save_buf.DataPtr[cfg_ptr++] = IK_BYTE0(val_ptr);
        save_buf.DataPtr[cfg_ptr++] = IK_BYTE1(val_ptr);

        for (j = 0; j < e.ValueSize; j++)
        {
            save_buf.DataPtr[val_ptr++] = ((uint8_t *) e.Value)[j];

            // Check for buffer overflow of val_ptr. If buffer overflow detected, free the buffer and return an error.
            if (val_ptr >= save_buf.Size)
            {
                free(save_buf.DataPtr);
                return IK_ERROR;
            }
        }

        // Check for buffer overflow of cfg_ptr. If buffer overflow detected, free the buffer and return an error.
        if (cfg_ptr >= save_buf.Size)
        {
            free(save_buf.DataPtr);
            return IK_ERROR;
        }
    }

    // Copy the buffer to the non-volatile memory.
    _cfg_init->ConfigMem_HW_Interface->WriteBuffer(0x0000, save_buf);

    // Free the buffer.
    free(save_buf.DataPtr);

    return IK_OK;
}

IK_Status_t IK_CFG_LoadConfigFromMemory(void)
{
    IK_CFG_Entry_t e;
    IK_Buffer_t read_buf;
    uint16_t cfg_size, i, j, p, val_ptr;

    // Check if the config is initialised and if the hw interface is present.
    if (_cfg.Size == 0 || _cfg.Entries == NULL ||
        _cfg_init == NULL|| _cfg_init->ConfigMem_HW_Interface == NULL)
        return IK_ERROR;

    // Retrieve the read buffer size.
    read_buf.Size = _cfg_init->ConfigMem_HW_Interface->ReadByte(0x0000);
    read_buf.Size += 0x00ff * ((uint16_t)_cfg_init->ConfigMem_HW_Interface->ReadByte(0x0001));

    // Read the buffer from the memory.
    read_buf = _cfg_init->ConfigMem_HW_Interface->ReadBuffer(0x0000, read_buf.Size);

    // Get the size of the config.
    cfg_size = read_buf.DataPtr[2];
    cfg_size += 0xff * ((uint16_t) read_buf.DataPtr[3]);

    // Reset the config by deallocating the buffer and allocating it again.
    _IK_CFG_FreeCfgBuf();
    _IK_CFG_AllocCfgBuf(IK_CFG_DEFAULT_CFG_BUF_SIZE);

    p = 4;
    // Read all config elements.
    for (i = 0; i < cfg_size; i++)
    {
        // Set the key and the value size.
        e.Key  = read_buf.DataPtr[p + 0];
        e.Key += 0xff * read_buf.DataPtr[p + 1];
        e.Key += 0xff00 * read_buf.DataPtr[p + 2];
        e.Key += 0xff0000 * read_buf.DataPtr[p + 3];

        e.ValueSize  = read_buf.DataPtr[p + 4];
        e.ValueSize += 0xff * read_buf.DataPtr[p + 5];

        // Retrieve the value pointer (this pointer references to the read buffer, not the processor's RAM)
        val_ptr = read_buf.DataPtr[p + 6];
        val_ptr += 0xff * read_buf.DataPtr[p + 7];

        // Allocate the value and check if the allocation was successful.
        e.Value = malloc(e.ValueSize);

        if (e.Value == NULL)
        {
            free(read_buf.DataPtr);
            return IK_ERROR;
        }

        // Fill the value
        for (j = 0; j < e.ValueSize; j++)
        {
            ((uint8_t *) e.Value)[j] = read_buf.DataPtr[val_ptr + j];
        }

        // Add the item to the config.
        if (IK_CFG_SetConfig(e) != IK_OK)
        {
            free(read_buf.DataPtr);
            return IK_ERROR;
        }

        p += (sizeof(IK_CFG_Entry_t) - 2); 
    }

    // Free the buffer.
    free(read_buf.DataPtr);

    return IK_OK;
}

/*--------------------------------------------------------------------------------------------------------------------*/

IK_Status_t IK_CFG_SetConfig(IK_CFG_Entry_t entry)
{
    uint16_t i;
    int32_t marker = -1;
    uint8_t iter = 0;

    // Check if config buffer is intialised.
    if (_cfg.Size == 0 || _cfg.Entries == NULL)
        return IK_ERROR;

    // Check for valid configuration.
    if (entry.Key == 0 ||
        entry.Value == NULL ||
        !IK_CFG_IsValueSizeValid(entry.Key, entry.ValueSize))
        return IK_ERROR;
    
    do
    {
        // Iterate over all configuration elements. If an empty space is found (and no previous empty one has been
        // found), mark that spot. If an element is found which has the same key as the element to set, replace the
        // marker with that spot and break out of the loop. Afterwards, if a suitable element space has been found the
        // variable marker will contain the index of it. If not, marker will still be -1. This can be used to judge, if
        // the config buffer has to be reallocated or if the element value setting can commence.
        for (i = 0; i < _cfg.Size; i++)
        {
            if (_cfg.Entries[i].Key == 0 && marker < 0)
            {
                marker = i;
            }
            else if (_cfg.Entries[i].Key == entry.Key)
            {
                marker = i;
                break;
            }
        }

        if (marker >= 0)
        {
            // Free the old value.
            free(_cfg.Entries[marker].Value);
            // Set the new entry.
            _cfg.Entries[marker] = entry;
        }
        else
        {
            // If not on the first iteration, return an error, as the previous allocation attempt did not yield any
            // space for the config to be set in.
            if (iter > 0)
                return IK_ERROR;

            // Realloc the config buffer for more space.
            if (_IK_CFG_ReallocCfgBuf(_cfg.Size + IK_CFG_DEFAULT_REALLOC_CFG_BUF_SIZE) != IK_OK)
                return IK_ERROR;
        }

        iter++;
    }
    while (marker < 0);
    
    return IK_OK;
}

IK_Status_t IK_CFG_ClearConfig(uint32_t key)
{
    uint16_t i;

    // Check if config buffer is intialised.
    if (_cfg.Size == 0 || _cfg.Entries == NULL)
        return IK_ERROR;

    // Check if the key is valid.
    if (key == 0)
        return IK_ERROR;
    
    // Iterate over all elements and delete all with the same key.
    for (i = 0; i < _cfg.Size; i++)
    {
        if (_cfg.Entries[i].Key == key)
        {
            // Free the old value.
            free(_cfg.Entries[i].Value);
            // Set the entry to NULL.
            _cfg.Entries[i] = (IK_CFG_Entry_t) IK_CFG_NULL_CFG_ENTRY;
            // Increase the fracturing index.
            _cfg_fract_val++;
        }
    }
    
    return IK_OK;
}

IK_CFG_Entry_t IK_CFG_GetConfig(uint32_t key)
{
    uint16_t i;

    // Check if config buffer is intialised.
    if (_cfg.Size == 0 || _cfg.Entries == NULL)
        return (IK_CFG_Entry_t) IK_CFG_NULL_CFG_ENTRY;

    // Check if the key is valid.
    if (key == 0)
        return (IK_CFG_Entry_t) IK_CFG_NULL_CFG_ENTRY;

    // Iterate over all elements to find the correct entry.
    for (i = 0; i < _cfg.Size; i++)
    {
        if (_cfg.Entries[i].Key == key)
        {
            return _cfg.Entries[i];
        }
    }

    // No matching entry found - return null struct.
    return (IK_CFG_Entry_t) IK_CFG_NULL_CFG_ENTRY;
}

IK_Bool_t IK_CFG_IsConfigSet(uint32_t key)
{
    uint16_t i;

    // Check if config buffer is intialised.
    if (_cfg.Size == 0 || _cfg.Entries == NULL)
        return FALSE;

    // Check if the key is valid.
    if (key == 0)
        return FALSE;

    // Iterate over all elements to find the correct entry.
    for (i = 0; i < _cfg.Size; i++)
    {
        if (_cfg.Entries[i].Key == key)
        {
            return TRUE;
        }
    }

    // No matching entry found.
    return FALSE;
}

IK_Bool_t IK_CFG_IsValueSizeValid(uint32_t key, uint16_t value_size)
{
    IK_CFG_KeyLookupItem_t klt_item;

    // Lookup the key
    klt_item = IK_CFG_KLT_LookupKey(key);

    // Check if the retrieval was successful & key is valid.
    if (klt_item.Key != key || key == 0)
        return FALSE;

    // Switch according to the value type.
    switch (klt_item.ValueType)
    {
        case IK_CFG_VALUE_TYPE_INT:
        case IK_CFG_VALUE_TYPE_UINT:
        case IK_CFG_VALUE_TYPE_FLOAT:
            // Fixed value size -> return true if sizes are equal.
            return (value_size == klt_item.ValueSize);
        case IK_CFG_VALUE_TYPE_STRING:
        case IK_CFG_VALUE_TYPE_BYTE_ARRAY:
            // Variable value size -> return true if given size is less than or equal to the lookup item's given value 
            // size.
            return (value_size <= klt_item.ValueSize);
        default:
            // If the value type cannot be evaluated, return false.
            return FALSE;
    }
}

IK_Status_t IK_CFG_ForceCompactification(void)
{
    // Check if config buffer is initialised.
    if (_cfg.Size == 0 || _cfg.Entries == NULL)
        return IK_ERROR;

    // Initialise compactification and return.
    return _IK_CFG_CompactifyCfgBuf();
}

/*--------------------------------------------------------------------------------------------------------------------*/

IK_Status_t IK_CFG_KLT_AddItem(IK_CFG_KeyLookupItem_t item)
{
    uint16_t i;
    uint8_t iter = 0;

    // Check if KLT is initialised
    if (_cfg_key_lookup.Entries == NULL || _cfg_key_lookup.Size == 0)
        return IK_ERROR;

    // Validate item
    if (item.Key == 0 || IK_CFG_KLT_HasKey(item.Key) ||
        item.ValueSize == 0 || item.ValueSize > IK_CFG_MAX_CFG_VALUE_SIZE)
        return IK_ERROR;

    do
    {
        // Check for first available space and add it to the list at that space.
        for (i = 0; i < _cfg_key_lookup.Size; i++)
        {
            // Check for availability
            if (_cfg_key_lookup.Entries[i].Key != 0)
            {
                // Slot not available.
                // Continue the search for an available slot.
                continue;
            }
            else
            {
                // Slot available.
                // Place the item in it.
                _cfg_key_lookup.Entries[i] = item;
                // End the search.
                break;
            }      
        }

        // Check if nothing available. If so, realloc KLT buffer.
        if (i == _cfg_key_lookup.Size)
        {
            // Check if already 2nd iteration. If so, return an error, as the
            // allocation of new memory for the buffer was not successful and
            // the item can not be added.
            if (iter > 0)
                return IK_ERROR;

            // Realloc the KLT buffer.
            if (_IK_CFG_ReallocCfgKeyLookupBuf(_cfg_key_lookup.Size + IK_CFG_DEFAULT_REALLOC_KEY_LOOKUP_BUF_SIZE) != IK_OK)
                return IK_ERROR;
        }

        iter++;
    }
    // Rerun the search if no available space has been found.
    while (i == _cfg_key_lookup.Size);

    return IK_OK;
}

IK_Status_t IK_CFG_KLT_RemoveItem(uint32_t key)
{
    uint16_t i;

    // Check if KLT is initialised
    if (_cfg_key_lookup.Entries == NULL || _cfg_key_lookup.Size == 0)
        return IK_ERROR;

    // Validate key.
    if (key == 0)
        return IK_ERROR;
    
    // Search for the item in question.
    for (i = 0; i < _cfg_key_lookup.Size; i++)
    {
        if (_cfg_key_lookup.Entries[i].Key == key)
        {
            // Found the element, delete it.
            _cfg_key_lookup.Entries[i] = (IK_CFG_KeyLookupItem_t) IK_CFG_NULL_KEY_LOOKUP_ITEM;
            // Add 1 to the fracturing value.
            _cfg_key_lookup_fract_val++;
        }
    }

    return IK_OK;
}

IK_CFG_KeyLookupItem_t IK_CFG_KLT_LookupKey(uint32_t key)
{
    uint16_t i;

    // Check if KLT is initialised
    if (_cfg_key_lookup.Entries == NULL || _cfg_key_lookup.Size == 0)
        return (IK_CFG_KeyLookupItem_t) IK_CFG_NULL_KEY_LOOKUP_ITEM;

    // Validate key.
    if (key == 0)
        return (IK_CFG_KeyLookupItem_t) IK_CFG_NULL_KEY_LOOKUP_ITEM;

    // Search for the item in question.
    for (i = 0; i < _cfg_key_lookup.Size; i++)
    {
        if (_cfg_key_lookup.Entries[i].Key == key)
        {
            // Found the element, return it.
            return _cfg_key_lookup.Entries[i];
        }
    }

    return (IK_CFG_KeyLookupItem_t) IK_CFG_NULL_KEY_LOOKUP_ITEM;
}

IK_Bool_t IK_CFG_KLT_HasKey(uint32_t key)
{
    uint16_t i;

    // Check if KLT is initialised
    if (_cfg_key_lookup.Entries == NULL || _cfg_key_lookup.Size == 0)
        return FALSE;

    // Loop through all lookup items and check if an item with the given key
    // exists.
    for (i = 0; i < _cfg_key_lookup.Size; i++)
    {
        if (_cfg_key_lookup.Entries[i].Key == key)
            return TRUE;
    }

    return FALSE;
}

IK_Status_t IK_CFG_KLT_ForceCompactification(void)
{
    // Check if KLT is initialised
    if (_cfg_key_lookup.Entries == NULL || _cfg_key_lookup.Size == 0)
        return IK_ERROR;

    // Initialise compactification and return.
    return _IK_CFG_CompactifyCfgKeyLookupBuf();
}

/*--------------------------------------------------------------------------------------------------------------------*/

static IK_Status_t _IK_CFG_AllocCfgBuf(uint16_t initial_size)
{
    if (initial_size == 0)
        return IK_ERROR;
    
    _cfg.Size = initial_size;
    _cfg.Entries = NULL;
    _cfg.Entries = (IK_CFG_Entry_t *) calloc(_cfg.Size, sizeof(IK_CFG_Entry_t));
    _cfg_fract_val = 0;

    if (_cfg.Entries == NULL)
        return IK_ERROR;

    return IK_OK;
}

static IK_Status_t _IK_CFG_ReallocCfgBuf(uint16_t new_size)
{
    IK_CFG_Entry_t *rptr;

    if (new_size == 0 ||
        new_size > IK_CFG_MAX_CFG_BUF_SIZE ||
        _cfg.Size == 0 ||
        _cfg.Entries == NULL)
        return IK_ERROR;
    
    // Realloc the memory.
    rptr = (IK_CFG_Entry_t *) realloc(_cfg.Entries, new_size * sizeof(IK_CFG_Entry_t));

    // Check if realloc was successful. If not, return an error.
    if (rptr == NULL)
        return IK_ERROR;

    // Memset the new memory to 0.
    if (new_size > _cfg.Size)
    {
        memset(rptr + _cfg.Size, 0x00, (new_size - _cfg.Size) * sizeof(IK_CFG_Entry_t));
    }

    // Overwrite the buffer to the new size and pointer.
    _cfg.Size = new_size;
    _cfg.Entries = rptr;

    return IK_OK;
}

static IK_Status_t _IK_CFG_CompactifyCfgBuf(void)
{
    uint16_t i, j;

    if (_cfg.Size == 0 || _cfg.Entries == NULL)
        return IK_ERROR;
    
    // Compactify all elements of the list.
    for (i = 0; i < _cfg.Size; i++)
    {
        // Check if empty. If not, continue.
        if (_cfg.Entries[i].Key == 0)
        {
            // Get the first element from the end and insert it in the current
            // location. If all elements from the end on to this location are
            // empty, the algorithm is complete.
            for (j = _cfg.Size - 1; j > i; j--)
            {
                // Check if entry present.
                if (_cfg.Entries[j].Key != 0)
                {
                    // Copy the j-entry to the i location and terminate the loop.
                    _cfg.Entries[i] = _cfg.Entries[j];
                    _cfg.Entries[j] = (IK_CFG_Entry_t) IK_CFG_NULL_CFG_ENTRY;
                    break;
                }
            }

            // If the whole upper area is empty, j will be equal to i. If so, 
            // terminate the compactification.
            if (i == j)
            {
                // Realloc to compactify memory usage.
                _IK_CFG_ReallocCfgBuf(i + 11);
                // Reset the fracturing value.
                _cfg_fract_val = 0;
                // Terminate.
                break;
            }
        }
    }

    return IK_OK;
}

static IK_Status_t _IK_CFG_FreeCfgBuf(void)
{
    uint16_t i;

    // Free the elements value.
    for (i = 0; i < _cfg.Size; i++)
    {
        free(_cfg.Entries[i].Value);
    }

    // Free the config itself.
    _cfg.Size = 0;
    free(_cfg.Entries);
    _cfg.Entries = NULL;
    return IK_OK;
}

static IK_Status_t _IK_CFG_AllocCfgKeyLookupBuf(uint16_t initial_size)
{
    if (initial_size == 0)
        return IK_ERROR;
    
    _cfg_key_lookup.Size = initial_size;
    _cfg_key_lookup.Entries = NULL;
    _cfg_key_lookup.Entries = (IK_CFG_KeyLookupItem_t *) calloc(_cfg_key_lookup.Size, sizeof(IK_CFG_KeyLookupItem_t));
    _cfg_key_lookup_fract_val = 0;

    if (_cfg_key_lookup.Entries == NULL)
        return IK_ERROR;
    
    return IK_OK;
}

static IK_Status_t _IK_CFG_ReallocCfgKeyLookupBuf(uint16_t new_size)
{
    IK_CFG_KeyLookupItem_t *rptr;

    if (new_size == 0 ||
        new_size > IK_CFG_MAX_KEY_LOOKUP_BUF_SIZE ||
        _cfg_key_lookup.Size == 0 ||
        _cfg_key_lookup.Entries == NULL)
        return IK_ERROR;
    
    // Realloc the memory.
    rptr = (IK_CFG_KeyLookupItem_t *) realloc(_cfg_key_lookup.Entries, new_size * sizeof(IK_CFG_KeyLookupItem_t));

    // Check if realloc was successful. If not, return an error.
    if (rptr == NULL)
        return IK_ERROR;

    // Memset the new memory to 0.
    if (new_size > _cfg_key_lookup.Size)
    {
        memset(rptr + _cfg_key_lookup.Size, 0x00, (new_size - _cfg_key_lookup.Size) * sizeof(IK_CFG_KeyLookupItem_t));
    }

    // Overwrite the buffer to the new size and pointer.
    _cfg_key_lookup.Size = new_size;
    _cfg_key_lookup.Entries = rptr;

    return IK_OK;
}

static IK_Status_t _IK_CFG_CompactifyCfgKeyLookupBuf(void)
{
    uint16_t i, j;

    if (_cfg_key_lookup.Size == 0 || _cfg_key_lookup.Entries == NULL)
        return IK_ERROR;
    
    // Compactify all elements of the list.
    for (i = 0; i < _cfg_key_lookup.Size; i++)
    {
        // Check if empty. If not, continue.
        if (_cfg_key_lookup.Entries[i].Key == 0)
        {
            // Get the first element from the end and insert it in the current
            // location. If all elements from the end on to this location are
            // empty, the algorithm is complete.
            for (j = _cfg_key_lookup.Size - 1; j > i; j--)
            {
                // Check if entry present.
                if (_cfg_key_lookup.Entries[j].Key != 0)
                {
                    // Copy the j-entry to the i location and terminate the loop.
                    _cfg_key_lookup.Entries[i] = _cfg_key_lookup.Entries[j];
                    _cfg_key_lookup.Entries[j] = (IK_CFG_KeyLookupItem_t) IK_CFG_NULL_KEY_LOOKUP_ITEM;
                    break;
                }
            }

            // If the whole upper area is empty, j will be equal to i. If so, 
            // terminate the compactification.
            if (i == j)
            {
                // Realloc to compactify memory usage.
                _IK_CFG_ReallocCfgKeyLookupBuf(i + 11);
                // Reset the fracturing value.
                _cfg_key_lookup_fract_val = 0;
                // Terminate.
                break;
            }
        }
    }

    return IK_OK;
}

static IK_Status_t _IK_CFG_FreeCfgKeyLookupBuf(void)
{
    _cfg_key_lookup.Size = 0;
    free(_cfg_key_lookup.Entries);
    _cfg_key_lookup.Entries = NULL;
    return IK_OK;
}