#include "CSkillPrtCProcessor.h"
#include "../CSingleton.h"
#include "../CEnvParams.h"
#include "../tools/CommonFuncs.h"

#define MAX_LINE_SIZE 1024

CSkillPrtCProcessor::CSkillPrtCProcessor()
{

}

void CSkillPrtCProcessor::SetPath(std::string path)
{
	m_skills.clear();
	parseXmlCfg(path, "SkillPresentation", m_skills);
}

void CSkillPrtCProcessor::GetPrtNames(std::map<std::string, std::string>& skillPrtNames, std::set<std::string>& outCfg)
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

std::string CSkillPrtCProcessor::GetSkillTotalContent(std::map<std::string, std::string>& prtNames)
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