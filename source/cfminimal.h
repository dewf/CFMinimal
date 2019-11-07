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

//#ifndef __cplusplus
//#include <stdbool.h>
//#endif

// Minimal Core Foundation stuff ======
typedef const void *CFTypeRef;

typedef ssize_t CFIndex;
CFMINIMAL_API extern const CFIndex kCFNotFound;
typedef struct {
	CFIndex location;
	CFIndex length;
} CFRange;
CFMINIMAL_API extern const CFRange dl_CFRangeZero;

inline CFRange CDECL CFRangeMake(CFIndex location, CFIndex length)
{
	CFRange ret;
	ret.location = location;
	ret.length = length;
	return ret;
}
inline CFIndex CFRangeEnd(CFRange range) {
	return range.location + range.length;
}

enum {
	kCFCompareCaseInsensitive = 1,
	kCFCompareBackwards = 4,		/* Starting from the end of the string */
	kCFCompareAnchored = 8,		    /* Only at the specified starting point */
	kCFCompareNonliteral = 16,		/* If specified, loose equivalence is performed (o-umlaut == o, umlaut) */
	kCFCompareLocalized = 32,		/* User's default locale is used for the comparisons */
	kCFCompareNumerically = 64,		/* Numeric comparison is used; that is, Foo2.txt < Foo7.txt < Foo25.txt */
	kCFCompareDiacriticInsensitive = 128, /* If specified, ignores diacritics (o-umlaut == o) */
	kCFCompareWidthInsensitive = 256, /* If specified, ignores width differences ('a' == UFF41) */
	kCFCompareForcedOrdering = 512 /* If specified, comparisons are forced to return either kCFCompareLessThan or kCFCompareGreaterThan if the strings are equivalent but not strictly equal, for stability when sorting (e.g. "aaa" > "AAA" with kCFCompareCaseInsensitive specified) */
};
typedef unsigned int CFStringCompareFlags;

typedef enum {
	/* Fixed-width types */
	kCFNumberSInt8Type = 1,
	kCFNumberSInt16Type = 2,
	kCFNumberSInt32Type = 3,
	kCFNumberSInt64Type = 4,
	kCFNumberFloat32Type = 5,
	kCFNumberFloat64Type = 6,	/* 64-bit IEEE 754 */
	/* Basic C types */
	kCFNumberCharType = 7,
	kCFNumberShortType = 8,
	kCFNumberIntType = 9,
	kCFNumberLongType = 10,
	kCFNumberLongLongType = 11,
	kCFNumberFloatType = 12,
	kCFNumberDoubleType = 13,
	/* Other */
	kCFNumberCFIndexType = 14,
	kCFNumberNSIntegerType = 15,
	kCFNumberCGFloatType = 16,
	kCFNumberMaxType = 16
} CFNumberType;

typedef enum {
	kCFURLPOSIXPathStyle = 0,
	// type 1 is deprecated
	kCFURLWindowsPathStyle = 2
} CFURLPathStyle;

#endif

