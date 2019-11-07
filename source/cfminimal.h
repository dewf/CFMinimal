#ifndef __CFMINIMAL_H__
#define __CFMINIMAL_H__

#ifdef _WIN32
#   define CF_PLATFORM_WINDOWS
#elif defined __linux__
#   define CF_PLATFORM_LINUX
#endif

#ifdef CF_PLATFORM_WINDOWS
#   ifdef CFMINIMAL_EXPORTS
#       define CFMINIMAL_API __declspec(dllexport)
#   else
#       define CFMINIMAL_API __declspec(dllimport)
#   endif
#   ifdef _WIN64
typedef __int64 ssize_t;
#   else
typedef __int32 ssize_t;
#   endif
#   include <Windows.h> // for CDECL
//#   define CDECL __cdecl
#elif defined CF_PLATFORM_LINUX
#   define CFMINIMAL_API __attribute__((visibility("default")))
#   define CDECL
#   include <sys/types.h> // for ssize_t
#endif

#include <stddef.h>
#include <stdint.h>

//#define DLHANDLE(ns,x) struct __dl_##ns##x; typedef struct __dl_##ns##x* dl_##ns##x##Ref
//#define DLHANDLE_WITH_MUTABLE(ns,x) struct __dl_##ns##x; typedef const struct __dl_##ns##x* dl_##ns##x##Ref; typedef struct __dl_##ns##x* dl_##ns##Mutable##x##Ref

#ifndef __cplusplus
#include <stdbool.h>
#endif

// Minimal Core Foundation stuff ======
typedef const void *dl_CFTypeRef;

typedef ssize_t dl_CFIndex;
CFMINIMAL_API extern const dl_CFIndex dl_kCFNotFound;
typedef struct {
	dl_CFIndex location;
	dl_CFIndex length;
} dl_CFRange;
CFMINIMAL_API extern const dl_CFRange dl_CFRangeZero;

inline dl_CFRange CDECL dl_CFRangeMake(dl_CFIndex location, dl_CFIndex length)
{
	dl_CFRange ret;
	ret.location = location;
	ret.length = length;
	return ret;
}
inline dl_CFIndex dl_CFRangeEnd(dl_CFRange range) {
	return range.location + range.length;
}

enum {
	dl_kCFCompareCaseInsensitive = 1,
	dl_kCFCompareBackwards = 4,		/* Starting from the end of the string */
	dl_kCFCompareAnchored = 8,		    /* Only at the specified starting point */
	dl_kCFCompareNonliteral = 16,		/* If specified, loose equivalence is performed (o-umlaut == o, umlaut) */
	dl_kCFCompareLocalized = 32,		/* User's default locale is used for the comparisons */
	dl_kCFCompareNumerically = 64,		/* Numeric comparison is used; that is, Foo2.txt < Foo7.txt < Foo25.txt */
	dl_kCFCompareDiacriticInsensitive = 128, /* If specified, ignores diacritics (o-umlaut == o) */
	dl_kCFCompareWidthInsensitive = 256, /* If specified, ignores width differences ('a' == UFF41) */
	dl_kCFCompareForcedOrdering = 512 /* If specified, comparisons are forced to return either kCFCompareLessThan or kCFCompareGreaterThan if the strings are equivalent but not strictly equal, for stability when sorting (e.g. "aaa" > "AAA" with kCFCompareCaseInsensitive specified) */
};
typedef unsigned int dl_CFStringCompareFlags;

typedef enum {
	/* Fixed-width types */
	dl_kCFNumberSInt8Type = 1,
	dl_kCFNumberSInt16Type = 2,
	dl_kCFNumberSInt32Type = 3,
	dl_kCFNumberSInt64Type = 4,
	dl_kCFNumberFloat32Type = 5,
	dl_kCFNumberFloat64Type = 6,	/* 64-bit IEEE 754 */
	/* Basic C types */
	dl_kCFNumberCharType = 7,
	dl_kCFNumberShortType = 8,
	dl_kCFNumberIntType = 9,
	dl_kCFNumberLongType = 10,
	dl_kCFNumberLongLongType = 11,
	dl_kCFNumberFloatType = 12,
	dl_kCFNumberDoubleType = 13,
	/* Other */
	dl_kCFNumberCFIndexType = 14,
	dl_kCFNumberNSIntegerType = 15,
	dl_kCFNumberCGFloatType = 16,
	dl_kCFNumberMaxType = 16
} dl_CFNumberType;

typedef enum {
	dl_kCFURLPOSIXPathStyle = 0,
	// type 1 is deprecated
	dl_kCFURLWindowsPathStyle = 2
} dl_CFURLPathStyle;


#endif




