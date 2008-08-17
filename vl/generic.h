/** @file    generic.h
 ** @author  Andrea Vedaldi
 ** @brief   Generic
 **/

/* AUTORIGHTS
Copyright 2007 (c) Andrea Vedaldi and Brian Fulkerson

This file is part of VLFeat, available in the terms of the GNU
General Public License version 2.
*/

#ifndef VL_GENERIC_H
#define VL_GENERIC_H

#include <stddef.h>
#include <time.h>
#include <assert.h>

/** @brief Library version string */
#define VL_VERSION_STRING "0.9.1"

/** @def VL_INLINE
 ** @brief Adds appropriate inline function qualifier
 ** @sa main-style-inline
 **/

/** @def VL_EXPORT
 ** @brief Declares a DLL exported symbol
 ** @sa @ref generic-symbols
 **/

#ifdef WIN32
#define VL_INLINE static __inline
#define snprintf _snprintf
#define isnan _isnan

#ifdef VL_BUILD_DLL
#define VL_EXPORT __declspec(dllexport)
#else
#define VL_EXPORT __declspec(dllimport)
#endif

#else /* UNIX */
#define VL_INLINE static __inline__

#ifdef VL_BUILD_DLL
#define VL_EXPORT __attribute__((visibility ("default")))
#else
#define VL_EXPORT
#endif

#endif

/** @internal @brief Stringify argument helper
 ** @see ::VL_STRINGIFY
 **/
#define VL_STRINGIFY_(x) # x

/** @brief Stringify macro
 **
 ** @param x macro to stringify
 **
 ** For instance, the following fragment
 ** @code
 ** #define A x.y.z
 ** printf(VL_STRINGIFY(A)) ;
 ** @endcode
 ** will print the string @c x.y.z.
 **/
#define VL_STRINGIFY(x) VL_STRINGIFY_(x)

/** ------------------------------------------------------------------
 ** @name Atomic data types
 **
 ** @{
 **/

typedef long long           vl_int64 ;   /**< signed 64-bit integer. */
typedef int                 vl_int32 ;   /**< signed 32-bit integer. */
typedef short               vl_int16 ;   /**< signed 16-bit integer. */
typedef char                vl_int8  ;   /**< signed  8-bit integer. */

typedef long long unsigned  vl_uint64 ;  /**< unsigned 64-bit integer. */
typedef int       unsigned  vl_uint32 ;  /**< unsigned 32-bit integer. */
typedef short     unsigned  vl_uint16 ;  /**< unsigned 16-bit integer. */
typedef char      unsigned  vl_uint8 ;   /**< unsigned  8-bit integer. */

typedef int                 vl_int ;     /**< @c int.                  */
typedef unsigned int        vl_uint ;    /**< @c unsigned @c int.      */

typedef int                 vl_bool ;    /**< boolean */
#ifndef WIN64
typedef long                vl_ptrint ;  /**< integer type holding a pointer. */
#else
typedef long long           vl_ptrint ;
#endif
/** @} */

/** ------------------------------------------------------------------
 ** @name Format strings for @c printf
 **
 ** @{
 **/

/** @def VL_FL_INT64 @brief @c printf length flag for ::vl_int64, ::vl_uint64. */
/** @def VL_FL_INT32 @brief @c printf length flag for ::vl_int32, ::vl_uint32. */
/** @def VL_FL_INT16 @brief @c printf length flag for ::vl_int16, ::vl_uint16. */
/** @def VL_FL_INT8  @brief @c printf length flag for ::vl_int8,  ::vl_uint8.  */

#define VL_FL_INT64  "ll"
#define VL_FL_INT32  ""
#define VL_FL_INT16  "h"
#define VL_FL_INT8   "hh"

/** @} */

/** @brief Largest integer (math constant) */
#define VL_BIG_INT  0x7FFFFFFFL

/** @brief Smallest integer (math constant) */
#define VL_SMALL_INT  (- VL_BIG_INT - 1)

/** @brief Largest unsigned integer (math constant) */
#define VL_BIG_UINT 0xFFFFFFFFUL

