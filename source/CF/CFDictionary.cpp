#include "CFDictionary.h"

#include "CFMutableDictionary.h"

using namespace cf;

DictionaryRef Dictionary::EmptyDict = makeEternal(new Dictionary());
