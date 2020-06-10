#include <string>
#include <io.h>
#include "CommonFuncs.h"

extern "C" DLL_API bool isNumber(char& c)
{
	return '0' <= c && '9' >= c;
}

extern "C" DLL_API std::string SelectNumber(std::string input)
{
	std::string str;
	for (auto iter = input.begin(); iter != input.end(); iter++)
	{
		if (isNumber(*iter) || '.' == *iter)
		{
			str.push_back(*iter);
		}
	}

	return str;
}

extern "C" DLL_API std::string DeepFindInDir(std::string path, std::string key)
{
	std::string ret;
	struct __finddata64_t fileinfoSkinName;
	_int64 handle = _findfirst64((path + "*").c_str(), &fileinfoSkinName);
	if (-1 != handle)
	{
		do
		{
			if (0 == strcmp(fileinfoSkinName.name, ".") || 0 == strcmp(fileinfoSkinName.name, ".."))
			{
				continue;
			}

			std::string strName = fileinfoSkinName.name;
			if (std::string::npos != strName.find(key))
			{
				ret = path + strName;
				break;
			}

			if (_A_SUBDIR == fileinfoSkinName.attrib)
			{
				ret = DeepFindInDir(path + strName + "/", key);
				if (!ret.empty())
				{
					break;
				}
			}
		} while (!_findnext64(handle, &fileinfoSkinName));
		_findclose(handle);
	}

	return ret;
}