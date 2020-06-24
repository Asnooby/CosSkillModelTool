#include "CSkillPrtCProcessor.h"
#include "../CSingleton.h"
#include "../CEnvParams.h"
#include "../tools/CommonFuncs.h"

#define MAX_LINE_SIZE 1024

CSkillPrtCProcessor::CSkillPrtCProcessor()
{

}

void CSkillPrtCProcessor::SetPath(const std::string path, bool createIfNotExist/* = false*/)
{
	CFileBase::SetPath(path, createIfNotExist);
	m_skills.clear();
	parseXmlCfg(m_lines, "SkillPresentation", "name", m_skills);
}

void CSkillPrtCProcessor::GetPrtNames(const std::map<std::string, std::string>& skillPrtNames, std::set<std::string>& outCfg)
{
	for (auto iter = skillPrtNames.begin(); iter != skillPrtNames.end(); iter++)
	{
		auto iterSkill = m_skills.find(iter->second);
		if (iterSkill != m_skills.end())
		{
			const char* key = "presentationName";
			auto index_1 = iterSkill->second.block.find(key);
			while (index_1 != std::string::npos)
			{
				index_1 = iterSkill->second.block.find('"', index_1 + 1);
				auto index_2 = iterSkill->second.block.find('"', index_1 + 1);
				outCfg.insert(iterSkill->second.block.substr(index_1 + 1, index_2 - index_1 - 1));

				index_1 = iterSkill->second.block.find(key, index_2 + 1);
			}
		}
	}
}

void CSkillPrtCProcessor::GetSkillPrtData(const std::map<std::string, std::string>& skillPrtNames, std::map<std::string, std::string>& outCfg, std::string skinName/* = ""*/, std::string newSkinName/* = ""*/)
{
	for (auto iter = skillPrtNames.begin(); iter != skillPrtNames.end(); iter++)
	{
		if (m_skills.end() != m_skills.find(iter->second))
		{
			auto prtName = iter->second;
			auto content = m_skills[iter->second].block;
			if (!skinName.empty() || newSkinName.empty())
			{
				replace_str(prtName, skinName, newSkinName);
				replace_str(content, skinName, newSkinName);
			}
			outCfg[prtName] = content;
		}
	}
}

std::string CSkillPrtCProcessor::GetSkillTotalContent(const std::map<std::string, std::string>& prtNames)
{
	std::string content;
	content += "<SkillPresentationSet>\r\n";
	for (auto iter = prtNames.begin(); iter != prtNames.end(); iter++)
	{
		auto iterPrt = m_skills.find(iter->second);
		if (iterPrt != m_skills.end())
		{
			content += iterPrt->second.block;
		}
	}
	content += "</SkillPresentationSet>";

	return content;
}

std::string CSkillPrtCProcessor::GenerateTotalContent(const std::map<std::string, std::string>& skillData)
{
	std::string content;

	vector<string> lines = m_lines;

	auto iterInsert = lines.end();
	if (lines.empty())
	{
		lines.push_back("<SkillPresentationSet>\r\n");
		lines.push_back("</SkillPresentationSet>");
		iterInsert = lines.end() - 1;
	}
	else
	{
		for (int index = lines.size() - 1; index >= 0; index--)
		{
			if (std::string::npos != lines[index].find("</SkillPresentationSet>"))
			{
				iterInsert = lines.begin() + index;
				break;
			}
		}
	}

	for (auto iter = skillData.begin(); iter != skillData.end(); iter++)
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

void CSkillPrtCProcessor::ExportGeneratedTotalContent(const std::map<std::string, std::string>& skillData)
{
	SetPath(m_path, true);
	auto content = GenerateTotalContent(skillData);
	WriteTotalContent(content);
}