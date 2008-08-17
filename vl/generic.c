/** @file    generic.c
 ** @author  Andrea Vedaldi
 ** @brief   Generic - Definition
 **/

/* AUTORIGHTS
Copyright 2007 (c) Andrea Vedaldi and Brian Fulkerson

This file is part of VLFeat, available in the terms of the GNU
General Public License version 2.
*/

/** 
 @mainpage VLFeat -- Vision Lab Features Library 

 @version __VLFEAT_VERSION__
 @author Andrea Vedaldi  (vedaldi@cs.ucla.edu)
 @author Brian Fulkerson (bfulkers@cs.ucla.edu)
 
 @par Copyright &copy; 2007-08 Andrea Vedaldi and Brian Fulkerson

 <em>VLFeat C library contains implementations of common computer
 vision algorithms, with a special focus on visual features for
 matching image regions. Applications include structure from motion and
 object and category detection and recognition.

 The library is accurate and easy to use, understand and modify. In
 order to maximize simplicity, the library is free of clutter and
 different algorithms are only weakly dependent. The code is portable
 (largely C-89 compatible) and embeds extensive documentation.</em>

 @section main-contents Contents

 - @ref design          "Design Concepts"
   - @ref design-objects     "Objects" 
   - @ref design-resources   "Memory and resource management" 
   - @ref design-threads     "Multi-threading"
   - @ref design-portability "Portability"
 - @ref main-glossary "Glossary"
 - Support functionalities
   - @ref generic.h   "Atomic data types, errors, and others"
   - @ref mathop.h    "Math operations"
   - @ref random.h    "Random number generator"
   - @ref stringop.h  "String operations"
   - @ref imop.h      "Image operations"
   - @ref rodrigues.h "Rodrigues formula"
   - @ref mexutils.h  "MATLAB MEX helper functions"
 - Algorithms
   - @ref sift.h     "Scale Invariant Feature Transform (SIFT)"
   - @ref mser.h     "Maximally Stable Extremal Regions (MSER)"
   - @ref ikmeans.h  "Integer K-means (IKM)"
   - @ref hikmeans.h "Hierarchical Integer K-means (HIKM)"
   - @ref aib.h      "Agglomerative Information Bottleneck (AIB)"
   - @ref dhog.h     "Dense Histogram of Oriented Gradients (DHOG)"

 @section design VLFeat Design Concepts

 VLFeat follows a simple but rigorous design that makes it portable
 and simple to use in conjunction with high level language such as
 MATLAB. This section illustrates and motivates the aspects of the
 design that are relevant to the users of the library. Most of the
 features discussed in here are implemented in the @ref generic.h
 module.

 @subsection design-objects Objects

 Most of VLFeat functionalities are implemented as opaque data
 structures, to which we refer as "objects". Typically, you create an
 object by means of a constructor function and dispose of it by means
 of a destructor function. The data member of the object should not be
 accessed directly, but by means of appropriate accessor functions
 (typically containing the @c _get and _set keywords in their names).

 @subsection design-resources Memory and Resource Management

 Resource management in VLFeat is minimal. In most cases, you can
 assume that VLFeat does not provide any resource management
 functionality at all. Objects or memory blocks allocated by the
 library but owned by the client must be explicitly disposed. The
 following rule helps identifying such blocks and objects:
 
 <b> The client owns a memory block or object if, and only if, it is
   returned by a library call containing the keywords @c _new or @c
   _copy, or by the allocator functions ::vl_malloc, ::vl_calloc,
   ::vl_realloc.</b>
 
 More in detail, the following rules apply:

 - Memory is the only managed resource. Other resources used by the
   library (e.g. files) are either passed to the library by the
   client, or acquired and released within the scope of a library
   function call.
 - The memory manager can be customized through ::vl_set_alloc_func
   (which sets the implementations of ::vl_malloc, ::vl_realloc,
   ::vl_calloc and ::vl_free). The library allocates memory only through
   these functions.
 - The memory manager is global to all threads.
 - At any moment, there is only one memory manager in
   existence. ::vl_set_alloc_func can be used only before any other
   function is invoked, or right before or after the previous memory
   manager has relinquished all memory.
 - Such rules apply both to the library client and to the library
   implementation. The only exception regards the memory allocated in
   the global library state, which uses the native memory manager.

  These rules make it possible to undo all the work done by the
  library at any given instant. Disposing the memory allocated by the
  custom memory manager essentially "resets" the library (except for
  its global state). This functionality is used extensively in the
  implementation of MATLAB MEX functions that access the library to
  support abrupt interruption.

   @note The rules guarantee that all memory allocated by the library
     at any given time is allocated by the same memory manager, except
     for the global state that should survive the "reset"
     operation. In order to support multiple memory managers, one
     should keep track of the allocator of each object (or memory
     block). Moreover, partial de-allocation of a pool of objects is
     dangerous, as such objects may be referred by other objects that
     are not being de-allocated, corrupting their state. A proper
     solution to the latter problem is the retain/release mechanism
     implemented, for instance, by Apple Core Foundation or Cocoa.

  @subsection design-threads Multi-threading

  The library is currently <em>not</em> thread safe, but this support
  will be added in a future release.

  The library is almost entirely reentrant. The only thread-sensitive
  operations are on the global library state and are limited to:

  - Global library configuration (e.g. ::vl_set_alloc_func).
  - Random number generator state (@ref random.h).
  - Error handling (e.g. ::vl_err_no).

  @subsection design-portability Portability features

  Platform dependent details are isolated in the @ref generic.h
  library module. These include:

  - Atomic types (e.g. ::vl_int32).
  - Special syntaxes for the declaration of symbols exported by the library
    and inline functions (e.g. ::VL_EXPORT).
  - Host-dependent conversion of data endianess 
    (e.g. ::vl_swap_host_big_endianness_8()).

  VLFeat uses processor specific features (e.g. Intel SSE) if those
  are available at compile time.
 
  @see http://www.macresearch.org/how_to_properly_use_sse3_and_ssse3_and_future_intel_vector_extensions_0

  @section main-glossary Glossary

  - <b>Column-major.</b> A <em>M x N </em> matrix <em>A</em> is
  stacked with column-major order as the sequence \f$(A_{11}, A_{21},
  \dots, A_{12}, \dots)\f$. More in general, when stacking a multi
  dimensional array this indicates that the first index is the one
  varying most quickly, with the other followed in the natural order.
  - <b>Opaque structure.</b> A structure is opaque if the user is not supposed
  to access its member directly, but through appropriate interface functions.
  Opaque structures are commonly used to define objects.
  - <b>Row-major.</b> A <em>M x N </em> matrix <em>A</em> is
  stacked with row-major order as the sequence \f$(A_{11}, A_{12},
  \dots, A_{21}, \dots)\f$. More in general, when stacking a multi
  dimensional array this indicates that the last index is the one
  varying most quickly, with the other followed in reverse order.
  - <b>Feature frame.</b> A <em>feature frame</em> is the geometrical
  description of a visual features. For instance, the frame of 
  a @ref sift.h "SIFT feature" is oriented disk and the frame of
  @ref mser.h "MSER feature" is either a compact and connected set or
  a disk.
  - <b>Feature descriptor.</b> A <em>feature descriptor</em> is a quantity
  (usually a vector) which describes compactly the appearance of an
  image region (usually corresponding to a feature frame).
**/

