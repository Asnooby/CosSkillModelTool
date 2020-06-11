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
	m_skills.clear();
	parseIniCfg(path, m_skills);
}

void CSkillPresentationIniProcessor::SetSkinId(std::string skinId)
{
	std::string path = CSingleton::gEnvParams.strProjectPath + "/Debug/singlepackage/heropackage/" + skinId + "/data/config/skillpresentation.ini";
	parseIniCfg(path, m_skills);
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

void CSkillPresentationIniProcessor::GetSkillPrtName(std::string skinId, std::vector<std::string>& SkillIds, std::map<std::string, std::string>& outCfg)
{
	std::vector<std::string> vRet;
	for (auto&  skillId : SkillIds)
	{
		auto iter = m_skills.find(skillId);
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
				outCfg[skillId] = prt_name;
			}
		}
	}
}