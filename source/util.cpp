#include "util.h"
#include "cfminimal.h"

//#ifdef CF_PLATFORM_WINDOWS
//#include <Windows.h>
//#include <stringapiset.h>
//
//std::wstring utf8_to_wstring(const std::string &utf8) {
//	auto bufSize = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
//	auto outBuffer = new wchar_t[bufSize];
//	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, outBuffer, bufSize);
//	std::wstring ret(outBuffer);
//	delete[] outBuffer;
//	return ret;
//	//return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.from_bytes(utf8.data());
//	//return utf_to_utf<wchar_t>(str.c_str(), str.c_str() + str.size());
//}
//
//std::string wstring_to_utf8(const std::wstring &wideStr) {
//	auto bufSize = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, NULL, 0, NULL, NULL);
//	auto outBuffer = new char[bufSize];
//	WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, outBuffer, bufSize, NULL, NULL);
//	std::string ret(outBuffer);
//	delete[] outBuffer;
//	return ret;
//	//return std::wstring_convert<std::codecvt_utf8<wchar_t>>{}.to_bytes(wideStr);
//	//return utf_to_utf<char>(str.c_str(), str.c_str() + str.size());
//}
//#elif defined CF_PLATFORM_LINUX
//
//#include "3rdparty/utfcpp/utf8.h"
//
////std::wstring utf8_to_wstring(const std::string &utf8) {
////    auto wide = utf8::utf8to16(utf8);
////}
////
////std::string wstring_to_utf8(const std::wstring &wideStr) {
////}
//#endif


