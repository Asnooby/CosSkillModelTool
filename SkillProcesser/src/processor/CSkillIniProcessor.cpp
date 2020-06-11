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
	m_skills.clear();
	parseIniCfg(path, m_skills);
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