/** 
 @file generic.h

 This module provides the following functionalities:
 
 - @ref generic-data-models
 - @ref generic-error  
 - @ref generic-heap
 - @ref generic-logging

 @section generic-data-models Data models

 Processors may differ in the way they represent multi-byte data types. This
 regards the 
 <em>endianness</em> of the assignment of data types to memory addresses
 and the <em>size</em> of the such types.
 
 <b>Endianness</b> concerns how multi-byte data types (such as 16, 32 and 64 bits
 integers) are stored into the addressable memory. All processors assign the bytes of a multi-byte data types
 to a contiguous range of memory addresses (e.g. a 16-bit integer could
 be assigned to the addresses <c>0x10001</c> and <c>0x10002</c>). There is
 however an ambiguity on which bytes are
 assigned to which addresses in the range.
  
 - The assignment is <em>big endian</em>, or in <em>network order</em>,
   if the most significant byte of the multi-byte data type is assigned to 
   the smaller memory address.
 
 - Analogously, the assignment is <em>little endian</em> if  the least significant
   byte is assigned to the smaller memory address.
  
 @remark Generally speaking, PPC processors are big endian and x86 processors
 are little endian.<br/><br/>
 
 @remark The names &ldquo;big endian&rdquo; and &ldquo;little endian&rdquo; are 
 a little confusing. &ldquo;Big endian&rdquo; means &ldquo;big endian first&rdquo;, i.e.
 the most significant byte comes first in the addressable space. Similarly,
 &ldquo;little endian&rdquo; means &ldquo;little endian first&rdquo;, 
 in the sense that the least significant byte comes first.
 
 In most cases endianness is not a concern as data is stored and read to the addressable space
 by one processor (or multiple processors of the same kind). Endianness 
 becomes a concern when data is communicated. 
 This includes: exchanging data with other processors 
 that use a different convention,
 transmitting data over a network, and storing data
 to a file.
 
 VLFeat provides a few functions to perform the¢ necessary conversions in the
 latter two cases. The function ::vl_get_endianness() detects endianness. The functions
 ::vl_swap_host_big_endianness_8(), ::vl_swap_host_big_endianness_4(),
 ::vl_swap_host_big_endianness_2() change the endiannes of the data from 
 the host convention (either little or big endian) to the network convention
 (big endian) and viceversa.
 
 The <b>size of C atomic data types</b> (such as
 @c short, @c int, @c long and so on) also differs with different
 processors. This may be a concern in several ways: communication of data,
 overflow of the numerical representations, need to 
 process data of a specific size (e.g. 8-bit image pixels). To this end,
 the C-99 standard header <c>stdint.h</c> provides types that have guaranteed
 sizes. Unfortunately Visual C++ does not implement this header, so 
 VLFeat reproduces part of its functionalities.
 
 In particular, VLFeat provides a full range of atomic data types
 (::vl_int8, ::vl_int16, ::vl_int32, etc.) which are mapped to the appropriate
 C native data type according to the following table.
 Notice that in practice the only relevant differences
 are the size of <code>long</code> and the size of pointers. 
   
 <table><caption>32-bit and 64-bit data models</caption>
 <tr style="font-weight:bold;">
   <td>Data model</td>
   <td >short</td>
   <td>int</td>
   <td>long</td>
   <td>long long</td>
   <td>pointers</td>
   <td>architecture</td>
  </tr>
 <tr>
   <td>ILP32</td>
   <td style="background-color:#ffa;">16</td>
   <td style="background-color:#afa;">32</td>
   <td style="background-color:#afa;">32</td>
   <td >64</td>
   <td style="background-color:#afa;">32</td>
   <td>common 32 bit architectures</td>
 </tr>
 <tr>
   <td>LP64</td>
   <td style="background-color:#ffa;">16</td>
   <td style="background-color:#afa;">32</td>
   <td>64</td>
   <td>64</td>
   <td>64</td>
   <td>UNIX-64 (Linux, Mac OS X)</td>
 </tr>
 <tr>
   <td>ILP64</td>
   <td style="background-color:#ffa;">16</td>
   <td>64</td>
   <td>64</td>
   <td>64</td>
   <td>64</td>
   <td>Alpha, Cray</td>
 </tr>
 <tr>
   <td>SLIP64</td>
   <td>64</td>
   <td>64</td>
   <td>64</td>
   <td>64</td>
   <td>64</td>
   <td></td>
 </tr>
 <tr>
   <td>LLP64</td>
   <td style="background-color:#ffa;">16</td>
   <td style="background-color:#afa;">32</td>
   <td style="background-color:#afa;">32</td>
   <td>64</td>
   <td>64</td>
   <td>Windows-64</td>
 </tr>
 </table>
 
 @section generic-symbols Exported symbols and inline functions

 Creating a dynamically linked library requires non-standard features
 of the C compiler to identify which symbols should be exported by
 the library and which should not. Similarly, the declaration of inline
 functions is a non-standard feature in C-89 compilers such as Visual C.
  
 The details on how such declarations are performed on each specific platform
 are hidden by the following macros:

 - ::VL_EXPORT declares symbols exported by the library.
 - ::VL_INLINE declares an inline function.
 
 @par "Example:"
 The following header file declares a function @c f that
 should be visible from outside the library.
 @code 
 #include <vl/generic.h>
 VL_EXPORT void f () ;
 VL_EXPORT int i ;
 @endcode
 Notice that the macro ::VL_EXPORT needs not to be included again
 when the function is defined.
 
 @par "Example:"
 The following header file declares an inline function @c f:
 @code
 #include <vl/generic.h>
 VL_INLINE int f() ;
 
 VL_INLINE int f() { return 1 ; }
 @endcode
 Here the first instruction defines the function @c f, where the second
 declares it. Notice that since this is an inline function, its definition
 must be found in the header file rather than in an implementation file.
 Notice also that definition and declaration can be merged.
 
 These macros translate according to the following tables:

  <table style="font-size:70%;">
  <caption>Macros for exporting library symbols</caption>
  <tr>
  <td>Platform</td>
  <td>Macro name</td>
  <td>Value when building the library</td>
  <td>Value when importing the library</td>
  </tr>
  <tr>
  <td>Unix/GCC</td>
  <td>::VL_EXPORT</td>
  <td>empty (assumes <c>-visibility=hidden</c> GCC option)</td>
  <td><c>__attribute__((visibility ("default")))</c></td>
  </tr>
  <tr>
  <td>Win/Visual C++</td>
  <td>::VL_EXPORT</td>
  <td>@c __declspec(dllexport)</td>
  <td>@c __declspec(dllimport)</td>
  </tr>
  </table>

  <table style="font-size:70%;">
  <caption>Macros for declaring inline functions</caption>
  <tr>
  <td>Platform</td>
  <td>Macro name</td>
  <td>Value</td>
  </tr>
  <tr>
  <td>Unix/GCC</td>
  <td>::VL_INLINE</td>
  <td>static inline</td>
  </tr>
  <tr>
  <td>Win/Visual C++</td>
  <td>::VL_INLINE</td>
  <td>static __inline</td>
  </tr>
  </table>

  @section generic-error Error handling

  Error handling uses the same style of the standard C library. Most
  functions return 0 when they succeed and -1 when they fail, and
  set the global variable ::vl_err_no with a code identifying the
  error occurred. This variable is never set on success and should
  be examined right after an error had occurred.

  @section generic-heap Heap allocation

  VLFeat uses the ::vl_malloc(), ::vl_realloc(), ::vl_calloc() and
  ::vl_free() functions to allocate the heap. Normally these functions
  are mapped to the underlying standard C library implementations. However
  ::vl_set_alloc_func() can be used to map them to other implementations.
  For instance, in MATALB MEX files these functions are mapped to 
  the MATLAB equivalent which has a garbage collection mechanism to cope
  with interruptions during execution.

  @section generic-logging Logging

  VLFeat uses the macros ::VL_PRINT and ::VL_PRINTF to print progress or
  debug informations. These functions are normally mapped to the @c
  printf function of the underlying standard C library. However
  ::vl_set_printf_func() can be used to map it to a different
  implementation. For instance, in MATLAB MEX files this function is
  mapped to @c mexPrintf. Setting the function to @c NULL disables
  logging.


**/

