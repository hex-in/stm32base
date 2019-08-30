#ifndef INCLUDED_RESOURCE_H_
#define INCLUDED_RESOURCE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************
 *                     Macros
 ******************************************************/
#ifndef MIN
#define MIN( x, y ) ((x) < (y) ? (x) : (y))
#endif /* ifndef MIN */

/* Suppress unused parameter warning */
#ifndef UNUSED_PARAMETER
#define UNUSED_PARAMETER( x ) ( (void)(x) )
#endif

#ifndef RESULT_ENUM
#define RESULT_ENUM( prefix, name, value )  prefix ## name = (value)
#endif /* ifndef RESULT_ENUM */


/* These Enum result values are for Resource errors
 * Values: 4000 - 4999
 */
#define RESOURCE_RESULT_LIST( prefix )  \
    RESULT_ENUM(  prefix, SUCCESS,                         0 ),   /**< Success */                           \
    RESULT_ENUM(  prefix, UNSUPPORTED,                     7 ),   /**< Unsupported function */              \
    RESULT_ENUM(  prefix, OFFSET_TOO_BIG,               4001 ),   /**< Offset past end of resource */       \
    RESULT_ENUM(  prefix, FILE_OPEN_FAIL,               4002 ),   /**< Failed to open resource file */      \
    RESULT_ENUM(  prefix, FILE_SEEK_FAIL,               4003 ),   /**< Failed to seek to requested offset in resource file */ \
    RESULT_ENUM(  prefix, FILE_READ_FAIL,               4004 ),   /**< Failed to read resource file */

#define resource_get_size( resource ) ((resource)->size)

/******************************************************
 *                    Constants
 ******************************************************/

#define RESOURCE_ENUM_OFFSET  ( 1300 )

/******************************************************
 *                Enumerations
 ******************************************************/

/**
 * Result type for HEXIN Resource function
 */
typedef enum
{
    RESOURCE_RESULT_LIST( RESOURCE_ )
} resource_result_t;

/******************************************************
 *                 Type Definitions
 ******************************************************/


/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/


/* @} */
#ifdef __cplusplus
} /*extern "C" */
#endif

#endif /* ifndef INCLUDED_RESOURCE_H_ */
