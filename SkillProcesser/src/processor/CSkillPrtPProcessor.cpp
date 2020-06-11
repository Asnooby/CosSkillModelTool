#include "CSkillPrtPProcessor.h"
#include "../CSingleton.h"
#include "../CEnvParams.h"
#include "../tools/CommonFuncs.h"

#define MAX_LINE_SIZE 1024

CSkillPrtPProcessor::CSkillPrtPProcessor()
{

}

void CSkillPrtPProcessor::SetPath(std::string path)
{
	m_skills.clear();
	parseXmlCfg(path, "Presentation", m_skills);
}

std::string CSkillPrtPProcessor::GetSkillTotalContent(std::set<std::string>& prtNames)
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