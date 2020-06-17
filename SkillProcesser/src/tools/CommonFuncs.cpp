#include <string>
#include <vector>
#include <io.h>
#include <direct.h>
#include "CommonFuncs.h"


bool isNumber(char& c)
{
	return '0' <= c && '9' >= c;
}

string SelectNumber(string input)
{
	string str;
	for (auto iter = input.begin(); iter != input.end(); iter++)
	{
		if (isNumber(*iter) || '.' == *iter)
		{
			str.push_back(*iter);
		}
	}

	return str;
}

string DeepFindInDir(string path, string key)
{
	string ret;
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

			string strName = fileinfoSkinName.name;
			if (string::npos != strName.find(key))
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

bool readFileLines(const string path, vector<string>& lines, unsigned int nSizeHeader/* = 0*/)
{
	FILE* file;
	fopen_s(&file, path.c_str(), "rb");
	if (file)
	{
		fseek(file, nSizeHeader, SEEK_SET);
		char pBuffer[MAX_LINE_SIZE];
		memset(pBuffer, MAX_LINE_SIZE, 0);
		while (fgets(pBuffer, MAX_LINE_SIZE, file))
		{
			lines.push_back(pBuffer);
			memset(pBuffer, MAX_LINE_SIZE, 0);
		}
		fclose(file);
		return true;
	}
	return false;
}

bool readWideFileLines(const wstring path, vector<wstring>& lines, unsigned int nSizeHeader/* = 0*/)
{
	FILE* file;
	_wfopen_s(&file, path.c_str(), L"rb,ccs=UNICODE");
	if (file)
	{
		fseek(file, nSizeHeader, SEEK_SET);
		wchar_t pBuffer[MAX_LINE_SIZE];
		memset(pBuffer, MAX_LINE_SIZE * sizeof(wchar_t), 0);
		while (fgetws(pBuffer, MAX_LINE_SIZE, file))
		{
			lines.push_back(pBuffer);
			memset(pBuffer, MAX_LINE_SIZE * sizeof(wchar_t), 0);
		}
		fclose(file);
		return true;
	}
	return false;
}

vector<string> split(const string& s, const string& seperator) {
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != s.size()) {
		int flag = 0;
		while (i != s.size() && flag == 0) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
		}

		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0) {
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]) {
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j) {
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}

void parseIniCfg(const std::vector<std::string>& lines, map<string, SKILL_BLOCK>& outCfg)
{
	if (!lines.empty())
	{
		SKILL_BLOCK curNode;
		unsigned int lineIndex = 0;
		for (auto& line : lines)
		{
			if (';' == line[0] || 0 == line.substr(0, 2).compare("//") || 0 == line.substr(0, 2).compare("\\\\"))
				continue;

			if ('[' == line[0] && line.npos != line.find(']'))
			{
				if (!curNode.id.empty())
				{
					curNode.endLine = lineIndex - 1;
					while (0 == lines[curNode.endLine].compare("\r") || 0 == lines[curNode.endLine].compare("\n") || 0 == lines[curNode.endLine].compare("\r\n"))
						curNode.endLine = curNode.endLine - 1;
					outCfg[curNode.id] = curNode;
				}
				curNode.init();
				curNode.startLine = lineIndex;
				curNode.id = line.substr(line.find('[') + 1, line.find(']') - line.find('[') - 1);
			}
			else if (!curNode.id.empty())
			{
				if (!line.empty())
				{
					curNode.block += line;
				}
			}

			lineIndex++;
		}
		if (!curNode.id.empty())
		{
			curNode.endLine = lineIndex - 1;
			while (0 == lines[curNode.endLine].compare("\r") || 0 == lines[curNode.endLine].compare("\n") || 0 == lines[curNode.endLine].compare("\r\n"))
				curNode.endLine = curNode.endLine - 1;
			outCfg[curNode.id] = curNode;
		}
	}
}

void parseXmlCfg(const std::vector<std::string>& lines, const string tag, map<string, SKILL_BLOCK>& outCfg)
{
	if (!lines.empty())
	{
		SKILL_BLOCK curNode;
		unsigned int lineIndex = 0;
		for (auto& line : lines)
		{
			if (line.npos != line.find("<" + tag + " "))
			{
				curNode.init();
				curNode.startLine = lineIndex;
				auto index_1 = line.find("name");
				if (line.npos != index_1)
				{
					index_1 = line.find('"', index_1 + 1);
					auto index_2 = line.find('"', index_1 + 1);
					curNode.id = line.substr(index_1 + 1, index_2 - index_1 - 1);
					curNode.block += line;
				}
			}
			else if (!curNode.id.empty())
			{
				if (!line.empty())
				{
					curNode.block += line;

					if (line.npos != line.find("/" + tag + ">"))
					{
						curNode.endLine = lineIndex - 1;
						while (0 == lines[curNode.endLine].compare("\r") || 0 == lines[curNode.endLine].compare("\n") || 0 == lines[curNode.endLine].compare("\r\n"))
							curNode.endLine = curNode.endLine - 1;
						outCfg[curNode.id] = curNode;
					}
				}
			}

			lineIndex++;
		}
	}
}

int getValueByBase(string srcBase, string srcValue, string dstBase)
{
	return atoi(dstBase.c_str()) + atoi(srcValue.c_str()) - atoi(srcBase.c_str());
}

void replace_str(std::string& str, const std::string& before, const std::string& after)
{
	for (std::string::size_type pos(0); pos != std::string::npos; pos += after.length())
	{
		pos = str.find(before, pos);
		if (pos != std::string::npos)
			str.replace(pos, before.length(), after);
		else
			break;
	}
}

bool createDirectory(const string folder)
{
	std::string folder_builder;
	std::string sub;
	sub.reserve(folder.size());
	for (auto it = folder.begin(); it != folder.end(); ++it)
	{
		//cout << *(folder.end()-1) << endl;
		const char c = *it;
		sub.push_back(c);
		if (c == '\\' || c == '/' || it == folder.end() - 1)
		{
			folder_builder.append(sub);
			if (0 != ::_access(folder_builder.c_str(), 0))
			{
				// this folder not exist
				if (0 != ::_mkdir(folder_builder.c_str()))
				{
					// create failed
					return false;
				}
			}
			sub.clear();
		}
	}
	return true;
}