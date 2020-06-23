#pragma once

#include <string>
#include <vector>
#include "../tools/CodeSwitcher.h"

class CFileBase
{
public:
	CFileBase();
	virtual void SetPath(const std::string path, bool createIfNotExist = false);

protected:
	void ReadFileLines(const std::string path, std::vector<std::string>& lines);
	void WriteTotalContent(const std::string content);

protected:
	std::vector<std::string> m_lines;
	std::string m_path;

private:
	FILE_TYPE m_eFileType;
};