#include "generic.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

VL_EXPORT int vl_err_no = 0 ;
VL_EXPORT char vl_err_msg [VL_ERR_MSG_LEN] = "" ;

/** ------------------------------------------------------------------
 ** @brief Get version string
 ** @return library version string
 **/

VL_EXPORT
char const *
vl_get_version_string ()
{
  return VL_VERSION_STRING 
    " (" 
#if   VL_ENDIANNESS == VL_BIG_ENDIAN
    "big endian"
#elif VL_ENDIANNESS == VL_LITTLE_ENDIAN
    "little endian"
#else
    "unkown endianness"
#endif
    ")" ;
}

/** @internal@brief A printf that does not do anything */
static int
do_nothing_printf (char const* format, ...)
{
  return 0 ;
}

/** @internal@brief Customizable @c malloc function pointer */
void *(*vl_malloc_func)  (size_t)          = &malloc ;
                                            
/** @internal@brief Customizable @c realloc function pointer */
void *(*vl_realloc_func) (void*,size_t)    = &realloc ;

/** @internal@brief Customizable @c calloc function pointer */
void *(*vl_calloc_func)  (size_t, size_t) = &calloc ;

/** @internal@brief Customizable @c free function pointer */
void  (*vl_free_func)    (void*)            = &free ;
                                            
