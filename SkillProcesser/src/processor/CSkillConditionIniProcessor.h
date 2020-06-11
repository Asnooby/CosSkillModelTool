#pragma once

#include "../IDllExport.h"
#include <string>
#include <map>
#include <vector>
#include "BaseDefine.h"

class DLL_API CSkillConditionIniProcessor
{
public:
	CSkillConditionIniProcessor();

public:
	void SetHeroId(std::string heroId);
	std::string GetSkillTotalContent(std::vector<std::string>& skillIds);

private:
	std::map<std::string, SKILL_BLOCK> m_skills;
};