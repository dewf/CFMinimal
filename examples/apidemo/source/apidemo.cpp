#include <stdio.h>
#include "../../../source/CF/CFTypes.h"

using namespace cf;

#define CFSTR(x) String::makeConstantString(x)

int main() {
	Object::startLeakMonitoring();

	auto s = String::createWithCString(u8"hello Л there Ж Ф");

	auto ms = s->createMutableCopy();
	printf("content: [%s] length %d\n", ms->getUtf8String().c_str(), ms->getLength());

	ms->replaceAll(CFSTR(u8"Л there"), CFSTR(u8"u w0t m8"));
	printf("content: [%s] length %d\n", ms->getUtf8String().c_str(), ms->getLength());

	ms->release();
	s->release();
	Object::endLeakMonitoring();
}
