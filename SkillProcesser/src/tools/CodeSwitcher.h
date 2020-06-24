#pragma once

#include "../IDllExport.h"
#include <string>
#include <map>
#include "../processor/BaseDefine.h"

using namespace std;

enum class FILE_TYPE
{
	UNKNOWN,
	ANSI,
	UTF_8,
	UTF_8_BOM,
	UNICODE_LITTLE_ENDIAN,
	UNICODE_BIG_ENDIAN,
};

string GetFileCodeHeader(FILE_TYPE eType);
bool IsStrUtf8(const char* str);
void UnicodeBigToLittleEndian(std::wstring& wide_string);
std::string WideToAnsi(const std::wstring& wide_string);
std::string WideToUtf8(const std::wstring& wide_string);
std::wstring AnsiToWide(const std::string& ansiString);
std::string AnsiToUtf8(const std::string& ansiString);
std::wstring Utf8ToWide(const std::string& utf8String);
std::string Utf8ToAnsi(const std::string& utf8String);