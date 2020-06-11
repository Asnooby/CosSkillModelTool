#pragma once

#include "../IDllExport.h"
#include <string>
#include <map>
#include <vector>
#include "BaseDefine.h"

class DLL_API CSkillPresentationIniProcessor
{
public:
	CSkillPresentationIniProcessor();

public:
	void SetHeroId(std::string heroId);
	void SetSkinId(std::string skinId);
	std::string GetSkillTotalContent(std::vector<std::string>& skillIds);
	void GetSkillPrtName(std::string skinId, std::vector<std::string>& SkillIds, std::map<std::string, std::string>& outCfg);

private:
	std::map<std::string, SKILL_BLOCK> m_skills;
};