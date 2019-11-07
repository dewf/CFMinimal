#ifndef __CF_ARRAY_H__
#define __CF_ARRAY_H__

#include "CFObject.h"

namespace cf {

	class Array; typedef Array* ArrayRef;
	class Array : public Object {
	protected:
		std::vector<ObjectRef> items;

		Array() {}
	public:
		TypeID getTypeID() const override { return kTypeIDArray; }
		const char *getTypeName() const override { return "CFArray"; }

		Array(ObjectRef *values, CFIndex numValues) {
			for (CFIndex i = 0; i < numValues; i++) {
				items.push_back(values[i]->retain());
			}
		}
		inline static ArrayRef create(ObjectRef *values, CFIndex numValues) {
			return new Array(values, numValues);
		}

		virtual ~Array() override {
			for (auto i = items.begin(); i != items.end(); i++) {
				(*i)->release();
			}
		}

		CFIndex getCount() { return (CFIndex)items.size(); }
		const ObjectRef getValueAtIndex(CFIndex idx) {
			if (idx >= 0 && idx < (CFIndex)items.size()) {
				return items[idx];
			}
			else {
				throw Exception("Array::getValueAtIndex() out of range");
			}
		}

		// internal API private (ie, non-CF method)
		std::vector<ObjectRef> _getVector() {
			return items;
		}

		virtual std::string toString() const override {
			return sprintfToStdString("Array@%p [%s]", this, Object::toString().c_str());
		}

		virtual ArrayRef copy() override {
			// immutable, so just return self
			return (ArrayRef)retain();
		}

		RETAIN_AND_AUTORELEASE(Array);
		WEAKREF_MAKE(Array);
	};
}

#endif