/** @brief Logarithm of 2 (math constant)*/
#define VL_LOG_OF_2 0.693147180559945

/** @brief Pi (math constant) */
#define VL_PI 3.141592653589793

/** @brief Single precision epsilon (math constant) 
 **
 ** Difference of the smallest representable number greater
 ** than 1.0 and 1.0.
 **
 **/
#define VL_EPSILON_F 1.19209290E-07F

/** @brief Double precision epsilon (math constant) 
 **
 ** Difference of the smallest representable number greater
 ** than 1.0 and 1.0.
 **/
#define VL_EPSILON_D 2.220446049250313e-16

/* 
   For the code below: An ANSI C compiler takes the two expressions,
   LONG_VAR and CHAR_VAR, and implicitly casts them to the type of the
   first member of the union. Refer to K&R Second Edition Page 148,
   last paragraph.
*/

/** @brief IEEE single quiet NaN constant */
static union { vl_uint32 raw ; float value ; } 
  const vl_nan_f = 
    { 0x7FC00000UL } ;

/** @brief IEEE single infinity constant */
static union { vl_uint32 raw ; float value ; } 
  const vl_infinity_f = 
    { 0x7F800000UL } ;

/** @brief IEEE double quiet NaN constant */
static union { vl_uint64 raw ; double value ; } 
  const vl_nan_d = 
    { 0x7FF8000000000000ULL } ;

/** @brief IEEE double infinity constant */
static union { vl_uint64 raw ; float value ; } 
  const vl_infinity_d = 
    { 0x7FF0000000000000ULL } ;

/** @brief Single NaN (not signaling) */
#define VL_NAN_F (vl_nan_f.value)

/** @brief Single Infinity (not signaling) */
#define VL_INFINITY_F (vl_infinity_f.value)

/** @brief Double NaN (not signaling) */
#define VL_NAN_D (vl_nan_d.value)

/** @brief Double Infinity (not signaling) */
#define VL_INFINITY_D (vl_infinity_d.value)


/** ------------------------------------------------------------------
 ** @name Heap allocation
 ** @{ 
 **/

VL_EXPORT
void vl_set_alloc_func (void *(*malloc_func)  (size_t),
                        void *(*realloc_func) (void*,size_t),
                        void *(*calloc_func)  (size_t, size_t),
                        void  (*free_func)    (void*)) ;
VL_INLINE void *vl_malloc  (size_t n) ;
VL_INLINE void *vl_realloc (void *ptr, size_t n) ;
VL_INLINE void *vl_calloc  (size_t n, size_t size) ;
VL_INLINE void  vl_free    (void* ptr) ;

/** @} */

/** ------------------------------------------------------------------
 ** @name Logging
 ** @{ 
 **/

VL_EXPORT
void vl_set_printf_func (int(*printf_func)(char const *str, ...)) ;

/** @def VL_PRINTF
 ** @brief Call user-customizable @c printf function
 **
 ** @param format format string.
 ** @param ... @c printf variable arguments.
 **
 ** The function calls the user customizable @c printf.
 **
 ** @return results of the user-customizable @c printf.
 **/
#define VL_PRINTF(format, ...) \
((*vl_printf_func)((format), __VA_ARGS__))

/** @def VL_PRINT
 ** @brief Call user-customizable @c printf function (no varags)
 **
 ** @param string string to print.
 **
 ** This macro is the same as ::VL_PRINTF() and should
 ** be used when the variable list of arguments is empty.
 **
 ** @return results of the user-customizable @c printf.
 **/
#define VL_PRINT(string) \
  ((*vl_printf_func)(string))


/** @} */

/** ------------------------------------------------------------------
 ** @name Error handling 
 ** @{ 
 **/

/** @brief Last error code */
extern VL_EXPORT int vl_err_no ;

/** @brief Error description maximum length 
 **
 ** This is the size of the ::vl_err_msg buffer.
 **/
#define VL_ERR_MSG_LEN 1024

/** @brief Last error description */
extern VL_EXPORT char vl_err_msg [VL_ERR_MSG_LEN] ;

