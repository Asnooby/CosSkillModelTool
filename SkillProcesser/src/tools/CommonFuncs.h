#pragma once

#include "../IDllExport.h"
#include <string>
#include <map>
#include "../processor/BaseDefine.h"

using namespace std;

extern "C" DLL_API bool isNumber(char& c);
extern "C" DLL_API string SelectNumber(string input);
extern "C" DLL_API string DeepFindInDir(string path, string key);


bool readFileLines(const string path, vector<string>& lines);
vector<string> split(const string& s, const string& seperator);
void parseIniCfg(const string path, map<string, SKILL_BLOCK>& outCfg);
void parseXmlCfg(const string path, const string tag, map<string, SKILL_BLOCK>& outCfg);
int getValueByBase(string srcBase, string srcValue, string dstBase);
void replace_str(std::string& str, const std::string& before, const std::string& after);