/** @internal@brief Customizable @c printf function pointer */
int   (*vl_printf_func)  (char const *, ...)=  printf ; /* &do_nothing_printf ;*/

/** ------------------------------------------------------------------
 ** @brief Set memory allocation functions
 ** @param malloc_func  pointer to @c malloc.
 ** @param realloc_func pointer to @c realloc.
 ** @param calloc_func  pointer to @c calloc.
 ** @param free_func    pointer to @c free.
 **/

VL_EXPORT
void vl_set_alloc_func (void *(*malloc_func)  (size_t),
                        void *(*realloc_func) (void*, size_t),
                        void *(*calloc_func)  (size_t, size_t),
                        void  (*free_func)    (void*))
{
  vl_malloc_func  = malloc_func ;
  vl_realloc_func = realloc_func ;
  vl_calloc_func  = calloc_func ;
  vl_free_func    = free_func ;
}

/** ------------------------------------------------------------------
 ** @brief Set printf function
 ** @param printf_func  pointer to @c printf.
 ** Let @c print_func be NULL to disable printf.
 **/

VL_EXPORT
void 
vl_set_printf_func (int(*printf_func) (char const *format, ...))
{
  vl_printf_func  = printf_func ? printf_func : do_nothing_printf ;
}

clock_t tic_mark ; /**< @internal Store clock time for ::vl_tic() */

/** ------------------------------------------------------------------
 ** @brief Set time reference
 **/

VL_EXPORT
void vl_tic() 
{
  tic_mark = clock() ;
}

/** ------------------------------------------------------------------
 ** @brief Get time since reference
 **
 ** Returns the processor time elapsed since ::vl_tic() was called.
 **
 ** @return time in seconds.
 **/

VL_EXPORT
double vl_toc()
{
  return (double) (clock() - tic_mark) / CLOCKS_PER_SEC ;
}