#define VL_ERR_OK       0  /**< No error */
#define VL_ERR_OVERFLOW 1  /**< Buffer overflow error */
#define VL_ERR_ALLOC    2  /**< Resource allocation error */
#define VL_ERR_BAD_ARG  3  /**< Bad argument or illegal data error */
#define VL_ERR_IO       4  /**< Input/output error */
#define VL_ERR_EOF      5  /**< End-of-file or end-of-sequence error */
#define VL_ERR_NO_MORE  5  /**< End-of-sequence @deprecated */

/** @} */

/** ------------------------------------------------------------------
 ** @name Common operations
 ** @{
 **/

/** @brief Min operation
 ** @param x value
 ** @param y value
 ** @return the minimum of @a x and @a y.
 **/
#define VL_MIN(x,y) (((x)<(y))?(x):(y))

/** @brief Max operation
 ** @param x value.
 ** @param y value.
 ** @return the maximum of @a x and @a y.
 **/
#define VL_MAX(x,y) (((x)>(y))?(x):(y))

/** @brief Signed left shift operation
 **
 ** The macro is equivalent to the builtin @c << operator, but it
 ** supports negative shifts too.
 ** 
 ** @param x value.
 ** @param n number of shift positions.
 ** @return @c x << n .
 **/
#define VL_SHIFT_LEFT(x,n) (((n)>=0)?((x)<<(n)):((x)>>-(n)))
/* @} */

/** ------------------------------------------------------------------
 **/

VL_EXPORT
char const * vl_get_version_string () ;

/** @name Measuring time
 ** @{
 **/
VL_EXPORT 
void vl_tic() ;

VL_EXPORT 
double vl_toc() ;
/** @} */

/** @name Endianness detection and conversion
 ** @{
 **/

#define VL_LITTLE_ENDIAN 0  /**< little endian architecture. */
#define VL_BIG_ENDIAN    1  /**< big endian architecture. */
       
/** @def VL_ENDIANNESS
 ** @brief Host Endianness.
 **
 ** This macro is equal to ::VL_BIG_ENDIAN or ::VL_LITTLE_ENDIAN
 ** depending on the endianness of the host.
 **
 ** @see @ref generic-data-models.
 **/
#if                                                \
  defined(__LITTLE_ENDIAN__)                   ||  \
  defined(__i386__)  || defined(__ia64__)      ||  \
  defined(WIN32)     || defined(__x86_64)      ||  \
  defined(__alpha__) || defined(__alpha)       ||  \
  defined(__arm__)   || defined(__SYMBIAN32__) ||  \
  (defined(__mips__) && defined(__MIPSEL__)) 
#define VL_ENDIANNESS VL_LITTLE_ENDIAN
#else
#define VL_ENDIANNESS VL_BIG_ENDIAN
#endif

VL_INLINE int  vl_get_endianness () ;
VL_INLINE void vl_swap_host_big_endianness_8 (void *dst, void* src) ;
VL_INLINE void vl_swap_host_big_endianness_4 (void *dst, void* src) ;
VL_INLINE void vl_swap_host_big_endianness_2 (void *dst, void* src) ;
/** @} */

/** ------------------------------------------------------------------
 ** @brief Get endianness
 ** @return @c ::VL_BIG_ENDIAN or ::VL_LITTLE_ENDIAN depending on the
 ** host endianness.
 ** @sa @ref generic-data-models
 **/

VL_INLINE int
vl_get_endianness () 
{
  return VL_ENDIANNESS ;
}

/** ------------------------------------------------------------------
 ** @brief Host <-> big endian transformation for 8-bytes value
 **
 ** @param dst destination 8-byte buffer.
 ** @param src source 8-byte bufffer.
 ** @see @ref generic-data-models.
 **/

