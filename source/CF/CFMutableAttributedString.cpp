#include "CFMutableAttributedString.h"

#include "CFMutableString.h"
#include "CFMutableDictionary.h"

#include "../AbstractRangeManager.h"

using namespace cf;

enum RangeOverlapType {
	AEntirelyBeforeB,   //End(A) <= Start(B)
	AEntirelyAfterB,    //Start(A) >= End(B)
	AOverlapsBStart,    //Start(A) <= Start(B) && End(A) < End(B)
	AOverlapsBEnd,      //Start(A) > Start(B) && End(A) >= End(B)
	AEntirelyInsideB,   //Start(A) > Start(B) && End(A) < End(B)
	AEntirelyContainsB,  //Start(A) <= Start(B) && End(A) => End(B)
	Error
};
RangeOverlapType GetRangeOverlap(CFRange a, CFRange b)
{
	if (CFRangeEnd(a) <= b.location) {
		return AEntirelyBeforeB;
	}
	else if (a.location >= CFRangeEnd(b)) {
		return AEntirelyAfterB;
	}
	else if (a.location <= b.location && CFRangeEnd(a) < CFRangeEnd(b)) {
		return AOverlapsBStart;
	}
	else if (a.location > b.location && CFRangeEnd(a) >= CFRangeEnd(b)) {
		return AOverlapsBEnd;
	}
	else if (a.location > b.location && CFRangeEnd(a) < CFRangeEnd(b)) {
		return AEntirelyInsideB;
	}
	else if (a.location <= b.location && CFRangeEnd(a) >= CFRangeEnd(b)) {
		return AEntirelyContainsB;
	}
	// should never get here
	assert(false);
	return Error;
}

void MutableAttributedString::deleteRange(CFRange delRange) {
	// first delete and adjust lengths of any ranges overlapping with the range param
	auto oldRanges = ranges;
	ranges.clear();
	for (auto i = oldRanges.begin(); i != oldRanges.end(); i++) {
		switch (GetRangeOverlap(delRange, i->range)) {
		case AEntirelyBeforeB:
		case AEntirelyAfterB: {
			// do nothing
			break;
		}
		case AOverlapsBStart: {
			// push range's beginning
			auto bEnd = CFRangeEnd(i->range);
			auto bStart = CFRangeEnd(delRange);
			i->range.location = bStart;
			i->range.length = bEnd - bStart;
			break;
		}
		case AOverlapsBEnd: {
			// remove range's ending
			i->range.length = delRange.location - i->range.location;
			break;
		}
		case AEntirelyInsideB: {
			// shrink length
			i->range.length -= delRange.length;
			break;
		}
		case AEntirelyContainsB: {
			i->attrs->release();
			continue; // delete entirely (skip the push_back below)
			break;
		}
		}
		ranges.push_back(*i);
	}
	// collapse gaps
	CFIndex index = 0;
	for (auto i = ranges.begin(); i != ranges.end(); i++) {
		i->range.location = index;
		index += i->range.length;
	}
}

void MutableAttributedString::insertRange(CFRange insRange) {
	// find which range contains the insert location, and add the insert length to it
	// then fixup the loc of all the ranges after it
	bool postInsert = false;
	CFIndex index = 0;
	for (auto i = ranges.begin(); i != ranges.end(); i++) {
		if (insRange.location >= i->range.location &&
			insRange.location <= CFRangeEnd(i->range))
		{
			i->range.length += insRange.length;
			postInsert = true;
		}
		else if (postInsert) {
			i->range.location = index;
		}
		index += i->range.length;
	}
}

void MutableAttributedString::replaceString(CFRange replRange, StringRef replacement) {
	if (isEditing) {
		// sorry, have to do it
		coalesce();
		// ... but continue until the client code formally calls endEditing()
	}
	//
	auto mut = str->createMutableCopy();
	mut->replaceString(replRange, replacement);
	str->release();
	str = mut->copy();
	mut->release();
	deleteRange(replRange);
	insertRange(CFRangeMake(replRange.location, replacement->getLength()));
}

// helper class to manage attributed range updates
class cf::AttrStringRangeManager : public AbstractRangeMananger<MutableDictionary, Dictionary>
{
private:
	MutableAttributedStringRef target;
public:
	AttrStringRangeManager(MutableAttributedStringRef target) : target(target) {}

	MutableDictionaryRef newAccum() override {
		return new MutableDictionary();
	};
	void releaseAccum(MutableDictionaryRef m) override {
		m->release();
	};
	void accumulate(MutableDictionaryRef accum, DictionaryRef item) override {
		auto count = item->getCount();
		auto keys = new ObjectRef[count];
		auto values = new ObjectRef[count];
		item->getKeysAndValues(keys, values);
		// merge keys and values from item, overwriting anything in accum
		for (int i = 0; i < count; i++) {
			accum->setValue(keys[i], values[i]);
		}
		delete keys;
		delete values;
	};

	void applyRangeSpec(RangeSpec *rangeSpec) override {
		// do something with the now non-overlapping range
		// note that the dict (rangeSpec->spec) needs to be retained/copied, as the calling code will release it right after we return
		AttributedString::RangeAttrs ra;
		ra.range = rangeSpec->range;
		ra.attrs = rangeSpec->spec->copy(); // it's a mutable dict coming in, so make an immutable copy for safety
		target->ranges.push_back(ra);
	};

	void apply() {
		target->ranges.clear();
		AbstractRangeMananger::apply();
	}
};


void MutableAttributedString::coalesce()
{
	AttrStringRangeManager manager(this);

	std::vector<DictionaryRef> toFree;

	// dump the existing ranges to the manager
	for (auto i = ranges.begin(); i != ranges.end(); i++) {
		manager.addRange(i->range, i->attrs);
		// note that it knows nothing of ownership, they are still ours to free
		toFree.push_back(i->attrs);
	}

	// then all the new stuff
	for (auto i = queuedChanges.begin(); i != queuedChanges.end(); i++) {
		auto dict = new Dictionary((ObjectRef *)&i->key, &i->value, 1);
		manager.addRange(i->range, dict);
		toFree.push_back(dict);
		// can release the key/value now, the dictionary is holding the ref
		i->key->release();
		i->value->release();
	}
	queuedChanges.clear();

	// do the magic!
	manager.apply(); // the ranges will be cleared inside here

	// free all the dictionaries that were marked for releasing (the old ranges + the one-off dictionaries we just created)
	for (auto i = toFree.begin(); i != toFree.end(); i++) {
		(*i)->release();
	}
}
