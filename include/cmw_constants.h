#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/******************************************************
 *                      Macros
 ******************************************************/

#ifndef RESULT_ENUM
#define RESULT_ENUM( prefix, name, value )  prefix ## name = (value)
#endif /* ifndef RESULT_ENUM */

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/
/* These Enum result values are for CMW errors
 * Values: 1000 - 1999
 */

#define CMW_RESULT_LIST( prefix )  \
    RESULT_ENUM( prefix, SUCCESS,                         0 ),   /**< Success */                           \
    RESULT_ENUM( prefix, PENDING,                         1 ),   /**< Pending */                           \
    RESULT_ENUM( prefix, TIMEOUT,                         2 ),   /**< Timeout */                           \
    RESULT_ENUM( prefix, BADARG,                          5 ),   /**< Bad Arguments */                     \
    RESULT_ENUM( prefix, UNFINISHED,                     10 ),   /**< Operation not finished yet (maybe aborted) */ \
    RESULT_ENUM( prefix, PARTIAL_RESULTS,              1003 ),   /**< Partial results */                   \
    RESULT_ENUM( prefix, INVALID_KEY,                  1004 ),   /**< Invalid key */                       \
    RESULT_ENUM( prefix, DOES_NOT_EXIST,               1005 ),   /**< Does not exist */                    \
    RESULT_ENUM( prefix, NOT_AUTHENTICATED,            1006 ),   /**< Not authenticated */                 \
    RESULT_ENUM( prefix, NOT_KEYED,                    1007 ),   /**< Not keyed */                         \
    RESULT_ENUM( prefix, IOCTL_FAIL,                   1008 ),   /**< IOCTL fail */                        \
    RESULT_ENUM( prefix, BUFFER_UNAVAILABLE_TEMPORARY, 1009 ),   /**< Buffer unavailable temporarily */    \
    RESULT_ENUM( prefix, BUFFER_UNAVAILABLE_PERMANENT, 1010 ),   /**< Buffer unavailable permanently */    \
    RESULT_ENUM( prefix, CONNECTION_LOST,              1012 ),   /**< Connection lost */                   \
    RESULT_ENUM( prefix, OUT_OF_EVENT_HANDLER_SPACE,   1013 ),   /**< Cannot add extra event handler */    \
    RESULT_ENUM( prefix, SEMAPHORE_ERROR,              1014 ),   /**< Error manipulating a semaphore */    \
    RESULT_ENUM( prefix, FLOW_CONTROLLED,              1015 ),   /**< Packet retrieval cancelled due to flow control */ \
    RESULT_ENUM( prefix, NO_CREDITS,                   1016 ),   /**< Packet retrieval cancelled due to lack of bus credits */ \
    RESULT_ENUM( prefix, NO_PACKET_TO_SEND,            1017 ),   /**< Packet retrieval cancelled due to no pending packets */ \
    RESULT_ENUM( prefix, CORE_CLOCK_NOT_ENABLED,       1018 ),   /**< Core disabled due to no clock */    \
    RESULT_ENUM( prefix, CORE_IN_RESET,                1019 ),   /**< Core disabled - in reset */         \
    RESULT_ENUM( prefix, UNSUPPORTED,                  1020 ),   /**< Unsupported function */             \
    RESULT_ENUM( prefix, BUS_WRITE_REGISTER_ERROR,     1021 ),   /**< Error writing to WLAN register */   \
    RESULT_ENUM( prefix, SDIO_BUS_UP_FAIL,             1022 ),   /**< SDIO bus failed to come up */       \
    RESULT_ENUM( prefix, JOIN_IN_PROGRESS,             1023 ),   /**< Join not finished yet */   \
    RESULT_ENUM( prefix, NETWORK_NOT_FOUND,            1024 ),   /**< Specified network was not found */   \
    RESULT_ENUM( prefix, INVALID_JOIN_STATUS,          1025 ),   /**< Join status error */   \
    RESULT_ENUM( prefix, UNKNOWN_INTERFACE,            1026 ),   /**< Unknown interface specified */ \
    RESULT_ENUM( prefix, SDIO_RX_FAIL,                 1027 ),   /**< Error during SDIO receive */   \
    RESULT_ENUM( prefix, HWTAG_MISMATCH,               1028 ),   /**< Hardware tag header corrupt */   \
    RESULT_ENUM( prefix, RX_BUFFER_ALLOC_FAIL,         1029 ),   /**< Failed to allocate a buffer to receive into */   \
    RESULT_ENUM( prefix, BUS_READ_REGISTER_ERROR,      1030 ),   /**< Error reading a bus hardware register */   \
    RESULT_ENUM( prefix, THREAD_CREATE_FAILED,         1031 ),   /**< Failed to create a new thread */   \
    RESULT_ENUM( prefix, QUEUE_ERROR,                  1032 ),   /**< Error manipulating a queue */   \
    RESULT_ENUM( prefix, BUFFER_POINTER_MOVE_ERROR,    1033 ),   /**< Error moving the current pointer of a packet buffer  */   \
    RESULT_ENUM( prefix, BUFFER_SIZE_SET_ERROR,        1034 ),   /**< Error setting size of packet buffer */   \
    RESULT_ENUM( prefix, THREAD_STACK_NULL,            1035 ),   /**< Null stack pointer passed when non null was reqired */   \
    RESULT_ENUM( prefix, THREAD_DELETE_FAIL,           1036 ),   /**< Error deleting a thread */   \
    RESULT_ENUM( prefix, SLEEP_ERROR,                  1037 ),   /**< Error sleeping a thread */ \
    RESULT_ENUM( prefix, BUFFER_ALLOC_FAIL,            1038 ),   /**< Failed to allocate a packet buffer */ \
    RESULT_ENUM( prefix, NO_PACKET_TO_RECEIVE,         1039 ),   /**< No Packets waiting to be received */ \
    RESULT_ENUM( prefix, INTERFACE_NOT_UP,             1040 ),   /**< Requested interface is not active */ \
    RESULT_ENUM( prefix, DELAY_TOO_LONG,               1041 ),   /**< Requested delay is too long */ \
    RESULT_ENUM( prefix, INVALID_DUTY_CYCLE,           1042 ),   /**< Duty cycle is outside limit 0 to 100 */ \
    RESULT_ENUM( prefix, PMK_WRONG_LENGTH,             1043 ),   /**< Returned pmk was the wrong length */ \
    RESULT_ENUM( prefix, UNKNOWN_SECURITY_TYPE,        1044 ),   /**< AP security type was unknown */ \
    RESULT_ENUM( prefix, WEP_NOT_ALLOWED,              1045 ),   /**< AP not allowed to use WEP - it is not secure - use Open instead */ \
    RESULT_ENUM( prefix, WPA_KEYLEN_BAD,               1046 ),   /**< WPA / WPA2 key length must be between 8 & 64 bytes */ \
    RESULT_ENUM( prefix, FILTER_NOT_FOUND,             1047 ),   /**< Specified filter id not found */ \
    RESULT_ENUM( prefix, SPI_ID_READ_FAIL,             1048 ),   /**< Failed to read 0xfeedbead SPI id from chip */ \
    RESULT_ENUM( prefix, SPI_SIZE_MISMATCH,            1049 ),   /**< Mismatch in sizes between SPI header and SDPCM header */ \
    RESULT_ENUM( prefix, ADDRESS_ALREADY_REGISTERED,   1050 ),   /**< Attempt to register a multicast address twice */ \
    RESULT_ENUM( prefix, SDIO_RETRIES_EXCEEDED,        1051 ),   /**< SDIO transfer failed too many times. */ \
    RESULT_ENUM( prefix, NULL_PTR_ARG,                 1052 ),   /**< Null Pointer argument passed to function. */ \
    RESULT_ENUM( prefix, THREAD_FINISH_FAIL,           1053 ),   /**< Error deleting a thread */ \
    RESULT_ENUM( prefix, WAIT_ABORTED,                 1054 ),   /**< Semaphore/mutex wait has been aborted */ \
    RESULT_ENUM( prefix, SET_BLOCK_ACK_WINDOW_FAIL,    1055 ),   /**< Failed to set block ack window */ \
    RESULT_ENUM( prefix, DELAY_TOO_SHORT,              1056 ),   /**< Requested delay is too short */ \
    RESULT_ENUM( prefix, INVALID_INTERFACE,            1057 ),   /**< Invalid interface provided */ \
    RESULT_ENUM( prefix, WEP_KEYLEN_BAD,               1058 ),   /**< WEP / WEP_SHARED key length must be 5 or 13 bytes */ \
    RESULT_ENUM( prefix, HANDLER_ALREADY_REGISTERED,   1059 ),   /**< EAPOL handler already registered */ \
    RESULT_ENUM( prefix, AP_ALREADY_UP,                1060 ),   /**< Soft AP or P2P group owner already up */ \
    RESULT_ENUM( prefix, EAPOL_KEY_PACKET_M1_TIMEOUT,  1061 ),   /**< Timeout occurred while waiting for EAPOL packet M1 from AP */ \
    RESULT_ENUM( prefix, EAPOL_KEY_PACKET_M3_TIMEOUT,  1062 ),   /**< Timeout occurred while waiting for EAPOL packet M3 from AP, which may indicate incorrect WPA2/WPA passphrase */ \
    RESULT_ENUM( prefix, EAPOL_KEY_PACKET_G1_TIMEOUT,  1063 ),   /**< Timeout occurred while waiting for EAPOL packet G1 from AP */ \
    RESULT_ENUM( prefix, EAPOL_KEY_FAILURE,            1064 ),   /**< Unknown failure occurred during the EAPOL key handshake */ \
    RESULT_ENUM( prefix, MALLOC_FAILURE,               1065 ),   /**< Memory allocation failure */ \
    RESULT_ENUM( prefix, ACCESS_POINT_NOT_FOUND,       1066 ),   /**< Access point not found */ \
    RESULT_ENUM( prefix, RTOS_ERROR,                   1067 ),   /**< RTOS operation failed */ \
    RESULT_ENUM( prefix, CLM_BLOB_DLOAD_ERROR,         1068 ),   /**< CLM blob download failed */

/******************************************************
 *                 Type Definitions
 ******************************************************/

/**
 * Common result type for HEXIN functions
 */
typedef enum
{
    CMW_RESULT_LIST( CMW_ )
} cmw_result_t;


#ifndef UNUSED_PARAMETER
#define UNUSED_PARAMETER(x) /*@-noeffect@*/ ( (void)(x) ) /*@+noeffect@*/
#endif

/******************************************************
 *                    Structures
 ******************************************************/

typedef uint32_t          cmw_time_t;          /* Time value in milliseconds */

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/


#ifdef __cplusplus
} /* extern "C" */
#endif