VL_INLINE void
vl_swap_host_big_endianness_8 (void *dst, void* src)
{
  char *dst_ = (char*) dst ;
  char *src_ = (char*) src ;
#if VL_ENDIANNESS == VL_BIG_ENDIAN
    dst_ [0] = src_ [0] ;
    dst_ [1] = src_ [1] ;
    dst_ [2] = src_ [2] ;
    dst_ [3] = src_ [3] ;
    dst_ [4] = src_ [4] ;
    dst_ [5] = src_ [5] ;
    dst_ [6] = src_ [6] ;
    dst_ [7] = src_ [7] ;
#else 
    dst_ [0] = src_ [7] ;
    dst_ [1] = src_ [6] ;
    dst_ [2] = src_ [5] ;
    dst_ [3] = src_ [4] ;
    dst_ [4] = src_ [3] ;
    dst_ [5] = src_ [2] ;
    dst_ [6] = src_ [1] ;
    dst_ [7] = src_ [0] ;
#endif
}

/** ------------------------------------------------------------------
 ** @brief Host <-> big endian transformation for 4-bytes value
 **
 ** @param dst destination 4-byte buffer.
 ** @param src source 4-byte bufffer.
 ** @sa @ref generic-data-models
 **/

VL_INLINE void
vl_swap_host_big_endianness_4 (void *dst, void* src)
{
  char *dst_ = (char*) dst ;
  char *src_ = (char*) src ;
#if VL_ENDIANNESS == VL_BIG_ENDIAN
    dst_ [0] = src_ [0] ;
    dst_ [1] = src_ [1] ;
    dst_ [2] = src_ [2] ;
    dst_ [3] = src_ [3] ;
#else 
    dst_ [0] = src_ [3] ;
    dst_ [1] = src_ [2] ;
    dst_ [2] = src_ [1] ;
    dst_ [3] = src_ [0] ;
#endif
}

/** ------------------------------------------------------------------
 ** @brief Host <-> big endian transformation for 2-bytes value
 **
 ** @param dst destination 2-byte buffer.
 ** @param src source 2-byte bufffer.
 ** @see generic-data-models.
 **/

VL_INLINE void
vl_swap_host_big_endianness_2 (void *dst, void* src)
{
  char *dst_ = (char*) dst ;
  char *src_ = (char*) src ;
#if VL_ENDIANNESS == VL_BIG_ENDIAN
    dst_ [0] = src_ [0] ;
    dst_ [1] = src_ [1] ;
#else
    dst_ [0] = src_ [1] ;
    dst_ [1] = src_ [0] ;
#endif
}

extern VL_EXPORT int   (*vl_printf_func)  (char const * format, ...) ;
extern VL_EXPORT void *(*vl_malloc_func)  (size_t) ;
extern VL_EXPORT void *(*vl_realloc_func) (void*,size_t) ;
extern VL_EXPORT void *(*vl_calloc_func)  (size_t, size_t) ;
extern VL_EXPORT void  (*vl_free_func)    (void*) ;          

/** ------------------------------------------------------------------
 ** @brief Call customizable @c malloc function
 ** @param n number of bytes to allocate.
 **
 ** The function calls the user customizable @c malloc.
 **
 ** @return result of @c malloc
 **/

VL_INLINE 
void*
vl_malloc (size_t n)
{
  return (*vl_malloc_func)(n) ;
}

/** ------------------------------------------------------------------
 ** @brief Call customizable @c resize function
 **
 ** @param ptr buffer to reallocate.
 ** @param n   number of bytes to allocate.
 **
 ** The function calls the user-customizable @c realloc.
 **
 ** @return result of the user-customizable @c realloc.
 **/

VL_INLINE 
void*
vl_realloc (void* ptr, size_t n)
{
  return (*vl_realloc_func)(ptr, n) ;
}

/** ------------------------------------------------------------------
 ** @brief Call customizable @c calloc function
 **
 ** @param n    size of each element in byte.
 ** @param size size of the array to allocate (number of elements).
 **
 ** The function calls the user-customizable @c calloc.
 **
 ** @return result of the user-customizable @c calloc.
 **/

VL_INLINE 
void*
vl_calloc (size_t n, size_t size)
{
  return (*vl_calloc_func)(n, size) ;
}

/** ------------------------------------------------------------------
 ** @brief Call customizable @c free function
 **
 ** @param ptr buffer to free.
 **
 ** The function calls the user customizable @c free.
 **/

VL_INLINE 
void
vl_free (void *ptr)
{
  (*vl_free_func)(ptr) ;
}

/* VL_GENERIC_H */
#endif
