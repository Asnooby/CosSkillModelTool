#pragma once

#include "../IDllExport.h"
#include <string>
#include <map>
#include "../processor/BaseDefine.h"

using namespace std;

extern "C" DLL_API bool isNumber(char& c);
extern "C" DLL_API string SelectNumber(string input);
extern "C" DLL_API string DeepFindInDir(string path, string key);

extern "C" DLL_API bool createFile(const string& path, bool bForced = false);
bool createDirectory(const string& folder);
bool readFileLines(const string& path, vector<string>& lines, unsigned int nSizeHeader = 0);
bool readWideFileLines(const wstring& path, vector<wstring>& lines, unsigned int nSizeHeader = 0);
vector<string> split(const string& s, const string& seperator);
void parseIniCfg(const std::vector<std::string>& lines, map<string, SKILL_BLOCK>& outCfg);
void parseXmlCfg(const std::vector<std::string>& lines, const string tag, const string attr, map<string, SKILL_BLOCK>& outCfg);
int getValueByBase(string srcBase, string srcValue, string dstBase);
void replace_str(std::string& str, const std::string& before, const std::string& after);