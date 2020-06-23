#include "CFileBase.h"
#include "../tools/CommonFuncs.h"
#include "../tools/CodeSwitcher.h"

#include <fstream>

CFileBase::CFileBase()
{

}

void CFileBase::SetPath(const std::string path, bool createIfNotExist/* = false*/)
{
	m_path = path;

	m_lines.clear();
	FILE* fp;
	fopen_s(&fp, path.c_str(), "rb");
	if (!fp)
	{
		if (createIfNotExist)
		{
			auto folder = path;
			replace_str(folder, "\\", "/");
			auto index = folder.rfind('/');
			if (folder.npos != index)
			{
				folder = folder.substr(0, index);
			}
			createDirectory(folder);
			fopen_s(&fp, path.c_str(), "w");
			fclose(fp);
			fp = nullptr;
		}
	}
	else
	{
		unsigned char c;
		unsigned int header;
		c = fgetc(fp);
		header = c << 8;
		c = fgetc(fp);
		header += c;

		switch (header)
		{
			case 0xfffe:  //65534
			{
				m_eFileType = FILE_TYPE::UNICODE_LITTLE_ENDIAN;
			}break;
			case 0xfeff://65279
			{
				m_eFileType = FILE_TYPE::UNICODE_BIG_ENDIAN;
			}break;
			case 0xefbb://61371
			{
				m_eFileType = FILE_TYPE::UTF_8_BOM;
			}break;
			default:
			{
				m_eFileType = FILE_TYPE::UNKNOWN;
			}
		}
		fclose(fp);
		fp = nullptr;

		ReadFileLines(m_path, m_lines);
	}
}

void CFileBase::ReadFileLines(const std::string path, std::vector<std::string>& lines)
{
	switch (m_eFileType)
	{
		case FILE_TYPE::UNKNOWN:
		{
			::readFileLines(path, lines);
			m_eFileType = FILE_TYPE::UTF_8;
			for (auto& line : lines)
			{
				if (!IsStrUtf8(line.c_str()))
				{
					line = AnsiToUtf8(line);
					m_eFileType = FILE_TYPE::ANSI;
				}
			}
		}break;
		case FILE_TYPE::ANSI:
		{
			::readFileLines(path, lines);
			for (auto& line : lines)
			{
				line = AnsiToUtf8(line);
			}
		}break;
		case FILE_TYPE::UTF_8_BOM:
		{
			m_eFileType = FILE_TYPE::UTF_8;
			::readFileLines(path, lines, 3);
		}break;
		case FILE_TYPE::UNICODE_LITTLE_ENDIAN:
		case FILE_TYPE::UNICODE_BIG_ENDIAN:
		{
			std::vector<std::wstring> wlines;
			::readWideFileLines(Utf8ToWide(path), wlines, 2);
			for (auto& wline : wlines)
			{
				if (FILE_TYPE::UNICODE_BIG_ENDIAN == m_eFileType)
				{
					UnicodeBigToLittleEndian(wline);
				}
				auto line = WideToUtf8(wline);
				lines.push_back(line);
			}
		}break;
		default:
		{
			::readFileLines(path, lines);
		}
	}
}

void CFileBase::WriteTotalContent(const std::string content)
{
	auto header = GetFileCodeHeader(FILE_TYPE::UTF_8);
	FILE* fp;
	fopen_s(&fp, m_path.c_str(), "wb");
	if (fp)
	{
		fwrite(header.c_str(), sizeof(char), header.size(), fp);
		fwrite(content.c_str(), sizeof(char), content.size(), fp);
		fclose(fp);
	}
}