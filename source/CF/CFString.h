#ifndef __CF_STRING_H__
#define __CF_STRING_H__

#include "CFObject.h"

#include "../util.h"

namespace cf {

	// STRING CLASS ====================================
	class MutableString; typedef MutableString* MutableStringRef; // fwd decl
	class String; typedef String* StringRef;
	class String : public Object {
	protected:
		std::wstring str;
	public:
		TypeID getTypeID() const override { return kTypeIDString; }
		const char *getTypeName() const override { return "CFString"; }

		String() {}

		String(const String &other)
			:str(other.str) {}

		// createWithCString
		String(const char *cStr) {
			str = utf8_to_wstring(cStr);
		}
		inline static StringRef createWithCString(const char *cStr) {
			return new String(cStr);
		}

		// createWithSubstring
		String(StringRef source, CFRange range) {
			str = source->str.substr(range.location, range.length);
		}
		inline static StringRef createWithSubstring(StringRef source, CFRange range) {
			return new String(source, range);
		}

		CFIndex getLength() { return (CFIndex)str.length(); }

		CFRange find(StringRef toFind, CFStringCompareFlags compareOptions);

		std::wstring getStdString() {
			return str;
		}

		std::string getUtf8String() {
			return wstring_to_utf8(str);
		}

		virtual bool operator <(const Object &b) const override {
			auto other_str = (String &)b;
			return str.compare(other_str.str) < 0;
		}

		virtual std::string toString() const override {
			return sprintfToStdString("String@%p: {%s} [%ls]", this, str.c_str(), Object::toString().c_str());
		}

		virtual StringRef copy() override {
			// this is immutable, so just return itself
			return (StringRef)retain();
		}

		MutableStringRef createMutableCopy(); // defined in cpp because of circular dependencies

		static StringRef makeConstantString(const char *cStr); // defined in the .cpp file because of some static variable stuff we don't expose here (annoying compiler warnings)

		RETAIN_AND_AUTORELEASE(String)
			WEAKREF_MAKE(String)
	};

} // end namespace cf

#endif // __CF_STRING_H__
