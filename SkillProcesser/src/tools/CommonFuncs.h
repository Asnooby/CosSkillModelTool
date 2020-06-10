#pragma once
#include <string>

#define _DLL_EXPORTS
#include "../IDllExport.h"

extern "C" DLL_API bool isNumber(char& c);
extern "C" DLL_API std::string SelectNumber(std::string input);
extern "C" DLL_API std::string DeepFindInDir(std::string path, std::string key);