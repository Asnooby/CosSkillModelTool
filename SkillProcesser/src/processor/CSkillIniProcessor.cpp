#include "CSkillIniProcessor.h"
#include "../CSingleton.h"
#include "../CEnvParams.h"
#include "../tools/CommonFuncs.h"

#define MAX_LINE_SIZE 1024

CSkillIniProcessor::CSkillIniProcessor()
{

}

void CSkillIniProcessor::SetHeroId(std::string heroId)
{
	std::string path = CSingleton::gEnvParams.strProjectPath + "/Debug/singlepackage/heropackage/" + heroId + "_common/data/config/skill.ini";
	SetPath(path);
	m_skills.clear();
	parseIniCfg(m_lines, m_skills);
}

void CSkillIniProcessor::GetSkillData(std::vector<std::string>& skillIds, std::string oldSkillId, std::string newSkillId, std::map<std::string, std::string>& outSkills)
{
	for (auto& skillId : skillIds)
	{
		auto iter = m_skills.find(skillId);
		if (iter != m_skills.end())
		{
			auto SubSkillId = std::to_string(getValueByBase(oldSkillId, iter->second.id, newSkillId));
			auto content = iter->second.block;
			outSkills[SubSkillId] = content;
		}
	}
}

std::string CSkillIniProcessor::GetSkillTotalContent(std::vector<std::string>& skillIds)
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

std::string CSkillIniProcessor::GenerateTotalContent(std::map<std::string, std::string>& skillDatas)
{
	std::string content;

	vector<string> lines = m_lines;

	bool bAddEmptyLine = true;
	for (auto iter = skillDatas.begin(); iter != skillDatas.end(); iter++)
	{
		if (m_skills.end() == m_skills.find(iter->first))
		{
			if (bAddEmptyLine)
			{
				lines.push_back("\r\n");
				lines.push_back("\r\n");
			}
			lines.push_back("[" + iter->first + "]\r\n");
			lines.push_back(iter->second);
		}
	}

	for (auto& line : lines)
	{
		content += line;
	}

	return content;
}

void CSkillIniProcessor::ExportGeneratedTotalContent(std::map<std::string, std::string>& skillDatas)
{
	SetPath(m_path, true);
	auto content = GenerateTotalContent(skillDatas);
	WriteTotalContent(content);
}