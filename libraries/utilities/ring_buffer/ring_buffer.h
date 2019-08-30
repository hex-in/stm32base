#pragma once

#include <stdint.h>
#include "hexin_result.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

typedef /*@abstract@*/ /*@immutable@*/ struct
{
    uint8_t*  buffer;
    uint32_t  size;
    volatile uint32_t  head; /* Index of the buffer to read from */
    volatile uint32_t  tail; /* Index of the buffer to write to */
} hexin_ring_buffer_t;

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/


/**
 *
 *  @addtogroup ringbuf     Ring buffer
 *  @ingroup utilities
 *
 * This library implements a lock-free ringbuffer as described in Donald E. Knuth, The Art of Computer Programming, Volume 3: Sorting and Searching.
 * Note that since the last entry of the ring buffer is not used in a lock free implementation, you should allocate one more entry then the total
 * number of entries you plan to use.
 *
 *  @{
 */


/** Initialize a ring buffer.
 *
 * Note: This is a non-locking ring buffer implementation that will store a
 *       maximum of buffer_size - 1 bytes of data.
 *
 * @param[out] ring_buffer : Pointer to the ring buffer structure to be initialized.
 * @param[in]  buffer      : Pointer to the buffer to use for the ring buffer.
 * @param[in]  buffer_size : Size of the buffer (maximum buffer_size - 1 bytes will be stored).
 *
 * @return @ref hexin_result_t
 */
hexin_result_t ring_buffer_init       ( /*@out@*/ hexin_ring_buffer_t* ring_buffer, /*@keep@*/ uint8_t* buffer, uint32_t buffer_size );

/** De-initialize a ring buffer.
 *
 * @param[in] ring_buffer : Pointer to the ring buffer structure.
 *
 * @return @ref hexin_result_t
 */
hexin_result_t ring_buffer_deinit     ( hexin_ring_buffer_t* ring_buffer );

/** Write to a ring buffer.
 *
 * @param[in] ring_buffer : Pointer to the ring buffer structure.
 * @param[in] data        : Pointer to the data to write to the buffer.
 * @param[in] data_length : Length of data in bytes.
 *
 * @return Length of data successfully written to the buffer.
 */
uint32_t       ring_buffer_write      ( hexin_ring_buffer_t* ring_buffer, const uint8_t* data, uint32_t data_length );

/** Return the amount of used space in a ring buffer.
 *
 * @param[in] ring_buffer : Pointer to the ring buffer structure.
 *
 * @return Length of data currently in the buffer.
 */
uint32_t       ring_buffer_used_space ( hexin_ring_buffer_t* ring_buffer );

/** Return the amount of available space in a ring buffer.
 *
 * @param[in] ring_buffer : Pointer to the ring buffer structure.
 *
 * @return Length of available space in the buffer.
 */
uint32_t       ring_buffer_free_space ( hexin_ring_buffer_t* ring_buffer );

/** Get a pointer to the start of data in a ring buffer.
 *
 * @param[in] ring_buffer  : Pointer to the ring buffer structure.
 * @param[out] data        : Address of the pointer for the data start in the buffer.
 * @param[out] data_length : Length of contiguous data bytes in the buffer.
 *
 * @return @ref hexin_result_t
 */
hexin_result_t ring_buffer_get_data   ( hexin_ring_buffer_t* ring_buffer, uint8_t** data, uint32_t* contiguous_bytes );

/** Consume (discard) data in a ring buffer.
 *
 * @param[in] ring_buffer    : Pointer to the ring buffer structure.
 * @param[in] bytes_consumed : Number of bytes to consume.
 *
 * @return @ref hexin_result_t
 */
hexin_result_t ring_buffer_consume    ( hexin_ring_buffer_t* ring_buffer, uint32_t bytes_consumed );

/** Read data from a ring buffer.
 *
 * @param[in] ring_buffer           : Pointer to the ring buffer structure.
 * @param[in] data                  : Pointer to the buffer for read data.
 * @param[in] data_length           : Length of the data buffer.
 * @param[out] number_of_bytes_read : Number of bytes read from the ring buffer.
 *
 * @return @ref hexin_result_t
 */
hexin_result_t ring_buffer_read       ( hexin_ring_buffer_t* ring_buffer, uint8_t* data, uint32_t data_length, uint32_t* number_of_bytes_read );

/** @} */

#ifdef __cplusplus
} /* extern "C" */
#endif
