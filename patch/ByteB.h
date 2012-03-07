#ifndef BYTEB_H
#define BYTEB_H

#include "Common.h"
#define item_offset (uint64)12297829382473034410
#include "PlatformDependent.h"

/* Removes warnings about "unsafe" functions in VC. */
#define _CRT_SECURE_NO_DEPRECATE

#define bit(b) (1<<b)

#ifdef __GNUG__
__extension__
typedef long long   int64;
#else
typedef long long   int64;
#endif
typedef long        int32;
typedef short       int16;
typedef char        int8;

typedef unsigned long       DWORD;

#ifdef __GNUG__
__extension__
typedef unsigned long long  uint64;
#else
typedef unsigned long long  uint64;
#endif
typedef unsigned long       uint32;
typedef unsigned short      uint16;
typedef unsigned char       uint8;
typedef unsigned short      unicode;

#ifdef __GNUG__
__extension__
typedef signed long long  sint64;
#else
typedef signed long long  sint64;
#endif

typedef signed long         sint32;
typedef signed short        sint16;
typedef signed char         sint8;


// Short for 'unsigned long'
typedef unsigned long       ulong;
// Short for 'unsigned int'
typedef unsigned int        uint;
// Short for 'unsigned short'
typedef unsigned short      ushort;
// Short for 'unsigned char'
typedef unsigned char       uchar;


// This is an integer type that has the same size as pointers
typedef long                intptr;
typedef unsigned long       uintptr;

// Ignore stupid warnings

#ifdef COMPILER_MSVC
#  pragma warning( disable : 4267 ) // conversion from 'size_t' to 'int', possible loss of data
#  pragma warning( disable : 4786 ) // identifier was truncated to '255' characters in the debug information
#  pragma warning( disable : 4309 ) // Truncation of constant value
#endif


#if defined (__GNUC__) && (defined (CPU_x86) || defined (CPU_x86_64))
// Optimized routines for x86{_64}
static inline const uint16 swap16 (const uint16 x)
{
    uint16 ret;
    __asm ("xchgb %%al,%%ah" : "=a" (ret) : "a" (x));
    return ret;
}
static inline const uint32 swap32 (const uint32 x)
{
    uint32 ret;
    __asm ("bswapl %%eax" : "=a" (ret) : "a" (x));
    return ret;
}
#if defined (CPU_x86)
static inline const uint64 swap64 (const uint64 x)
{
    uint64 ret;
    __asm ("bswapl %%eax\nbswapl %%edx\nxchgl %%eax,%%edx" : "=A" (ret) : "A" (x));
    return ret;
}
#else
static inline const uint64 swap64 (const uint64 x)
{
    uint64 ret;
    __asm ("bswapq %%rax" : "=A" (ret) : "A" (x));
    return ret;
}
#endif
static inline void xchg32 (void *a, void *b)
{
    __asm ("movl (%0),%%eax\nxchgl (%1),%%eax\nmovl %%eax,(%0)"
            :: "r" (a), "r" (b) : "eax");
}
#else
static inline const uint16 swap16 (const uint16 x)
{ return (x >> 8) | (x << 8); }
static inline const uint32 swap32 (const uint32 x)
{ return (x >> 24) | ((x >> 8) & 0xff00) | ((x << 8) & 0xff0000) | (x << 24); }
static inline const uint64 swap64 (const uint64 x)
{ return ((x >> 56) /*& 0x00000000000000ffULL*/) | ((x >> 40) & 0x000000000000ff00ULL) |
    ((x >> 24) & 0x0000000000ff0000ULL) | ((x >> 8 ) & 0x00000000ff000000ULL) |
        ((x << 8 ) & 0x000000ff00000000ULL) | ((x << 24) & 0x0000ff0000000000ULL) |
        ((x << 40) & 0x00ff000000000000ULL) | ((x << 56) /*& 0xff00000000000000ULL*/); }
static inline void xchg32 (void *a, void *b)
{
    uint32 tmp = *(uint32 *)a;
    *(uint32 *)a = *(uint32 *)b;
    *(uint32 *)b = tmp;
}
#endif

/// Compute the square of the argument
    template <typename T>
static T Square (T a)
{ return a * a; }

inline sint64 abs64(sint64 x) {return (sint64)x>=0?(sint64)x:-(sint64)x;};

inline uint32 ustrlen(unicode* str) { for (int i=0;;i++) if (str[i] == 0) return i; };

typedef std::list<uint16> ServerSequences; //This is for the sequence blocker.
#define Sequence_Block_Range 256 //This defines the range... if you have the memory space, you can increase it.

inline uint16 _atoi(char* str) {
    return str?atoi(str):0;
}
inline uint32 _atol(char* str) {
    return str?atol(str):0;
}
inline float _atof(char* str) {
    return str?(float)atof(str):0.f;
}
inline double _atod(char* str) {
    return str?atof(str):0.f;
}
inline bool _atob(char* str) {
    return (str?atoi(str):0)?1:0;
}

inline uint16 _atoi(const char* str) {
#ifdef _DEBUG
    assert(str != 0);
#endif
    return str?atoi((char*)str):0;
}
inline uint32 _atol(const char* str) {
#ifdef _DEBUG
    assert(str != 0);
#endif
    return str?atol((char*)str):0;
}
inline float _atof(const char* str) {
#ifdef _DEBUG
    assert(str != 0);
#endif
    return str?(float)atof((char*)str):0.f;
}
inline double _atod(const char* str) {
#ifdef _DEBUG
    assert(str != 0);
#endif
    return str?atof((char*)str):0.f;
}
inline bool _atob(const char* str) {
#ifdef _DEBUG
    assert(str != 0);
#endif
    return (str?atoi((char*)str):0)?1:0;
}

#define atoi _atoi
#define atol _atol
#define atof _atof
#define atod _atod
#define atob _atob

#endif
