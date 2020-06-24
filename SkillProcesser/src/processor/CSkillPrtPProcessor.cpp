#include "CSkillPrtPProcessor.h"
#include "../CSingleton.h"
#include "../CEnvParams.h"
#include "../tools/CommonFuncs.h"

#define MAX_LINE_SIZE 1024

CSkillPrtPProcessor::CSkillPrtPProcessor()
{

}

void CSkillPrtPProcessor::SetPath(const std::string path, bool createIfNotExist/* = false*/)
{
	CFileBase::SetPath(path, createIfNotExist);
	m_skills.clear();
	parseXmlCfg(m_lines, "Presentation", "name", m_skills);
}

void CSkillPrtPProcessor::GetPrtData(const std::set<std::string>& prtNames, std::map<std::string, std::string>& outCfg, std::string skinName, std::string newSkinName)
{
	for (auto& prtName : prtNames)
	{
		auto iter = m_skills.find(prtName);
		if (iter != m_skills.end())
		{
			auto prtName = iter->first;
			auto content = iter->second.block;

			replace_str(prtName, skinName, newSkinName);
			replace_str(content, iter->first, prtName);

			outCfg[prtName] = content;
		}
	}
}

std::string CSkillPrtPProcessor::GetPrtTotalContent(const std::set<std::string>& prtNames)
{
	std::string content;
	content += "<PresentationSet>\r\n";
	for (auto iter = prtNames.begin(); iter != prtNames.end(); iter++)
	{
		auto iterPrt = m_skills.find(*iter);
		if (iterPrt != m_skills.end())
		{
			content += iterPrt->second.block;
		}
	}
	content += "</PresentationSet>";

	return content;
}

std::string CSkillPrtPProcessor::GenerateTotalContent(const std::map<std::string, std::string>& prtData)
{
	std::string content;

	vector<string> lines = m_lines;

	auto iterInsert = lines.end();
	if (lines.empty())
	{
		lines.push_back("<PresentationSet>\r\n");
		lines.push_back("</PresentationSet>");
		iterInsert = lines.end() - 1;
	}

	int nLastLineIndex = -1;
	for (int index = lines.size() - 1; index >= 0; index--)
	{
		if (std::string::npos != lines[index].find("</PresentationSet>"))
		{
			iterInsert = lines.begin() + index;
			break;
		}
	}

	for (auto iter = prtData.begin(); iter != prtData.end(); iter++)
	{
		//if (m_skills.end() == m_skills.find(iter->first))
		//{
			iterInsert = lines.insert(iterInsert, iter->second) + 1;
		//}
	}

	for (auto& line : lines)
	{
		content += line;
	}

	return content;
}

void CSkillPrtPProcessor::ExportGeneratedTotalContent(const std::map<std::string, std::string>& prtData)
{
	SetPath(m_path, true);
	auto content = GenerateTotalContent(prtData);
	WriteTotalContent(content);
}