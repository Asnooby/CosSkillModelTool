#pragma once

#include "../IDllExport.h"
#include <string>
#include <map>
#include <set>
#include "BaseDefine.h"

class DLL_API CSkillPrtPProcessor
{
public:
	CSkillPrtPProcessor();

public:
	void SetPath(std::string path);
	std::string GetSkillTotalContent(std::set<std::string>& prtNames);

private:
	std::map<std::string, SKILL_BLOCK> m_skills;
};