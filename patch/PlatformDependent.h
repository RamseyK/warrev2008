#ifndef __SRC_SHARED_PLATFORMDEPENDENT__H_
#define __SRC_SHARED_PLATFORMDEPENDENT__H_

/* Use PLATFORM_ for native API-specific issues, workarounds and implementations. */
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(__WIN32__)
#   define PLATFORM_WIN32
#else
#   define PLATFORM_OTHER
#endif

/* Use COMPILER_ for compilation-specific issues and workarounds. */
#if defined(_MSC_VER)
#   define COMPILER_MSVC
#elif defined(DFS_CPPB)
#   define COMPILER_BORLAND
#elif defined(__GNUG__)
#   define COMPILER_GCC
#endif


#ifdef PLATFORM_WIN32
#   define strcasecmp _stricmp
#else
#   define _stricmp strcasecmp
#endif

#ifdef PLATFORM_WIN32
    int inet_aton(const char *cp, struct in_addr *inp);
#endif

#endif

