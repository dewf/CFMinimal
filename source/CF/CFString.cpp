#include "CFString.h"

#include <algorithm>
#include <cctype>

#include "CFMutableString.h"

using namespace cf;

StringRef String::makeConstantString(const char *cStr)
{
	static std::map<std::wstring, StringRef> constantStrings; // if this is outside the function there's no guarantee it's initialized before first use
	//
	auto key = utf8_to_wstring(cStr);

	auto found = constantStrings.find(key);
	if (found != constantStrings.end()) {
		return found->second;
	}
	else {
		// create anew
		auto ret = new String(cStr);
		ret->isEternal = true;
		constantStrings[key] = (StringRef)ret->retain();
		return ret;
	}
}

CFRange String::find(StringRef toFind, CFStringCompareFlags compareOptions)
{
	auto supportedFlags = kCFCompareCaseInsensitive;
	if (compareOptions & ~supportedFlags) {
		throw Exception("String::find - unsupported compare option flags");
	}
	// else
	std::wstring::iterator found = str.end();
	if (compareOptions & kCFCompareCaseInsensitive) {
		found = std::search(
			str.begin(), str.end(),
			toFind->str.begin(), toFind->str.end(),
			[](wchar_t ch1, wchar_t ch2) { return std::toupper(ch1) == std::toupper(ch2);  }
		);
	}
	else {
		auto index = str.find(toFind->str, 0);
		if (index != std::wstring::npos) {
			return CFRangeMake((CFIndex)index, (CFIndex)toFind->str.length());
		}
	}
	if (found != str.end()) {
		return CFRangeMake((CFIndex)(found - str.begin()), (CFIndex)toFind->str.length());
	}
	// else
	return CFRangeMake(-1, 0);
}

// defined here because of circular dependencies
MutableStringRef String::createMutableCopy() {
	return MutableString::createFromString(this);
}
