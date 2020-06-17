#include "CSkillPresentationIniProcessor.h"
#include "../CSingleton.h"
#include "../CEnvParams.h"
#include "../tools/CommonFuncs.h"
#include <algorithm>

#define MAX_LINE_SIZE 1024

CSkillPresentationIniProcessor::CSkillPresentationIniProcessor()
{

}

void CSkillPresentationIniProcessor::SetHeroId(std::string heroId)
{
	std::string path = CSingleton::gEnvParams.strProjectPath + "/Debug/singlepackage/heropackage/" + heroId + "_common/data/config/skillpresentation.ini";
	SetPath(path);
	m_skills.clear();
	parseIniCfg(m_lines, m_skills);
}

void CSkillPresentationIniProcessor::SetSkinId(std::string skinId)
{
	std::string path = CSingleton::gEnvParams.strProjectPath + "/Debug/singlepackage/heropackage/" + skinId + "/data/config/skillpresentation.ini";
	SetPath(path);
	m_skills.clear();
	parseIniCfg(m_lines, m_skills);
}

std::string CSkillPresentationIniProcessor::GetSkillTotalContent(std::vector<std::string>& skillIds)
{
	std::string content;
	for (auto& skillId : skillIds)
	{
		auto iter = m_skills.find(skillId);
		if (iter != m_skills.end())
		{
			content += "\r\n[" + iter->second.id + "]\r\n" + iter->second.block;
		}
	}

	return content;
}

std::string CSkillPresentationIniProcessor::GenerateTotalContent(std::string skinId, std::map<std::string, std::string>& skillDatas)
{
	std::string content;

	vector<string> lines = m_lines;

	bool bAddEmptyLine = true;
	for (auto iter = skillDatas.begin(); iter != skillDatas.end(); iter++)
	{
		int insertIndex = -1;
		if (m_skills.end() != m_skills.find(iter->first))
		{
			if (std::string::npos != m_skills[iter->first].block.find(skinId))
			{
				continue;
			}
			else
			{
				insertIndex = m_skills[iter->first].endLine;
			}
		}

		std::string line = skinId + " = " + iter->second;
		if (-1 == insertIndex)
		{
			if (!lines.empty())
			{
				auto lineBack = lines.back();
				if (0 != lineBack.compare("\r\n") && 0 != lineBack.compare("\n") && 0 != lineBack.compare("\r"))
				{
					lines.push_back("\r\n\r\n");
				}
			}
			lines.push_back("[" + iter->first + "]\r\n");
			lines.push_back(line);
			lines.push_back("\r\n");
		}
		else
		{
			lines.insert(lines.begin() + insertIndex, line);
		}
	}

	for (auto& line : lines)
	{
		content += line;
	}

	return content;
}

void CSkillPresentationIniProcessor::ExportGeneratedTotalContent(std::string skinId, std::map<std::string, std::string>& skillDatas)
{
	SetPath(m_path, true);
	auto content = GenerateTotalContent(skinId, skillDatas);
	WriteTotalContent(content);
}

void CSkillPresentationIniProcessor::GetSkillPrtName(std::string skinId, std::vector<std::string>& SkillIds, std::map<std::string, std::string>& outCfg, std::string skillId/* = ""*/, std::string newSkillId/* = ""*/)
{
	std::vector<std::string> vRet;
	for (auto&  id : SkillIds)
	{
		auto iter = m_skills.find(id);
		if (iter != m_skills.end())
		{
			auto index_1 = iter->second.block.find(skinId);
			if (std::string::npos == index_1)
			{
				index_1 = iter->second.block.find("name");
			}
			if (std::string::npos != index_1)
			{
				index_1 = iter->second.block.find('=', index_1 + 1);
				auto index_2 = iter->second.block.find('\r', index_1 + 1);
				if (std::string::npos == index_2)
				{
					index_2 = iter->second.block.find('\n', index_1 + 1);
				}
				if (std::string::npos == index_2)
				{
					index_2 = iter->second.block.length();
				}
				auto prt_name = iter->second.block.substr(index_1 + 1, index_2 - index_1 - 1);
				prt_name.erase(std::remove(prt_name.begin(), prt_name.end(), ' '), prt_name.end());
				prt_name.erase(std::remove(prt_name.begin(), prt_name.end(), '\r'), prt_name.end());
				prt_name.erase(std::remove(prt_name.begin(), prt_name.end(), '\n'), prt_name.end());

				auto SubSkillId = std::to_string(getValueByBase(skillId, id, newSkillId));
				outCfg[SubSkillId] = prt_name;
			}
		}
	}
}

void CSkillPresentationIniProcessor::SetSkinName(std::map<std::string, std::string>& skillData, std::string skinName, std::string newSkinName)
{
	for (auto iter = skillData.begin(); iter != skillData.end(); iter++)
	{
		if (!skinName.empty() && !newSkinName.empty())
		{
			replace_str(iter->second, skinName, newSkinName);
		}
	}
}